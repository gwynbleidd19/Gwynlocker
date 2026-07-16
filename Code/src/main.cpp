// ===== Libraries =====                   ===                               ====================

#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== Settings =====                    ===
#define SECRET_CODE "2203"
#define MAX_PIN_LENGTH 4
#define MAX_ATTEMPTS 3
#define BLOCK_TIME 10000

// === Buzzer ===
const int buzzerPin = A0;

// ===== Keyboard =====                     ===
const byte ROWS = 4;
const byte COLS = 4;

byte rowPins[ROWS] = {5, 4, 3, 2};
byte colPins[COLS] = {6, 7, 8, 9};

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ===== Display =====                       ===

LiquidCrystal_I2C lcd(0x27, 16, 2);

// ===== Shift register =====                ===

int dataPin  = 10;   // SER
int clockPin = 12;   // SRCLK
int latchPin = 11;   // RCLK

byte ledState = 0;

// ===== Variables =====                      ===

String inputCode = "";
String secretCode = SECRET_CODE;

bool unlocked = false;
int attempts = 0;
unsigned long lastAttemptTime = 0;
const int max_attempts = MAX_ATTEMPTS;
const int block_time = BLOCK_TIME;

// ===== Register management =====             ===

void setLed(int num, bool state) {
  if (num < 1 || num > 8) return;
  int bitIndex = num - 1;

  if (state) {
    ledState = ledState | (1 << bitIndex);
  } else {
    ledState = ledState & ~(1 << bitIndex);
  }

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, ledState);
  digitalWrite(latchPin, HIGH);
}

void setGreenLeds(bool state) {
  setLed(2, state);   // Q1
  setLed(3, state);   // Q2
  setLed(4, state);   // Q3
}

void setRedLeds(bool state) {
  setLed(5, state);   // Q4
  setLed(6, state);   // Q5
  setLed(7, state);   // Q6
}

void allOff() {
  for (int i = 1; i <= 8; i++) {
    setLed(i, LOW);
  }
}

// ===== Display =====                          ===    

void updateDisplay() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (unlocked) {
    lcd.print("UNLOCKED!");
    lcd.setCursor(0, 1);
    lcd.print("Come in dude");
    return;
  }

  if (attempts >= max_attempts) {
    lcd.print("BLOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Wait for 10 sec");
    return;
  }

  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  lcd.print("> ");

  for (size_t i = 0; i < inputCode.length(); i++) {
    lcd.print("*");
  }
}

void showMessage(String msg, bool success) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(success ? "ACCESS ALLOWED" : "ACCESS REJECTED");
  lcd.setCursor(0, 1);
  lcd.print(msg);
  delay(2000);
  updateDisplay();
}

 void beep(int freq, int duration){
    tone(buzzerPin, freq, duration);
    delay(duration);
    noTone(buzzerPin);
  }

// ===== Code check =====                         ===

void checkCode() {
  
  Serial.println("checkCode start");

  if (attempts >= max_attempts) {
    return;
    Serial.println("Blocked, exit");
  }

 Serial.print("inputCode: ");
 Serial.println(inputCode);
 Serial.print("Secret code: ");
 Serial.println(secretCode);

  if (inputCode == secretCode) {
    Serial.println("Correct!");
    unlocked = true;
    setRedLeds(LOW);
    setGreenLeds(HIGH);
    beep(1850, 600);
    showMessage("Welcome", true);
    attempts = 0;
  } else {
    Serial.println("INCORRECT");
    attempts++;
    setRedLeds(HIGH);
    setGreenLeds(LOW);
    beep(200, 600);
    showMessage("Try again", false);

    if (attempts >= max_attempts) {
      lastAttemptTime = millis();
      updateDisplay();
    }
  }

  inputCode = "";
  Serial.println("checkCode END");
}

// ===== Setup =====                                ===

void setup() {
  Serial.begin(9600);
   
  pinMode(buzzerPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  ledState = 0;
  allOff();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Gwynlocker");
  lcd.setCursor(0, 1);
  lcd.print("Loading...");
  delay(1000);
  updateDisplay();

  Serial.println("=== Gwynlocker ===");
  Serial.println("Enter PIN (4 nums, then #)");
}

// ===== LOOP =====                                  ===
void loop() {
  
  // ===== Global restart by means of * =====        ===
  char key = keypad.getKey();

  if (key == '*') {
    inputCode = "";
    unlocked = false;
    attempts = 0;
    setRedLeds(LOW);
    setGreenLeds(LOW);
    updateDisplay();
    Serial.println("Reset");
    return;
  }

  // ===== Block =====
  if (attempts >= max_attempts) {
    if (millis() - lastAttemptTime > block_time) {
      attempts = 0;
      setRedLeds(LOW);
      updateDisplay();
    }
    return;
  }

  // ===== Locker opened =====
  if (unlocked) {
    return;
  }

  // ===== If no press =====
  if (!key) return;

  Serial.print("Key: ");
  Serial.println(key);

  // ===== PIN Check-up =====
  if (key == '#') {
    checkCode();
    return;
  }

  // ===== Numbers input =====
  if (key >= '0' && key <= '9') {
    if (inputCode.length() < MAX_PIN_LENGTH) {
      inputCode += key;
      Serial.print("Input: ");
      Serial.println(inputCode);
      updateDisplay();
    }
  }
}  //                                                  ==================================