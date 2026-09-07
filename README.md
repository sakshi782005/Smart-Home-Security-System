# Smart Home Security System 🔐

An ESP32-based Smart Home Security System developed using Arduino and simulated using Wokwi.

## Features

- PIR Motion Detection
- Door Open/Close Detection
- Keypad-Based Password Authentication
- 16x2 I2C LCD Display
- Buzzer Alarm
- LED Security Indicator
- Servo Motor Controlled Door Lock
- System Arm and Disarm Functionality

## Components Used

- ESP32 Development Board
- PIR Motion Sensor
- 16x2 I2C LCD
- 4x4 Membrane Keypad
- Servo Motor
- Buzzer
- Red LED
- Push Button
- 1kΩ Resistor

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| PIR Sensor | GPIO 13 |
| Buzzer | GPIO 12 |
| LED | GPIO 2 |
| Door Sensor/Button | GPIO 14 |
| Servo Motor | GPIO 19 |
| LCD SDA | GPIO 21 |
| LCD SCL | GPIO 22 |

### Keypad Connections

| Keypad Pin | ESP32 Pin |
|---|---|
| R1 | GPIO 32 |
| R2 | GPIO 33 |
| R3 | GPIO 25 |
| R4 | GPIO 26 |
| C1 | GPIO 27 |
| C2 | GPIO 16 |
| C3 | GPIO 17 |
| C4 | GPIO 18 |

## Working

1. The system starts in armed mode.
2. The PIR sensor detects motion.
3. If motion is detected, the buzzer and LED are activated.
4. The door sensor detects whether the door is opened or closed.
5. The user can enter a password using the keypad.
6. The correct PIN unlocks the door using the servo motor.
7. The system is then disarmed.

## Default Password

```text
1234
