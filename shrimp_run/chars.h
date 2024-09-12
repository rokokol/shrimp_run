#include <LiquidCrystal_I2C.h>

// Chars
byte shrimp_1[] = {
  B00000,
  B00000,
  B00010,
  B00001,
  B11111,
  B01010,
  B00000,
  B00000
};

byte shrimp_2[] = {
  B00000,
  B00000,
  B00010,
  B00001,
  B11111,
  B10101,
  B00000,
  B00000
};

byte seaweed_1[] = {
  B01000,
  B00001,
  B01010,
  B10111,
  B01110,
  B11110,
  B11111,
  B11111
};

byte seaweed_2[] = {
  B00000,
  B11000,
  B01010,
  B11101,
  B01110,
  B11110,
  B11111,
  B11111
};

byte shrimp_dead[] = {
  B00000,
  B00000,
  B00000,
  B01010,
  B11111,
  B00001,
  B00010,
  B00000
};

void init_chars(LiquidCrystal_I2C lcd) {
  lcd.createChar(0, shrimp_1);
  lcd.createChar(1, shrimp_2);
  lcd.createChar(2, seaweed_1);
  lcd.createChar(3, seaweed_2);
  lcd.createChar(4, shrimp_dead);
}
