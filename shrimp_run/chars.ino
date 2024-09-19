// Chars
int texture_shift = 0;

byte shrimp_1_sprite[] = {
  B00000,
  B00000,
  B00000,
  B00010,
  B00001,
  B01111,
  B01010,
  B00000
};

byte shrimp_2_sprite[] = {
  B00000,
  B00000,
  B00000,
  B00010,
  B00001,
  B01111,
  B10101,
  B00000
};

byte shrimp_dead_sprite[] = {
  B00100,
  B01110,
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B00000,
};

byte seaweed_1_sprite[] = {
  B01000,
  B00001,
  B01010,
  B10111,
  B01110,
  B11110,
  B11111,
  B11111
};

byte seaweed_2_sprite[] = {
  B00000,
  B11000,
  B01010,
  B11101,
  B01110,
  B11110,
  B11111,
  B11111
};

byte wave_sprite[] = {
  B00000,
  B00100,
  B11011,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte ground_sprite[] = {
  B00000,
  B11111,
  B01001,
  B00010,
  B10000,
  B00000,
  B00000,
  B00010
};

byte fish_sprite[] = {
  B00000,
  B00000,
  B00100,
  B01101,
  B11110,
  B01101,
  B00000,
  B00000
};

void init_chars(LiquidCrystal_I2C lcd) {
  lcd.createChar(0 + texture_shift, shrimp_1_sprite);
  lcd.createChar(1 + texture_shift, shrimp_2_sprite);
  lcd.createChar(2 + texture_shift, shrimp_dead_sprite);
  lcd.createChar(3 + texture_shift, seaweed_1_sprite);
  lcd.createChar(4 + texture_shift, seaweed_2_sprite);
  lcd.createChar(5 + texture_shift, wave_sprite);
  lcd.createChar(6 + texture_shift, ground_sprite);
  lcd.createChar(7 + texture_shift, fish_sprite);
}
