# Documentación de API (referencias externas usadas por el firmware)

## Football-Data (Soccer)
- Base: `https://api.football-data.org/v4/`
- Header: `X-Auth-Token: <TU_TOKEN>`
- Endpoint ejemplo: `/matches?dateFrom=YYYY-MM-DD&dateTo=YYYY-MM-DD&status=LIVE,IN_PLAY,PAUSED,FINISHED&competitions=PL`
- Campo `status`: `SCHEDULED | LIVE | IN_PLAY | PAUSED | FINISHED | ...`

## NBA (balldontlie)
- Base: `https://nba.balldontlie.io/api/v1`
- Auth: `Authorization: Bearer <API_KEY>`
- Endpoint ejemplo: `/games?team_ids[]=14&per_page=1`
- Campos: `status`, `home_team_score`, `visitor_team_score`, etc.

## ThingSpeak
- Write: `https://api.thingspeak.com/update.json?api_key=<WRITE>&field1=<rssi>&field2=<uptime>&field3=<interval>&field4=<screen>`
- TalkBack (opcional): `https://api.thingspeak.com/talkbacks/<ID>/commands/execute.json?api_key=<KEY>`