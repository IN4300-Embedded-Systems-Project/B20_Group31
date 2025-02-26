#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "SLT-LTE-WiFi-1";
const char* password = "Malinsha99";

// Pin definitions
const int ldrPin = A0;      // Connect LDR to analog pin A0
const int ledPin = 4;      // Connect LED/Bulb to digital pin D2

// Web server on port 80
ESP8266WebServer server(80);

// Variables to store sensor values and states
int ldrValue = 0;
bool ledState = false;
bool autoMode = true;             // Default to automatic mode
int ldrThreshold = 500;           // LDR threshold value (adjust based on your environment)
bool manualOverride = false;      // Flag for manual override

// Timer for updating sensor readings
unsigned long previousMillis = 0;
const long interval = 1000;  // Update interval in milliseconds
void setup() {
  Serial.begin(115200);
  delay(100);
  
  // Initialize pins
  pinMode(ldrPin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.print(ssid);
  
  // Wait for WiFi to connect
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 // Define server routes
  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/getdata", handleGetData);
  server.on("/setauto", handleSetAuto);
  server.on("/setthreshold", handleSetThreshold);
  server.onNotFound(handleNotFound);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle client requests
  server.handleClient();
  
  // Update sensor readings and control LED periodically
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read LDR value
    ldrValue = analogRead(ldrPin);
    Serial.print("LDR Value: ");
    Serial.println(ldrValue);
// Control LED based on LDR value if in auto mode
    if (autoMode && !manualOverride) {
      if (ldrValue < ldrThreshold) {
        // It's dark, turn on the light
        if (!ledState) {
          ledState = true;
          digitalWrite(ledPin, ledState);
          Serial.println("Auto: LED turned ON (low light detected)");
        }
      } else {
        // It's bright, turn off the light
        if (ledState) {
          ledState = false;
          digitalWrite(ledPin, ledState);
          Serial.println("Auto: LED turned OFF (bright light detected)");
        }
      }
    }
  }
}
void handleRoot() {
  String html = "<!DOCTYPE html>\n";
  html += "<html>\n";
  html += "<head>\n";
  html += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  html += "  <title>ESP8266 Control Panel</title>\n";
  html += "  <style>\n";
  html += "    body { font-family: Arial, sans-serif; text-align: center; margin: 0px auto; padding: 20px; }\n";
  html += "    .container { display: flex; flex-direction: column; width: 100%; max-width: 400px; margin: 0 auto; }\n";
  html += "    h1 { color: #0066cc; }\n";
  html += "    .card { background-color: #f8f9fa; box-shadow: 2px 2px 12px #aaa; padding: 20px; margin: 20px 0; border-radius: 5px; }\n";
  html += "    .reading { font-size: 24px; font-weight: bold; margin: 10px 0; }\n";
  html += "    #ldrValue { color: #3498db; }\n";
  html += "    #ledStatus, #modeStatus { font-weight: bold; }\n";
  html += "    .btn { background-color: #4CAF50; border: none; color: white; padding: 12px 20px; text-align: center; font-size: 16px; margin: 10px 5px; border-radius: 4px; }\n";
  html += "    .btn-on { background-color: #4CAF50; }\n";
  html += "    .btn-off { background-color: #f44336; }\n";
  html += "    .btn-auto { background-color: #2196F3; }\n";
  html += "    .btn-manual { background-color: #ff9800; }\n";
  html += "    .btn:hover { opacity: 0.8; cursor: pointer; }\n";
  html += "    .slider-container { width: 100%; margin: 20px 0; }\n";
  html += "    .slider { width: 80%; height: 25px; }\n";
  html += "    .threshold-value { font-weight: bold; margin: 10px 0; }\n";
  html += "  </style>\n";
  html += "</head>\n";
html += "<body>\n";
  html += "  <div class='container'>\n";
  html += "    <h1>IoT Based Smart Lighting System (Group 31) </h1>\n";
  
  html += "    <div class='card'>\n";
  html += "      <h2>Light Sensor</h2>\n";
  html += "      <p>Current light level:</p>\n";
  html += "      <div class='reading'>LDR Value: <span id='ldrValue'>";
  html += ldrValue;
  html += "</span></div>\n";
  html += "      <div class='threshold-value'>Threshold: <span id='thresholdValue'>";
  html += ldrThreshold;
  html += "</span></div>\n";
  html += "      <div class='slider-container'>\n";
  html += "        <input type='range' min='0' max='1023' value='";
  html += ldrThreshold;
  html += "' class='slider' id='thresholdSlider'>\n";
  html += "        <button class='btn' onclick='setThreshold()'>Set Threshold</button>\n";
  html += "      </div>\n";
  html += "    </div>\n";
html += "    <div class='card'>\n";
  html += "      <h2>LED Control</h2>\n";
  html += "      <p>Mode: <span id='modeStatus'>";
  html += autoMode ? "AUTOMATIC" : "MANUAL";
  html += "</span></p>\n";
  html += "      <button class='btn ";
  html += autoMode ? "btn-manual" : "btn-auto";
  html += "' onclick='toggleMode()'>";
  html += autoMode ? "Switch Control Mode" : "Switch Control Mode";
  html += "</button>\n";
  html += "      <p>LED is currently: <span id='ledStatus'>";
  html += ledState ? "ON" : "OFF";
  html += "</span></p>\n";
  html += "      <button class='btn ";
  html += ledState ? "btn-off" : "btn-on";
  html += "' onclick='toggleLED()'>";
  html += ledState ? "On / Off" : "On / Off";
  html += "</button>\n";
  html += "    </div>\n";
  
  html += "  </div>\n";
html += "  <script>\n";
  html += "    function toggleLED() {\n";
  html += "      var xhr = new XMLHttpRequest();\n";
  html += "      xhr.open('GET', '/toggle', true);\n";
  html += "      xhr.onreadystatechange = function() {\n";
  html += "        if (xhr.readyState == 4 && xhr.status == 200) {\n";
  html += "          updateStatus();\n";
  html += "        }\n";
  html += "      };\n";
  html += "      xhr.send();\n";
  html += "    }\n";
  
  html += "    function toggleMode() {\n";
  html += "      var xhr = new XMLHttpRequest();\n";
  html += "      xhr.open('GET', '/setauto', true);\n";
  html += "      xhr.onreadystatechange = function() {\n";
  html += "        if (xhr.readyState == 4 && xhr.status == 200) {\n";
  html += "          updateStatus();\n";
  html += "        }\n";
  html += "      };\n";
  html += "      xhr.send();\n";
  html += "    }\n";
  
html += "    function setThreshold() {\n";
  html += "      var threshold = document.getElementById('thresholdSlider').value;\n";
  html += "      var xhr = new XMLHttpRequest();\n";
  html += "      xhr.open('GET', '/setthreshold?value=' + threshold, true);\n";
  html += "      xhr.onreadystatechange = function() {\n";
  html += "        if (xhr.readyState == 4 && xhr.status == 200) {\n";
  html += "          document.getElementById('thresholdValue').innerHTML = threshold;\n";
  html += "        }\n";
  html += "      };\n";
  html += "      xhr.send();\n";
  html += "    }\n";
html += "    function updateStatus() {\n";
  html += "      var xhr = new XMLHttpRequest();\n";
  html += "      xhr.open('GET', '/getdata', true);\n";
  html += "      xhr.onreadystatechange = function() {\n";
  html += "        if (xhr.readyState == 4 && xhr.status == 200) {\n";
  html += "          var response = JSON.parse(xhr.responseText);\n";
  html += "          document.getElementById('ldrValue').innerHTML = response.ldr;\n";
  html += "          document.getElementById('ledStatus').innerHTML = response.ledState ? 'ON' : 'OFF';\n";
  html += "          document.getElementById('modeStatus').innerHTML = response.autoMode ? 'AUTOMATIC' : 'MANUAL';\n";
  html += "          document.getElementById('thresholdValue').innerHTML = response.threshold;\n";
  html += "          document.getElementById('thresholdSlider').value = response.threshold;\n";
  
  html += "          var ledBtn = document.querySelector('.btn:nth-of-type(3)');\n";
  html += "          if (response.ledState) {\n";
  html += "            ledBtn.innerHTML = 'On / Off';\n";
  html += "            ledBtn.className = 'btn btn-off';\n";
  html += "          } else {\n";
  html += "            ledBtn.innerHTML = 'On / Off';\n";
  html += "            ledBtn.className = 'btn btn-on';\n";
  html += "          }\n";
  
html += "          var modeBtn = document.querySelector('.btn:nth-of-type(1)');\n";
  html += "          if (response.autoMode) {\n";
  html += "            modeBtn.innerHTML = 'Switch Control Mode';\n";
  html += "            modeBtn.className = 'btn btn-manual';\n";
  html += "          } else {\n";
  html += "            modeBtn.innerHTML = 'Switch Control Mode';\n";
  html += "            modeBtn.className = 'btn btn-auto';\n";
  html += "          }\n";
  html += "        }\n";
  html += "      };\n";
  html += "      xhr.send();\n";
  html += "    }\n";
  
  html += "    // Update status every second\n";
  html += "    setInterval(updateStatus, 1000);\n";
  html += "  </script>\n";
  
  html += "</body>\n";
  html += "</html>\n";
  
 server.send(200, "text/html", html);
}

