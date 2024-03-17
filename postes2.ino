#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#define BUTTON1_PIN D1
#define BUTTON2_PIN D2
#define BUTTON3_PIN D3
#define LED_PIN D8
#define BUZZER_PIN D6

int button1State;
int button2State;
int button3State;
int ledState = 0;
int buzzerState = 100;

int buttonPushCounter = 0;
int lastButtonState = 0;
boolean switchState;

const char* ssid = "Universitas Mulawarman";
const char* password = "";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  server.on("/", handle);
  server.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON3_PIN, INPUT_PULLUP);
}

void loop() {
  mainProgram();
  server.handleClient();
}

void handle() {
  String strLed = String(ledState);
  String strBuzzer = String(buzzerState);
  String response = "<html><body><h2>LED State: " + strLed + "</h2><br><h2>Buzzer State: " + strBuzzer + "</h2></body></html>";
  server.send(200, "text/html", response);
}

void mainProgram() {
  button1State = digitalRead(BUTTON1_PIN);
  button2State = digitalRead(BUTTON2_PIN);
  button3State = digitalRead(BUTTON3_PIN);
  delay(100);

  if (button3State != lastButtonState) {
    if (button3State == LOW) {
      buttonPushCounter++;
    }
    delay(50);
  }
  lastButtonState = button3State;

  if (buttonPushCounter % 2 == 0) {
    if (!button1State) {
      if (0 <= ledState <= 249) {
        ledState += 3;
      }
      if (100 <= buzzerState <= 2500) {
        buzzerState += 10;
      }
    }
    if (!button2State) {
      if (0 <= ledState <= 249) {
        ledState -= 3;
      }
      if (100 <= buzzerState <= 2500) {
        buzzerState -= 10;
      }
    }

    tone(BUZZER_PIN, buzzerState);
    analogWrite(LED_PIN, ledState);
  } else {
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }
}