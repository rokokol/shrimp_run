void update_seaweed_spots() {
  for (int i = 0; i < SEAWEEDS_MAX_COUNT; i++) {
    if (seaweeds[i][0] < 0) {
      spawn_seaweed_spot(i);
    } else {
      move_seaweed_spot(i);
      if (game_over) {
        speed = INITIAL_SPEED;
        return;
      }
    }
  }
}

void spawn_seaweed_spot(int i) {
  if (!random(0, SPAWN_SEAWEED_CHANCE) && ticks - spawn_seaweed_tick >= SPAWN_SEAWEED_DURATION) {
    seaweeds[i][0] = START_SCREEN;
    int temp = random(10);
    if (temp < ONE_CHANCE) {
      seaweeds[i][1] = 1;
    } else if (temp < DOUBLE_CHANCE) {
      seaweeds[i][1] = 2;
    } else {
      seaweeds[i][1] = 3;
    }

    spawn_seaweed_tick = ticks;
  }
}

void move_seaweed_spot(int n) {
  int* seaweed = seaweeds[n];

  screen.setCursor(seaweed[0], FLOOR);
  for (int i = 0; i < seaweed[1] && seaweed[0] + i <= START_SCREEN; i++) {
    int seaweed_pos = seaweed[0] + i;
    if (hero[0] == FLOOR && seaweed_pos == HERO_POS_X) {
      game_over = true;
      return;
    } else {
      if (seaweed_pos == START_SCREEN - 8 || seaweed_pos == START_SCREEN - 7) {
        allow_fish = false;
      }

      screen.write(random(SEAWEED_1, SEAWEED_2 + 1));  // Seaweed animation
    }
  }
  screen.print(" ");

  if (seaweed[0] == 0 && seaweed[1] > 0) {
    seaweed[1] -= 1;
  } else {
    seaweed[0] -= 1;
  }
}

bool check_seaweed(int n) {
  for (int i = 0; i < SEAWEEDS_MAX_COUNT; i++) {
    int pos = seaweeds[i][0];
    for (int j = 0; j < 2 && pos + j <= n; j++) {
      if (n = pos + j) {
        return true;
      }
    }
  }

  return false;
}
