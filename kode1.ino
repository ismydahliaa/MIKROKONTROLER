int ledPin = 2;
char myData = 0;

int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 12;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

const byte pin_rpm = 13;
volatile int rev = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // Motor pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(pin_rpm, INPUT_PULLUP);

  // Setup PWM
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  Serial.println("Ready. Type 1 to ON, 0 to OFF.");
}

void MotorOn() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, dutyCycle); // motor berputar
}

void MotorOff() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, 0); // motor berhenti total
}

void loop() {
  if (Serial.available() > 0) {     // pastikan ada data
    myData = Serial.read();         // baca karakter
    if (myData == '1') {
      Serial.println("LED dan Motor ON");
      digitalWrite(ledPin, HIGH);
      MotorOn();
    } 
    else if (myData == '0') {
      Serial.println("LED dan Motor OFF");
      digitalWrite(ledPin, LOW);
      MotorOff();
    }
  }
}
