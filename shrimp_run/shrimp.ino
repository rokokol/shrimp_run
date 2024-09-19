void update_hero() {
  int j_coef = jump_type == JUMP_TYPE ? JUMP_COEF : BIG_JUMP_COEF;
  prev_state = hero[0];

  if (jump_type != 0 && hero[0] == FLOOR) {
    hero[0] = JUMP;
    jump_type == JUMP_TYPE ? JUMP_TYPE : BIG_JUMP_TYPE;
    jump_tick = ticks;

    if (jump_type == JUMP_TYPE) {
      play_sound(JUMP_SOUND, JUMP_SOUND_DURATION);
    } else if (jump_type == BIG_JUMP_TYPE) {
      play_sound(BIG_JUMP_SOUND, JUMP_SOUND_DURATION);
    }
  } else if (hero[0] == JUMP && ticks - jump_tick >= JUMP_DURATION * j_coef) {
    hero[0] = FLOOR;
    jump_type = 0;
    play_sound(LAND_SOUND, JUMP_SOUND_DURATION);
  }
}

void fix_jump() {
  if (jump_type != BIG_JUMP_TYPE) {

    if (y >= BIG_JUMP_START) {
      jump_type = BIG_JUMP_TYPE;
    } else if (y > 0) {
      jump_type = JUMP_TYPE;
    }
  }
}
