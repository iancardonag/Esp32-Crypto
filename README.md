💹 CryptoMonitor IoT — Monitoreo de Bitcoin con ESP32 + OLED + ThingSpeak + CoinGecko API

Autores:
👨‍💻 Ian Cardona  
👨‍💻 Keiner Astos  

Asignatura: Computación en la Nube  
Actividad: Taller 4 — Sistema IoT con ESP32 y Servicios en la Nube  

---------------------------------------------------------------
🎯 OBJETIVO GENERAL
---------------------------------------------------------------
Desarrollar un sistema IoT completo que integre un ESP32 con un display OLED SSD1306 y servicios en la nube, demostrando competencias en Internet de las Cosas (IoT), APIs REST, visualización local y computación distribuida.

---------------------------------------------------------------
🎯 OBJETIVOS ESPECÍFICOS
---------------------------------------------------------------
1. Implementar un dispositivo IoT con ESP32 y display OLED SSD1306.  
2. Integrar servicios web externos (CoinGecko API).  
3. Enviar información a la nube mediante ThingSpeak.  
4. Desarrollar una interfaz web local para control del sistema.  
5. Implementar manejo de errores y reconexión automática Wi-Fi.  
6. Documentar el proceso completo de diseño, pruebas y solución de errores.  

---------------------------------------------------------------
🧩 DESCRIPCIÓN DEL PROYECTO
---------------------------------------------------------------
El sistema **CryptoMonitor IoT** monitorea en tiempo real el valor de Bitcoin (BTC) en diferentes monedas (USD, EUR, GBP).  
Cada minuto obtiene los precios actualizados desde la **API de CoinGecko**, los muestra en el **OLED SSD1306**, y envía los datos (precio, variación y señal Wi-Fi) al canal de **ThingSpeak**.  

Además, incluye un **servidor web local (ESP32 WebServer)** para cambiar la moneda desde el navegador del usuario.  

---------------------------------------------------------------
⚙️ COMPONENTES REQUERIDOS
---------------------------------------------------------------
**HARDWARE**
- ESP32 DevKit v1  
- Pantalla OLED SSD1306 (128x64, I2C)  
- Cables Dupont Macho-Hembra  
- Conexión Wi-Fi con acceso a Internet  

**SOFTWARE**
- PlatformIO o Arduino IDE  
- Librerías:  
  - WiFi.h  
  - HTTPClient.h  
  - WiFiClientSecure.h  
  - ArduinoJson.h  
  - Adafruit_GFX.h  
  - Adafruit_SSD1306.h  

**SERVICIOS EN LA NUBE**
- ThingSpeak  
- CoinGecko API  

---------------------------------------------------------------
📡 FUNCIONAMIENTO GENERAL
---------------------------------------------------------------
1. El ESP32 se conecta a la red Wi-Fi definida en `config_bit.h`.  
2. Solicita el valor actual del Bitcoin mediante la **API de CoinGecko**.  
3. Calcula la variación porcentual respecto a la lectura anterior.  
4. Muestra el valor, símbolo y tendencia en el display OLED.  
5. Envía los datos al canal de ThingSpeak.  
6. Permite cambiar la moneda desde una interfaz web local.  

---------------------------------------------------------------
🌐 INTERFAZ WEB LOCAL
---------------------------------------------------------------
Cuando el ESP32 se conecta, imprime su IP local en el monitor serie:  


Luego, abre esa IP en tu navegador:  
👉 `http://192.168.1.45`

Allí podrás seleccionar la moneda (USD, EUR, GBP) y cambiarla con un botón.  
El cambio se refleja automáticamente en el OLED y en los datos enviados a ThingSpeak.  

---------------------------------------------------------------
🖥️ VISUALIZACIÓN OLED
---------------------------------------------------------------
Ejemplo de pantalla:  
BTC/USD
$106477.00
UP +0.34%


Cada 60 segundos el valor se actualiza automáticamente.  

---------------------------------------------------------------
☁️ ENVÍO A THINGSPEAK
---------------------------------------------------------------
Campos configurados:  
- **field1** → Precio actual de BTC  
- **field2** → Variación porcentual  
- **field3** → Moneda (1=USD, 2=EUR, 3=GBP)  
- **field4** → Intensidad de señal Wi-Fi (RSSI)  

Estos datos se pueden visualizar en gráficos directamente desde ThingSpeak.  

┌──────────────┐
│   CoinGecko  │─────► Datos de Bitcoin (USD/EUR/GBP)
└──────────────┘
        │
        ▼
┌──────────────────┐
│      ESP32       │
│  - Wi-Fi Client  │
│  - HTTP Request  │
│  - OLED Display  │
│  - WebServer     │
└──────────────────┘
        │
        ▼
┌──────────────────┐
│   ThingSpeak     │
│  Canal IoT Cloud │
└──────────────────┘


---------------------------------------------------------------
🧪 PRUEBAS Y ERRORES ENCONTRADOS
---------------------------------------------------------------
**Problema 1:** DNS Failed for api.coindesk.com  
🧩 *Causa:* API descontinuada.  
✅ *Solución:* Se reemplazó por **CoinGecko API**, más estable y moderna.  

**Problema 2:** HTTP code: -1  
🧩 *Causa:* Petición HTTPS sin cliente seguro.  
✅ *Solución:* Se añadió `WiFiClientSecure` y `client.setInsecure()`.  

**Problema 3:** Variación 0.00%  
🧩 *Causa:* El valor `lastPrice` se actualizaba antes del cálculo.  
✅ *Solución:* Se movió la actualización al final del bloque de lectura.  

**Problema 4:** Caracteres extraños en OLED  
🧩 *Causa:* OLED no soporta caracteres Unicode.  
✅ *Solución:* Se reemplazaron por símbolos ASCII (UP, DOWN, STABLE).  

**Problema 5:** Fallos con hotspot móvil  
🧩 *Causa:* DNS del iPhone no resolvía dominios HTTPS correctamente.  
✅ *Solución:* Se añadió DNS manual `8.8.8.8` en la configuración Wi-Fi.  

---------------------------------------------------------------
📊 RESULTADOS OBTENIDOS
---------------------------------------------------------------
✅ Sistema funcional y estable.  
✅ OLED muestra datos en tiempo real.  
✅ Envío correcto de datos a ThingSpeak.  
✅ Interfaz web local operativa.  
✅ Comunicación HTTPS exitosa con CoinGecko.  

---------------------------------------------------------------
💡 MEJORAS FUTURAS
---------------------------------------------------------------
- Soporte para más criptomonedas (ETH, DOGE, LTC).  
- Historial local de precios y gráficas en OLED.  
- Alerta por cambio brusco de precio.  
- Gráficas de tendencia en ThingSpeak.  
- Panel web más interactivo.  

---------------------------------------------------------------
📘 CONCLUSIÓN
---------------------------------------------------------------
El proyecto **CryptoMonitor IoT** demuestra la integración completa entre hardware, software y servicios en la nube.  
Cumple los objetivos del taller mostrando la interacción entre IoT y computación en la nube, utilizando **APIs REST**, **procesamiento local**, **visualización OLED**, e **integración con plataformas Cloud (ThingSpeak)**.  

Este sistema representa una base sólida para aplicaciones de monitoreo financiero o ambiental en tiempo real, usando dispositivos de bajo costo y arquitectura distribuida.
