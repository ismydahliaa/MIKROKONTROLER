#include <WiFi.h>
#include <PubSubClient.h>

// ==========================
// GANTI SESUAI KONFIGURASI MU
// ==========================
const char* ssid = "ANNA";
const char* password = "annaramadhani2105";

const char* mqtt_server = "test.mosquitto.org"; // broker publik, bisa diganti sesuai app MQTT kamu
const int mqtt_port = 1883;

// ==========================
// PIN SETUP
// ==========================
int ledPin = 2;
int motor1Pin1 = 27;
int motor1Pin2 = 26;
int enable1Pin = 12;

// PWM setup
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;

// ==========================
// MQTT CLIENT
// ==========================
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// ==========================
// MOTOR CONTROL FUNCTIONS
// ==========================
void MotorOn() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, dutyCycle);
  Serial.println("Motor ON");
}

void MotorOff() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  ledcWrite(pwmChannel, 0);
  Serial.println("Motor OFF");
}

// ==========================
// MQTT CALLBACK
// ==========================
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Pesan diterima di topic: ");
  Serial.println(topic);
  Serial.print("Pesan: ");
  String messageTemp;

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  // Jika menerima pesan dari topic "esp32/motor"
  if (String(topic) == "esp32/motor") {
    if (messageTemp == "1") {
      digitalWrite(ledPin, HIGH);
      MotorOn();
    } else if (messageTemp == "0") {
      digitalWrite(ledPin, LOW);
      MotorOff();
    }
  }
}

// ==========================
// RECONNECT MQTT
// ==========================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Menghubungkan ke MQTT...");
    if (client.connect("ESP32MotorClient")) {
      Serial.println("terhubung!");
      client.subscribe("esp32/motor"); // pastikan sama dengan topic di aplikasi
    } else {
      Serial.print("gagal, rc=");
      Serial.print(client.state());
      Serial.println(" coba lagi dalam 5 detik...");
      delay(5000);
    }
  }
}

// ==========================
// SETUP
// ==========================
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // PWM setup
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(enable1Pin, pwmChannel);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// ==========================
// LOOP
// ==========================
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
