void update_hero() {
  int j_coef = jump_type == JUMP_TYPE ? JUMP_COEF : BIG_JUMP_COEF;
  prev_state = hero[0];

  if (jump_type != 0 && hero[0] == FLOOR) {
    hero[0] = JUMP;
    jump_type = jump_type == JUMP_TYPE ? JUMP_TYPE : BIG_JUMP_TYPE;
    jump_tick = ticks;
  } else if (hero[0] == JUMP && ticks - jump_tick >= JUMP_DURATION * j_coef) {
    hero[0] = FLOOR;
    jump_type = 0;
  }
}