void handleToggle() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  
  // Set manual override flag when user toggles LED
  manualOverride = true;
  autoMode = false;  // Switch to manual mode when user toggles LED
  
  Serial.print("Manual Toggle: LED turned ");
  Serial.println(ledState ? "ON" : "OFF");
  
  String json = "{\"ledState\":";
  json += ledState ? "true" : "false";
  json += ",\"autoMode\":false}";
  
  server.send(200, "application/json", json);
}
void handleGetData() {
  String json = "{\"ldr\":";
  json += ldrValue;
  json += ",\"ledState\":";
  json += ledState ? "true" : "false";
  json += ",\"autoMode\":";
  json += autoMode ? "true" : "false";
  json += ",\"threshold\":";
  json += ldrThreshold;
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleSetAuto() {
  autoMode = !autoMode;
  if (autoMode) {
    manualOverride = false;  // Reset manual override when switching to auto mode
    Serial.println("Switched to AUTO mode");
  } else {
    manualOverride = true;   // Enable manual override when switching to manual mode
    Serial.println("Switched to MANUAL mode");
  }
String json = "{\"autoMode\":";
  json += autoMode ? "true" : "false";
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleSetThreshold() {
  if (server.hasArg("value")) {
    ldrThreshold = server.arg("value").toInt();
    Serial.print("Threshold set to: ");
    Serial.println(ldrThreshold);
  }
  
  server.send(200, "text/plain", "OK");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
 for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain",message);
}