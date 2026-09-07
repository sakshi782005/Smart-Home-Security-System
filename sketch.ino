#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <ESP32Servo.h>
// ================= PIN DEFINITIONS =================

#define PIR_PIN 13
#define BUZZER_PIN 12
#define LED_PIN 2
#define DOOR_PIN 14
#define SERVO_PIN 19

// ================= LCD =================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ================= SERVO =================

Servo doorServo;

const int LOCKED_POSITION = 0;
const int UNLOCKED_POSITION = 90;

// ================= KEYPAD =================

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Keypad pins according to your circuit

byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 16, 17, 18};

Keypad keypad = Keypad(
  makeKeymap(keys),
  rowPins,
  colPins,
  ROWS,
  COLS
);

// ================= PASSWORD =================

// You can change this PIN later

String correctPIN = "1234";
String enteredPIN = "";

// ================= VARIABLES =================

bool systemArmed = true;
bool doorUnlocked = false;

int previousDoorStatus = HIGH;
int previousMotionStatus = LOW;

// ==================================================

void showMessage(String line1, String line2 = "") {

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print(line1);

  lcd.setCursor(0, 1);
  lcd.print(line2);
}

// ==================================================

void alarmON() {

  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 1000);
}

// ==================================================

void alarmOFF() {

  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
}

// ==================================================

void setup() {

  Serial.begin(115200);

  // Sensor pins

  pinMode(PIR_PIN, INPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  pinMode(LED_PIN, OUTPUT);

  pinMode(DOOR_PIN, INPUT_PULLUP);

  // Start with alarm OFF

  alarmOFF();

  // LCD Setup

  lcd.init();
  lcd.backlight();

  showMessage("Smart Home", "Security System");

  Serial.println("Smart Home Security System Started!");

  delay(2000);

  // Servo Setup

  doorServo.attach(SERVO_PIN);

  // Initially lock the door

  doorServo.write(LOCKED_POSITION);

  doorUnlocked = false;

  showMessage("System Armed", "Door Locked");

  Serial.println("System Armed");
  Serial.println("Door Locked");

  delay(2000);

  showMessage("Enter PIN:", "");
}

// ==================================================

void checkKeypad() {

  char key = keypad.getKey();

  if (key) {

    Serial.print("Key Pressed: ");
    Serial.println(key);

    // Number keys

    if (key >= '0' && key <= '9') {

      if (enteredPIN.length() < 4) {

        enteredPIN += key;

        // Show stars instead of PIN

        lcd.setCursor(0, 1);

        for (int i = 0; i < enteredPIN.length(); i++) {
          lcd.print("*");
        }
      }
    }

    // Clear PIN using *

    else if (key == '*') {

      enteredPIN = "";

      showMessage("Enter PIN:", "");

      Serial.println("PIN Cleared");
    }

    // Check PIN using #

    else if (key == '#') {

      verifyPIN();
    }
  }
}

// ==================================================

void verifyPIN() {

  if (enteredPIN == correctPIN) {

    Serial.println("Correct PIN");

    showMessage("Correct PIN!", "Door Unlocked");

    // Unlock servo

    doorServo.write(UNLOCKED_POSITION);

    doorUnlocked = true;

    systemArmed = false;

    alarmOFF();

    delay(2000);

    showMessage("Door Unlocked", "System Disarmed");
  }

  else {

    Serial.println("Wrong PIN!");

    showMessage("WRONG PIN!", "Try Again");

    alarmON();

    delay(1500);

    alarmOFF();

    showMessage("Enter PIN:", "");
  }

  // Clear entered PIN

  enteredPIN = "";
}

// ==================================================

void checkDoor() {

  int doorStatus = digitalRead(DOOR_PIN);

  // Door opened

  if (doorStatus == LOW &&
      previousDoorStatus == HIGH) {

    Serial.println("Door Opened!");

    if (systemArmed) {

      showMessage("WARNING!", "Door Opened!");

      alarmON();
    }
  }

  // Door closed

  if (doorStatus == HIGH &&
      previousDoorStatus == LOW) {

    Serial.println("Door Closed");

    if (systemArmed) {

      alarmOFF();

      showMessage("Door Closed", "System Armed");
    }
  }

  previousDoorStatus = doorStatus;
}

// ==================================================

void checkMotion() {

  int motion = digitalRead(PIR_PIN);

  // Motion detected

  if (motion == HIGH &&
      previousMotionStatus == LOW) {

    Serial.println("Motion Detected!");

    if (systemArmed) {

      showMessage("WARNING!", "Motion Detected!");

      alarmON();
    }
  }

  // Motion stopped

  if (motion == LOW &&
      previousMotionStatus == HIGH) {

    Serial.println("Motion Stopped");

    if (systemArmed) {

      alarmOFF();

      showMessage("System Armed", "Monitoring...");
    }
  }

  previousMotionStatus = motion;
}

// ==================================================

void loop() {

  // Check keypad continuously

  checkKeypad();

  // Check door sensor

  checkDoor();

  // Check PIR sensor

  checkMotion();

  delay(50);
}