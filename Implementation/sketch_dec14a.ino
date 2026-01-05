#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "AIModel.h"   // Your AI model header

// -----------------------------
// DHT22 configuration
// -----------------------------
#define DHTPIN 4          // GPIO pin connected to DHT22 DATA (change if needed)
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// -----------------------------
// Wi-Fi configuration
// -----------------------------
const char* ssid     = "Bektash";
const char* password = "Ahmed1994";

// -----------------------------
// ThingSpeak configuration
// -----------------------------
// Make sure these match your channel settings
const char* tsServer = "https://api.thingspeak.com/update?api_key=ZSJE10I97BQUTJRX&field1=0";
String tsApiKey = "ZSJE10I97BQUTJRX";

// -----------------------------
// Model / labels
// -----------------------------
const char* labels[] = {"Comfortable", "Uncomfortable", "Warm"};  // Order must match model output

// How often to send data (ThingSpeak minimum is 15 seconds)
const unsigned long UPDATE_INTERVAL_MS = 20000;   // 20 seconds

unsigned long lastUpdateTime = 0;

// -----------------------------
// Helper: connect to Wi-Fi
// -----------------------------
void connectToWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  // Try to connect for up to 10 seconds
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nWiFi connection failed.");
  }
}

// -----------------------------
// Setup
// -----------------------------
void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Initializing DHT22...");
  dht.begin();

  // Wi-Fi
  connectToWiFi();

  Serial.println("AI-based Comfort Classifier + ThingSpeak Ready!");
}

// -----------------------------
// Loop
// -----------------------------
void loop() {
  unsigned long now = millis();

  // Only read sensor & upload every UPDATE_INTERVAL_MS
  if (now - lastUpdateTime < UPDATE_INTERVAL_MS) {
    return;
  }
  lastUpdateTime = now;

  // Ensure Wi-Fi is connected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected. Reconnecting...");
    connectToWiFi();
  }

  // -----------------------------
  // 1) Read sensor
  // -----------------------------
  float temp = dht.readTemperature();   // °C
  float hum  = dht.readHumidity();      // %

  if (isnan(temp) || isnan(hum)) {
    Serial.println("Sensor error! Failed to read from DHT22.");
    return;
  }

  // -----------------------------
  // 2) Run ML model
  // -----------------------------
  double input[2]  = { temp, hum };
  double output[3] = {0.0, 0.0, 0.0};

  // The model function from your .h file
  score(input, output);

  // Find the class with highest probability
  int maxIndex = 0;
  double maxVal = output[0];
  for (int i = 1; i < 3; i++) {
    if (output[i] > maxVal) {
      maxVal = output[i];
      maxIndex = i;
    }
  }

  const char* predictedLabel = labels[maxIndex];

  // -----------------------------
  // 3) Debug print to Serial
  // -----------------------------
  Serial.println("======================================");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  Serial.print("Humidity:    ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Predicted class: ");
  Serial.print(predictedLabel);
  Serial.print(" (index ");
  Serial.print(maxIndex);
  Serial.println(")");

  Serial.print("Probabilities: [");
  Serial.print(output[0], 4);
  Serial.print(", ");
  Serial.print(output[1], 4);
  Serial.print(", ");
  Serial.print(output[2], 4);
  Serial.println("]");

  // -----------------------------
  // 4) Send data to ThingSpeak
  // -----------------------------
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Map:
    // field1 = temperature
    // field2 = humidity
    // field3 = class index (0, 1, 2)
    // field4 = prob of "Comfortable"
    // field5 = prob of "Uncomfortable"
    // field6 = prob of "Warm"

    String url = String(tsServer) +
      "?api_key=" + tsApiKey +
      "&field1=" + String(temp, 2) +
      "&field2=" + String(hum, 2) +
      "&field3=" + String(maxIndex) +
      "&field4=" + String(output[0], 4) +
      "&field5=" + String(output[1], 4) +
      "&field6=" + String(output[2], 4);

    Serial.print("Request URL: ");
    Serial.println(url);

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.print("ThingSpeak response (entry ID or 0): ");
      Serial.println(payload);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
    }

    http.end();
  } else {
    Serial.println("WiFi disconnected, skipped ThingSpeak upload.");
  }

  // Nothing else to do; interval is controlled at top of loop
}
