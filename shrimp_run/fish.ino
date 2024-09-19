void update_fish() {
  for (int i = 0; i < FISHES_MAX_COUNT; i++) {
    if (fish[i] < -2) {
      spawn_fish(i);
    } else {
      move_fish(i);
      if (game_over) {
        prev_speed = speed;
        speed = initial_speed;
        return;
      }
    }
  }
}

void spawn_fish(int i) {
  int shift = random(0, 2);

  bool cond = !random(0, SPAWN_FISH_CHANCE) && ticks - spawn_fish_tick >= SPAWN_FISH_DURATION;
  if (cond && allow_fish) {  // To exclude an impossible obstacle
    fish[i] = START_SCREEN - shift;
    Serial.print("Fish ");
    Serial.println(shift);

    spawn_fish_tick = ticks;
  }
}

void move_fish(int i) {
  screen.setCursor(fish[i], JUMP);
  bool lose_cond = fish[i] == HERO_POS_X || (fish[i] == HERO_POS_X - 1 && prev_state == JUMP);

  if (hero[0] == JUMP && lose_cond) {
    game_over = true;
    screen.setCursor(fish[i] + 1, JUMP);  // Fish will be dead too
  } else if (fish[i] >= 0) {
    screen.write(FISH);
  }

  if (fish[i] < START_SCREEN - 1) {
    screen.print("  ");
  }

  fish[i] -= 2;
}
