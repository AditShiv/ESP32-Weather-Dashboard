#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define RED_LED 15
#define GREEN_LED 2
#define BLUE_LED 5
#define LDRPIN 34

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

DHT dht(DHTPIN, DHTTYPE);
AsyncWebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000); // IST offset

String getWeatherCondition(float hum, int light) {
  if (light > 700 && hum < 50) return "Sunny";
  else if (light > 300 && hum < 70) return "Cloudy";
  else return "Rainy";
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(1000);
  Serial.println("WiFi connected");
  timeClient.begin();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    int light = analogRead(LDRPIN);
    timeClient.update();
    String currentTime = timeClient.getFormattedTime();
    String weather = getWeatherCondition(hum, light);

    digitalWrite(RED_LED, weather == "Sunny");
    digitalWrite(GREEN_LED, weather == "Cloudy");
    digitalWrite(BLUE_LED, weather == "Rainy");

    String html = R"rawliteral(
      <!DOCTYPE html><html><head><title>Weather Monitor</title>
      <style>body{font-family:Arial;text-align:center;background:#f0f0f0;}
      .card{background:white;padding:20px;margin:20px auto;width:300px;
      border-radius:10px;box-shadow:0 0 10px #aaa;}</style></head><body>
      <h1>🌤️ Weather Dashboard</h1><div class="card">
      <p><strong>Date & Time:</strong> %TIME%</p>
      <p><strong>Temperature:</strong> %TEMP% °C</p>
      <p><strong>Humidity:</strong> %HUM% %</p>
      <p><strong>Light Level:</strong> %LIGHT%</p>
      <p><strong>Condition:</strong> %WEATHER%</p></div></body></html>
    )rawliteral";

    html.replace("%TIME%", currentTime);
    html.replace("%TEMP%", String(temp));
    html.replace("%HUM%", String(hum));
    html.replace("%LIGHT%", String(light));
    html.replace("%WEATHER%", weather);
    request->send(200, "text/html", html);
  });

  server.begin();
}

void loop() {
  // Nothing needed here — everything runs in server callbacks
}
