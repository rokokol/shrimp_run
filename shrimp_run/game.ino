#include "easter_egg.h"

void game_over_animation() {
  screen.setCursor(UPPER_MSG_INDENT, 0);
  if (blinks % 2 == 1) {
    screen.print("GAME");
    screen.setCursor(UPPER_MSG_INDENT + 6, 0);
    screen.print("OVER");
    tone(BUZZER_PIN, BLINK_SOUND_A, LOSE_SOUND_DURATION);
  } else {
    write_waves(UPPER_MSG_INDENT);
    tone(BUZZER_PIN, BLINK_SOUND_B, LOSE_SOUND_DURATION);
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
  tone(BUZZER_PIN, BLINK_SOUND_C, LOSE_SOUND_DURATION * 2);
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
  tone(BUZZER_PIN, DEAD_SOUND, LOSE_SOUND_DURATION * 5);
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
  int score = (delta) / 1000;

  if (score > 999) {
    screen.write(243);  // Infinity symbol of people who lasted 17 minutes
    write_waves(1);
    end_game = true;
    tone(BUZZER_PIN, END_GAME_SOUND, END_GAME_SOUND_DURATION);
    delay(END_GAME_SOUND_DURATION);
  } else {
    screen.print(score);
  }
}
