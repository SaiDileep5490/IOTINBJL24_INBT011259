const int gasSensorPin =A0; // declaring gasPin
const int ledPin1 = 3; // declaring led pin
const int ledPin2 = 4;   // declaring led pin
const int buzzerPin=5;  // declaring buzzer  pin
const int gasThreshold = 150; // declaring threshold value
int gasValue;
int delayValue = 100;  // declaring delay time

void setup(){
  pinMode(4,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(A0,INPUT);
  
Serial.begin(9600);
  
}

void loop(){
  gasValue = analogRead(gasSensorPin); // Reads the value from the sensor
  Serial.print("Gas Sensor Value: ");
  Serial.println(gasValue);
  
  if (gasValue > gasThreshold) {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(buzzerPin, HIGH);
    Serial.println("SMOKE DETECTED");
  }
  else{
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(buzzerPin, LOW);
    Serial.println("SMOKE  NOT DETECTED");
  }
  delay(delayValue); // Adding delay to avoid any fluctations
  
}
    
  
  