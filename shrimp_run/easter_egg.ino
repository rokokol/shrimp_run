// -------------------------------------------------
// Copyright (c) 2024 HiBit <https://www.hibit.dev>
// -------------------------------------------------

int subway_note = 0;
ulong subway_time = 0;

int melody_egg[] = {
  NOTE_A4, REST, NOTE_B4, REST, NOTE_C5, REST, NOTE_A4, REST,
  NOTE_D5, REST, NOTE_E5, REST, NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_D5, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, REST,
  NOTE_D5, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_G5, NOTE_B4, REST,
  NOTE_C5, REST, NOTE_B4, NOTE_A4, REST,

  NOTE_G4, NOTE_A4, NOTE_C5, NOTE_A4, NOTE_C5, NOTE_D5, REST,
  NOTE_B4, NOTE_A4, NOTE_G4, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_C5, REST,

  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST, NOTE_D5, REST, NOTE_E5, REST,
  NOTE_G5, NOTE_F5, NOTE_E5, REST,

  NOTE_C5, REST, NOTE_D5, REST, NOTE_G4, REST
};

int durations_egg[] = {
  8, 8, 8, 8, 8, 8, 8, 4,
  8, 8, 8, 8, 2, 2,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 4, 8, 8, 8, 8, 8, 1, 4,

  8, 8, 8, 8, 2, 8, 8,
  2, 8,

  8, 8, 8, 8, 2, 8, 8,
  2, 8, 8, 8, 8,

  8, 8, 8, 8, 2, 8, 8,
  4, 8, 3, 8, 8, 8, 8, 8, 1, 4,

  2, 6, 2, 6, 4, 4, 2, 6, 2, 3,
  8, 8, 8, 8,

  2, 6, 2, 6, 2, 1
};

int subway_melody[] = {
  NOTE_C4,
  REST,
  NOTE_G4,
  REST,
  NOTE_AS4,
  NOTE_C5,
  NOTE_AS4,
  REST,
  NOTE_F4,
  NOTE_DS4,
  REST,
  NOTE_C4,
  REST,
  NOTE_G4,
  REST,
  NOTE_AS4,
  NOTE_C5,
  NOTE_AS4,
  REST,
  NOTE_F4,
  NOTE_DS4,
  REST,
  NOTE_C4,
  REST,
  NOTE_G4,
  REST,
  NOTE_AS4,
  NOTE_C5,
  NOTE_AS4,
  REST,
  NOTE_F4,
  NOTE_DS4,
  REST,

  NOTE_C4,
  REST,
  NOTE_E4,
  REST,
  NOTE_G4,
  NOTE_A4,
  NOTE_AS4,
  NOTE_C5,
  REST,
  NOTE_C5,
  REST,
  NOTE_AS4,
  REST,
  NOTE_A4,
  REST,
  NOTE_AS4,
  REST,
  NOTE_AS4,
  NOTE_C5,
  REST,
  NOTE_AS4,
  NOTE_A4,
  REST,
  REST,
  NOTE_C5,
  REST,
  NOTE_AS4,
  REST,
  NOTE_A4,
  REST,
  NOTE_AS4,
  REST,
  NOTE_E5,
  REST,

  NOTE_C5,
  REST,
  NOTE_C5,
  REST,
  NOTE_AS4,
  REST,
  NOTE_A4,
  REST,
  NOTE_AS4,
  REST,
  NOTE_AS4,
  NOTE_C5,
  REST,
  NOTE_AS4,
  NOTE_A4,
  REST,
  REST,
  NOTE_C5,
  REST,
  NOTE_AS4,
  REST,
  NOTE_A4,
  REST,
  NOTE_AS4,
  REST,
  NOTE_E4,
  REST,
};

int subway_durations[] = {
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,
  4, 8, 4, 8, 4, 8, 8, 16, 8, 8, 16,

  4, 8, 4, 8, 4, 4, 4,
  8, 16, 8, 16, 8, 16, 8, 16,
  8, 16, 8, 8, 16, 8, 8, 16,
  4,
  8, 16, 8, 16, 8, 16, 8, 4, 8,
  4,

  8, 16, 8, 16, 8, 16, 8, 16,
  8, 16, 8, 8, 16, 8, 8, 16,
  4,
  8, 16, 8, 16, 8, 16, 8, 4, 8,
  1
};

void easter_egg() {
  int size = sizeof(durations_egg) / sizeof(int);

  for (int note = 0; true; note++) {
    int duration = 1000 / durations_egg[note % size];
    tone(BUZZER_PIN, melody_egg[note % size], duration);

    int pauseBetweenNotes = duration * 1.30;
    delay(pauseBetweenNotes);

    noTone(BUZZER_PIN);
  }
}

void subway_tick() {
  ulong delta = millis() - subway_time;
  int duration = 1000 / subway_durations[subway_note];
  if (delta >= duration * 1.2) {
    tone(BUZZER_PIN, subway_melody[subway_note], duration * 1.3);

    int size = sizeof(subway_durations) / sizeof(int);
    subway_note = (subway_note + 1) % size;
    subway_time = millis();
  }
}
