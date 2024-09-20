ulong last_loud_time = 0;
int direction = 0;
bool is_show_loud = false;

void show_loud() {
  screen.setCursor(START_SCREEN - MAX_LOUD_LEVEL + 1, 0);

  if (loud_level == 0) {
    screen.print("MUTE");
  } else {
    for (int i = MIN_LOUD_LEVEL; i < loud_level; i++) {
      screen.write(255);
    }
    for (int i = loud_level; i <= MAX_LOUD_LEVEL; i++) {
      screen.print(' ');
    }
  }

  is_show_loud = true;
}

void hide_loud() {
  write_waves(START_SCREEN - MAX_LOUD_LEVEL + 1);
  is_show_loud = false;
}

void set_loud() {
  int value = map(loud_level, MIN_LOUD_LEVEL, MAX_LOUD_LEVEL, 0, 255);
  analogWrite(BUZZER_VCC_PIN, value);
  show_loud();
}

void fix_loud() {
  if (direction == 0) {
    if (x > 0) {
      direction = 1;
    } else if (x < 0) {
      direction = -1;
    }
  }
}

void loud_tick() {
  ulong delta = millis() - last_loud_time;
  if (delta >= LOUD_TICK) {
    fix_loud();  // To be less annoying I put it on a level below than fix_jump()

    if (delta >= LOUD_TICK * SHOW_LOUD_TICKS && direction == 0 && is_show_loud) {
      hide_loud();
    } else if (direction != 0) {

      if (loud_level >= MAX_LOUD_LEVEL) {
        play_sound(CANNOT_CHANGE_LOUD_SOUND, CHANGE_LOUD_SOUND_DURATION);
      } else if (direction > 0) {
        play_sound(INCREASE_LOUD_SOUND, CHANGE_LOUD_SOUND_DURATION);
      } else {
        play_sound(DECREASE_LOUD_SOUND, CHANGE_LOUD_SOUND_DURATION);
      }

      loud_level = constrain(loud_level + direction, MIN_LOUD_LEVEL, MAX_LOUD_LEVEL);
      set_loud();

      direction = 0;
      last_loud_time = millis();
    }
  }
}
