#define BLYNK_TEMPLATE_ID "TMPL32029KVH6"
#define BLYNK_TEMPLATE_NAME "workwi"
#define BLYNK_AUTH_TOKEN "8-_B7LSE4y7RWb3kImCp6GPKVr_Yf0I7"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <AccelStepper.h>  // Library for controlling stepper motors

#define DHTPIN 15          // Pin for DHT22
#define DHTTYPE DHT22      // DHT22 sensor type
#define LED_PIN 2          // LED pin

// Define the pins for the stepper motor
#define IN1 18  // Pin 1 for stepper
#define IN2 19  // Pin 2 for stepper
#define IN3 21  // Pin 3 for stepper
#define IN4 22  // Pin 4 for stepper

DHT dht(DHTPIN, DHTTYPE);
AccelStepper stepper(AccelStepper::FULL4WIRE, IN1, IN3, IN2, IN4);  // Initialize stepper motor

char auth[] = BLYNK_AUTH_TOKEN; // Auth Token from Blynk
char ssid[] = "Wokwi-GUEST";    // Wokwi's WiFi network
char pass[] = "";               // No password for Wokwi-GUEST


bool automaticMode = true;
bool motorStatus = false;
bool ledStatus = false;

// Blynk virtual pins for control
#define V0 0   // Switch for Automatic/Manual mode
#define V1 1   // Switch to control LED in manual mode
#define V2 2   // Switch to control Motor in manual mode

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);  // Connect to WiFi and Blynk
  dht.begin();
  
  pinMode(LED_PIN, OUTPUT);
  stepper.setMaxSpeed(1000);   // Set max speed for the stepper
  stepper.setAcceleration(500);  // Set acceleration

  // Turn off motor and LED at startup
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  Blynk.run();
  
  if (automaticMode) {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" °C, Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // Check temperature and humidity thresholds
    if (temperature > 35 || temperature < 12 || humidity > 70 || humidity < 40) {
      digitalWrite(LED_PIN, HIGH);
      stepper.setSpeed(400);     // Set speed for the motor
      stepper.runSpeed();        // Run the motor
      motorStatus = true;
      ledStatus = true;
    } else {
      digitalWrite(LED_PIN, LOW);
      stepper.stop();            // Stop the motor
      motorStatus = false;
      ledStatus = false;
    }
    
    // Update the Blynk app with the current status
    Blynk.virtualWrite(V1, ledStatus);
    Blynk.virtualWrite(V2, motorStatus);
  }
}

// Blynk writes to Virtual Pin V0 to switch between Manual/Automatic mode
BLYNK_WRITE(V0) {
  int mode = param.asInt();
  automaticMode = (mode == 1); // 1 for automatic mode

  if (automaticMode) {
    Serial.println("Automatic mode enabled.");
    Blynk.setProperty(V1, "enabled", false); // Disable manual LED control
    Blynk.setProperty(V2, "enabled", false); // Disable manual Motor control
  } else {
    Serial.println("Manual mode enabled.");
    Blynk.setProperty(V1, "enabled", true);  // Enable manual LED control
    Blynk.setProperty(V2, "enabled", true);  // Enable manual Motor control
  }
}

// Manual control for LED (V1)
BLYNK_WRITE(V1) {
  if (!automaticMode) {
    int ledState = param.asInt();
    digitalWrite(LED_PIN, ledState);
    ledStatus = (ledState == 1);
  }
}

// Manual control for Motor (V2)
BLYNK_WRITE(V2) {
  if (!automaticMode) {
    int motorState = param.asInt();
    if (motorState == 1) {
      stepper.setSpeed(400);     // Set speed for the motor in manual mode
      stepper.runSpeed();        // Run the motor
    } else {
      stepper.stop();            // Stop the motor
    }
    motorStatus = (motorState == 1);
  }
}
