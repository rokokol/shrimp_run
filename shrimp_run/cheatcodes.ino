ulong last_code_time = 0;
int actions[QUEUE_LEN];
bool j_was_not_touch = true;
bool activated = false;
bool speed_bost = false;
bool (*cheats_check[])() = {
  immortal_check,
  resurrect_check,
  speed_boost_check,
  ultra_speed_boost_check,
  subway_check,
  shift_sprites_check,
  bonus_score_check,
  clear_check
};

byte immortal_code[] = {
  UP, UP,
  DOWN, DOWN,
  LEFT,
  RIGHT,
  LEFT,
  RIGHT
};

byte resurrect_code[] = {
  RIGHT, LEFT,
  UP, UP, UP
};

byte speed_boost_code[] = {
  RIGHT, RIGHT, RIGHT, RIGHT,
  DOWN
};

byte ultra_speed_boost_code[] = {
  DOWN,
  RIGHT, RIGHT,
  RIGHT, RIGHT,
  RIGHT, RIGHT,
  RIGHT
};

byte subway_code[] = {
  RIGHT, RIGHT,
  LEFT, LEFT, LEFT,
  UP
};

byte shift_sprites_code[] = {
  UP, RIGHT, DOWN, LEFT,
  LEFT, DOWN, RIGHT, UP
};

byte bonus_score_code[] = {
  UP, RIGHT, DOWN, LEFT,
  LEFT, RIGHT, UP, UP
};

byte clear_code[] = {
  LEFT,
  DOWN, DOWN,
  LEFT,
  UP
};

byte* codes[] = {
  immortal_code,
  resurrect_code,
  speed_boost_code,
  ultra_speed_boost_code,
  subway_code,
  shift_sprites_code,
  bonus_score_code,
  clear_code
};

size_t sizes[] = {
  sizeof(immortal_code),
  sizeof(resurrect_code),
  sizeof(speed_boost_code),
  sizeof(ultra_speed_boost_code),
  sizeof(subway_code),
  sizeof(shift_sprites_code),
  sizeof(bonus_score_code),
  sizeof(clear_code)
};

void cheatcode_tick() {
  ulong delta = millis() - last_code_time;
  update_queue();

  if (delta > CHEATCODE_TICK) {

    if (activated) {
      activated = false;
      toggle_backlight();
    }

    int cheats_count = sizeof(cheats_check) / sizeof(cheats_check[0]);
    for (int i = 0; i < cheats_count; i++) {
      bool res = check_code(codes[i], sizes[i], cheats_check[i]);

      if (res) {
        play_sound(CHEATCODE_SOUND, CHEATCODE_SOUND_DURATION);
        toggle_backlight();
        activated = true;
      }
    }

    last_code_time = millis();
  }
}

void update_queue() {
  if (abs(x) < 100 && abs(y) < 100) {
    j_was_not_touch = true;
  }

  if (!j_was_not_touch) {
    return;
  }

  int action;
  if (y > 250) {
    action = UP;
    Serial.println("UP");
  } else if (y < -250) {
    action = DOWN;
    Serial.println("DOWN");
  } else if (x > 250) {
    action = RIGHT;
    Serial.println("RIGHT");
  } else if (x < -250) {
    action = LEFT;
    Serial.println("LEFT");
  } else {
    action = NONE;
  }

  if (action != NONE) {
    j_was_not_touch = false;
    for (int i = 0; i < QUEUE_LEN - 1; i++) {
      actions[i] = actions[i + 1];
    }

    actions[QUEUE_LEN - 1] = action;
  }
}

bool check_code(byte* code, size_t code_size, bool (*function)()) {
  for (int i = 0; i < code_size; i++) {
    int j = i + QUEUE_LEN - code_size;
    if (code[i] != actions[j]) {
      return false;
    }
  }

  actions[QUEUE_LEN - 1] = NONE;

  return function();
}

bool immortal_check() {
  immortal = !immortal;

  actions[QUEUE_LEN - 1] = NONE;
  return true;
}

bool resurrect_check() {
  if (game_over) {
    game_over = false;
    speed = prev_speed;
    Serial.println(speed);

    return true;
  }

  return false;
}

bool speed_boost_check() {
  if (speed_bost) {
    initial_speed = INITIAL_SPEED;
    speed_bost = false;
  } else {
    initial_speed = MAX_SPEED;
    speed = MAX_SPEED;
    speed_bost = true;
  }

  return true;
}

bool ultra_speed_boost_check() {
  if (speed_bost) {
    initial_speed = INITIAL_SPEED;
    speed_bost = false;
  } else {
    initial_speed = ULTRA_SPEED;
    speed = ULTRA_SPEED;
    speed_bost = true;
  }

  return true;
}

bool subway_check() {
  subway = !subway;

  return true;
}

bool shift_sprites_check() {
  texture_shift += 1;
  init_chars(screen);

  return true;
}

bool bonus_score_check() {
  bonus_score += 100;

  return true;
}

bool clear_check() {
  reset_types();
  screen.setCursor(0, 1);
  clear_line();
  screen.setCursor(0, 2);
  clear_line();
  return true;
}

void clear_line() {
  for (int i = 0; i <= START_SCREEN; i++) {
    screen.print(' ');
  }
}
