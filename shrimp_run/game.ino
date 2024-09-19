void game_over_animation() {
  screen.setCursor(UPPER_MSG_INDENT, 0);
  if (blinks % 2 == 1) {
    screen.print("GAME");
    screen.setCursor(UPPER_MSG_INDENT + 6, 0);
    screen.print("OVER");
    play_sound(BLINK_SOUND_A, LOSE_SOUND_DURATION);
  } else {
    write_waves(UPPER_MSG_INDENT);
    play_sound(BLINK_SOUND_B, LOSE_SOUND_DURATION);
  }

  toggle_backlight();
  last_time = millis();
  blinks += 1;
}

void game_over_tip() {
  screen.setCursor(0, 3);
  screen.print("2x");
  screen.write(GROUND);
  screen.print("STICK");
  screen.write(GROUND);
  screen.print("FOR");
  screen.write(GROUND);
  screen.print("RESTART");
  stop_flag = true;
  play_sound(BLINK_SOUND_C, LOSE_SOUND_DURATION * 2);
}

void end_game_msg() {
  end_game_flag = true;
  screen.setCursor(UPPER_MSG_INDENT, 0);
  screen.print("YOU");
  screen.setCursor(UPPER_MSG_INDENT + 4, 0);
  screen.print("DID");
  screen.setCursor(UPPER_MSG_INDENT + 8, 0);
  screen.print("IT");
  delay(2000);  // Specially for several jokes
  play_sound(DEAD_SOUND, LOSE_SOUND_DURATION * 5);
  screen.setCursor(0, 3);
  screen.print("NOW");
  screen.setCursor(4, 3);
  screen.print("GO");
  screen.setCursor(7, 3);
  screen.print("OUTSIDE");
  screen.setCursor(16, 3);
  screen.print("BRUH");
  delay(4000);  // I hope you will see it without cheating...
  easter_egg();
}

void update_ticks() {

  ticks += 1;
  if (speed > MAX_SPEED) {
    speed -= SPEED_STEP;
  }

  if (end_game) {
    return;
  }

  screen.home();
  ulong delta = millis() - start_time;
  int score = (delta) / 1000 + bonus_score;

  if (score > 999) {
    screen.write(243);  // Infinity symbol of people who lasted 17 minutes
    write_waves(1);
    end_game = true;
    play_sound(END_GAME_SOUND, END_GAME_SOUND_DURATION);
    delay(END_GAME_SOUND_DURATION);
  } else {
    screen.print(score);
  }
}

void restart() {
  play_sound(START_SOUND, START_SOUND_DURATION);

  screen.clear();
  reset_types();

  write_waves(0);
  write_ground();
  hero[0] = FLOOR;
  randomSeed(analogRead(A2));

  last_time = 0;
  start_time = millis();
  spawn_seaweed_tick = 0;
  spawn_fish_tick = FISH_START;
  ticks = 0;
  jump_tick = 0;
  jump_type = 0;
  blinks = 0;
  speed = initial_speed;
  game_over = false;
  end_game = false;
  stop_flag = false;
  end_game_flag = false;
  prev_speed = 0;
  speed = initial_speed;
  bonus_score = 0;
}
