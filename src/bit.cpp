#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config_bit.h"
#include <config_bit.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
WiFiClientSecure client;
HTTPClient http;
WebServer server(80);

String currency = "USD";
float lastPrice = 0.0;
unsigned long lastUpdate = 0;
unsigned long updateInterval = 60000; // cada minuto

// ---------------------------------------------------
// DIBUJO OLED
void drawCentered(const String &l1, const String &l2 = "", const String &l3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int16_t x1, y1; uint16_t w, h;
  int y = 8;
  display.getTextBounds(l1, 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, y); display.println(l1); y += h + 4;
  if (l2.length()) { display.getTextBounds(l2, 0, 0, &x1, &y1, &w, &h); display.setCursor((SCREEN_WIDTH - w) / 2, y); display.println(l2); y += h + 4; }
  if (l3.length()) { display.getTextBounds(l3, 0, 0, &x1, &y1, &w, &h); display.setCursor((SCREEN_WIDTH - w) / 2, y); display.println(l3); }
  display.display();
}

// ---------------------------------------------------
// CONEXIÓN WI-FI
void connectWiFi() {
  WiFi.mode(WIFI_STA);
  IPAddress dns(8,8,8,8); // DNS de Google
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, dns);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  drawCentered("Wi-Fi", "Conectando...");
  uint8_t tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries++ < 40) delay(250);
  if (WiFi.status() == WL_CONNECTED) {
    drawCentered("Wi-Fi OK", WiFi.localIP().toString());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    drawCentered("Wi-Fi FAIL");
  }
}


// ---------------------------------------------------
float fetchBTC(float &changePct, String &symbol) {
  WiFiClientSecure client;
  client.setInsecure();  // desactiva verificación del certificado SSL
  http.begin(client, API_URL);
  int code = http.GET();
  Serial.print("HTTP code: ");
  Serial.println(code);

  if (code != 200) {
    http.end();
    return -1;
  }

  String json = http.getString();
  Serial.println(json);
  http.end();

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, json);
  if (error) return -1;

  // 👇 aquí el cambio importante
  String currLower = currency;
  currLower.toLowerCase();

  float price = doc["bitcoin"][currLower].as<float>();
  symbol = (currency == "USD") ? "$" : (currency == "EUR") ? "€" : "£";

  if (lastPrice > 0 && price > 0) {
  changePct = ((price - lastPrice) / lastPrice) * 100.0;
} else {
  changePct = 0;
}

// 👇 actualiza después del cálculo
lastPrice = price;

  return price;
}

// ---------------------------------------------------
// ENVÍO A THINGSPEAK
void sendToThingSpeak(float price, float changePct) {
  String url = String(THINGSPEAK_URL) + "?api_key=" + THINGSPEAK_WRITE_KEY +
               "&field1=" + String(price, 2) +
               "&field2=" + String(changePct, 2) +
               "&field3=" + (currency == "USD" ? "1" : currency == "EUR" ? "2" : "3") +
               "&field4=" + String(WiFi.RSSI());
  client.setInsecure();
  http.begin(client, url);
  int code = http.GET();
  http.end();
  Serial.println("ThingSpeak status: " + String(code));
}

// ---------------------------------------------------
// INTERFAZ WEB
void handleRoot() {
  String html = R"(
  <html>
  <head><title>CryptoMonitor IoT</title></head>
  <body style='font-family:Arial;background:#111;color:#eee;text-align:center'>
    <h2>💹 CryptoMonitor IoT</h2>
    <p>Moneda actual: <b id='curr'></b></p>
    <form action='/set'>
      <select name='currency' style='font-size:18px;padding:5px;'>
        <option value='USD'>USD 🇺🇸</option>
        <option value='EUR'>EUR 🇪🇺</option>
        <option value='GBP'>GBP 🇬🇧</option>
      </select>
      <input type='submit' value='Cambiar' style='font-size:18px;margin-left:10px;'>
    </form>
    <p style='margin-top:40px;font-size:13px;'>Datos cargados en ThingSpeak cada minuto.</p>
  </body>
  <script>document.getElementById('curr').innerText = ')" + currency + R"(';</script>
  </html>)";
  server.send(200, "text/html; charset=utf-8", html);

}

void handleSet() {
  if (server.hasArg("currency")) {
    currency = server.arg("currency");
    drawCentered("Moneda cambiada", currency);
    Serial.println("Nueva moneda: " + currency);
  }
  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------------------------------------------------
// SETUP
void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("No OLED");
  }

  drawCentered("CryptoMonitor", "Inicializando...");
  connectWiFi();

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
  Serial.println("Servidor web iniciado");
  drawCentered("CryptoMonitor", "IP:", WiFi.localIP().toString());
}

// ---------------------------------------------------
// LOOP PRINCIPAL
void loop() {
  server.handleClient();
  unsigned long now = millis();

  if (now - lastUpdate > updateInterval) {
    lastUpdate = now;
    float changePct; String symbol;
    float price = fetchBTC(changePct, symbol);

    if (price > 0) {
      String trend = (changePct > 0.05)
  ? "UP +" + String(changePct, 2) + "%"
  : (changePct < -0.05)
      ? "DOWN " + String(changePct, 2) + "%"
      : "STABLE 0.00%";
 
      drawCentered("BTC/" + currency, symbol + String(price, 2), trend);
      sendToThingSpeak(price, changePct);
    } else {
      drawCentered("Error API", "Reintentando...");
    }
  }

  delay(100);
}
