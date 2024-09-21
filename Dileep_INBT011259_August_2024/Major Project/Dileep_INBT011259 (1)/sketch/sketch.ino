#include <WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Pin Definitions
#define DHTPIN 4          // Pin connected to DHT22
#define LDRPIN 34         // Pin connected to LDR
#define DHTTYPE DHT22     // DHT22 sensor type

// Wi-Fi credentials
const char *ssid = "Wokwi-GUEST";  // Wi-Fi SSID
const char *password = "";         // No password for Wokwi

// ThingSpeak credentials
const char *apiKey = "D142SAK493JY04HM";  // ThingSpeak Write API Key
const unsigned long channelID = 2657644;   // ThingSpeak Channel ID

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to Wi-Fi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Reconnect to Wi-Fi if disconnected
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi lost, reconnecting...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nReconnected to Wi-Fi");
  }

  // Read DHT22 sensor data
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read LDR sensor data
  int ldrValue = analogRead(LDRPIN);

  // Check if DHT22 readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT22 sensor");
    return;
  }

  // Print readings to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Update ThingSpeak fields
  ThingSpeak.setField(1, temperature);  // Field 1: Temperature
  ThingSpeak.setField(2, humidity);     // Field 2: Humidity
  ThingSpeak.setField(3, ldrValue);     // Field 3: LDR

  // Write data to ThingSpeak
  int status = ThingSpeak.writeFields(channelID, apiKey);
  if (status == 200) {
    Serial.println("Data successfully sent to ThingSpeak");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP status code: ");
    Serial.println(status);
  }

  // Delay before the next update
  delay(20000);  // Wait 20 seconds between updates
}
