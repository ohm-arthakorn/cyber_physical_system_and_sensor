#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
const char* ssid = "yourSSID";
const char* pass = "yourPASS";
AsyncWebServer server(80);
void setup() {
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) delay(500);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
  request->send(200, "text/html", "<html><body><h1>Smart Meter</h1><div
  id='data'></div><script>setInterval(()=>fetch('/data').then(r=>r.text()).then(t=>document.get
  ElementById('data').innerHTML=t),1000);</script></body></html>");
  });
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest* request) {
    // อ่าน ADC แล้วส่งค่า
    float vin = getVoltage();
    float current = getCurrent();
    float power = vin * current;
    char buf[64];
    snprintf(buf, sizeof(buf), "Voltage=%.2f V, Current=%.2f A, Power=%.2f W",vin, current, power);
    request->send(200, "text/plain", buf);
  });
  server.begin();
}