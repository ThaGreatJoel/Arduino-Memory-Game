#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int buttonPin[] = {2, 3, 4, 5};
const char* buttonLabels[] = {"RED", "GREEN", "BLUE", "YELLOW"};
const int buzzerPin = 6;

int sequence[100];
int level = 1;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
  }
  pinMode(buzzerPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Simon Says Game");
  delay(2000);
  
  randomSeed(analogRead(0));
}

void loop() {
  lcd.clear();
  lcd.print("Level ");
  lcd.print(level);
  delay(1000);

  generateSequence();
  showSequence();

  if (getPlayerInput()) {
    level++;
    tone(buzzerPin, 1000, 300);
  } else {
    lcd.clear();
    lcd.print("Wrong! Score:");
    lcd.setCursor(0, 1);
    lcd.print("Level ");
    lcd.print(level - 1);
    tone(buzzerPin, 200, 800);
    delay(3000);
    level = 1;
  }
}

void generateSequence() {
  for (int i = 0; i < level; i++) {
    sequence[i] = random(0, 4);
  }
}

void showSequence() {
  lcd.clear();
  lcd.print("Watch!");
  delay(1000);
  for (int i = 0; i < level; i++) {
    lcd.setCursor(0, 1);
    lcd.print(buttonLabels[sequence[i]]);
    delay(800);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(300);
  }
}

bool getPlayerInput() {
  lcd.clear();
  lcd.print("Your Turn!");
  for (int i = 0; i < level; i++) {
    int input = waitForButton();
    if (input != sequence[i]) {
      return false;
    }
  }
  return true;
}

int waitForButton() {
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPin[i]) == LOW) {
        tone(buzzerPin, 800, 100);
        lcd.setCursor(0, 1);
        lcd.print(buttonLabels[i]);
        delay(500);
        while (digitalRead(buttonPin[i]) == LOW); // Wait for release
        return i;
      }
    }
  }
}
