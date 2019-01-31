//Libraries
#include <LiquidCrystal.h>

//Global Variables
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
unsigned long displayTime = 0;
bool disp = false;

void setup() {
  lcd.begin(16, 2);
}

void loop() {
  if(millis()-displayTime >= 1000){
    lcd.clear();
    if(disp){
      lcd.setCursor(0, 0);
      lcd.print("Tone Detected:");
      lcd.setCursor(0, 1);
      lcd.print("Doorbell");
    }
    else {
      lcd.setCursor(0, 0);
      lcd.print("Rrriiiiinggg");
      lcd.setCursor(0, 1);
      lcd.print("Rrriiiiinggg");
    }
    disp = !disp;
    displayTime = millis();
  }
}
