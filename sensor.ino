#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h> // Library for the Web Server
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

const char* ssid     = "HOTSPOT@UPNJATIM.AC.ID";
const char* password = "belanegara";

// Hardware Pins
#define DHTPIN 0      // D3
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Initialize the web server on port 80
ESP8266WebServer server(80);

void handleRoot() {
  digitalWrite(LED_BUILTIN, LOW); // Blink LED when someone visits the page
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

// Create a better HTML page
String html = "<!DOCTYPE html><html lang='en'>";
html += "<head>";
html += "<meta http-equiv='refresh' content='5'>";
html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
html += "<title>ESP8266 Weather Station</title>";

// CSS Styling
html += "<style>";
html += "body { margin:0; font-family: 'Segoe UI', Arial; background: linear-gradient(to right, #CCA5F0, #EFF0A5); text-align:center; color:#333; }";
html += ".container { padding: 30px; }";
html += "h1 { margin-bottom: 30px; color: #574747;}";
html += "h3 { margin-bottom: 25px; color: #FFFFFF;}";
html += ".card { background: white; border-radius: 15px; padding: 20px; margin: 15px auto; width: 250px; box-shadow: 0 4px 10px rgba(0,0,0,0.2); }";
html += ".title { font-size: 18px; color: #574747; }";
html += ".value { font-size: 32px; font-weight: bold; margin-top: 10px; }";
html += "</style>";

html += "</head><body>";

html += "<div class='container'>";
html += "<h1>ESP8266 Weather Station</h1>";
html += "<h3>Kelompok 4</h3>";


// Temperature Card
html += "<div class='card'>";
html += "<div class='title'>Suhu Ruangan</div>";
html += "<div class='value'>" + String(t) + " C</div>";
html += "</div>";

// Humidity Card
html += "<div class='card'>";
html += "<div class='title'>Tingkat Kelembapan</div>";
html += "<div class='value'>" + String(h) + " %</div>";
html += "</div>";

html += "</div>";

html += "</body></html>";

server.send(200, "text/html", html);
  
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

void handleRootJSON() {
  // Blink LED to indicate data transmission/request
  digitalWrite(LED_BUILTIN, LOW); 
  
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Create a JSON string (Machine-friendly)
  // Format: {"temperature": 25.5, "humidity": 60.0}
  String json = "{";
  json += "\"temperature\": " + String(t) + ",";
  json += "\"humidity\": " + String(h);
  json += "}";

  // Send with "application/json" header instead of "text/html"
  server.send(200, "application/json", json);
  
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  // Define the "Home" route of the web server
  server.on("/", handleRoot);
  server.begin();

  // Show the IP address on the OLED (You need this to access the data!)
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("IP Address:");
  display.setTextSize(2);
  display.setCursor(0,20);
  display.println(WiFi.localIP());
  display.display();
}

void loop() {
  server.handleClient(); // Listen for web browsers
}