#include <OneButton.h>
#include <GyverJoy.h>
#include "chars.h"

#define ulong unsigned long

// Constants
#define START_SCREEN 19
#define BUTTON_PIN 2
#define JY_PIN A1
#define INITIAL_SPEED 500
#define SPEED_STEP 1
#define MAX_SPEED 80
#define HERO_POS_X 4
#define ONE_CHANCE 6     // The higher, the lower the chance of double and triple
#define DOUBLE_CHANCE 9  // The higher, the lower the chance of triple
#define SPAWN_DURATION 4
#define SPAWN_CHANCE 4
#define FLOOR 2
#define JUMP 1
#define JUMP_TYPE 1
#define BIG_JUMP_TYPE 2
#define JUMP_DURATION 1
#define JUMP_COEF 2         // How much JUMP_DURATIONs per one jump
#define BIG_JUMP_COEF 3     // How much JUMP_DURATIONs per one big jump
#define BIG_JUMP_START 254  // Joystick position for double jump
#define BLINKS_COUNT 6      // Blinks count when game over
#define GAME_OVER_INDENT 5
#define FISH_START 40         // Ticks amount needed to spawn fishes
#define SEAWEEDS_MAX_COUNT 5  // Maximum number of seaweed spots on the screen
#define FISHES_MAX_COUNT 2    // Maximum number of fish on the screen

// Sprites
#define SHRIMP_1 0
#define SHRIMP_2 1
#define SHRIMP_DEAD 2
#define SEAWEED_1 3
#define SEAWEED_2 4
#define WAVE 5
#define GROUND 6
#define FISH 7

// Classes
GyverJoy jy(JY_PIN);
OneButton butt(BUTTON_PIN, true, true);
LiquidCrystal_I2C screen(0x27, 20, 4);


// Arrays
int seaweeds[SEAWEEDS_MAX_COUNT][2];
int fish[FISHES_MAX_COUNT];
int hero[] = { FLOOR, 0 };

// Variables
ulong last_time = 0;
ulong start_time = 0;
int ticks = 0;
int spawn_tick = 0;
int jump_tick = 0;
int jump_type = 0;
int blinks = 0;
int speed = INITIAL_SPEED;
bool game_over = false;
bool backlight = true;


// Main functions
void setup() {
  randomSeed(analogRead(A2));
  Serial.begin(9600);
  screen.init();
  screen.backlight();
  init_chars(screen);
  write_waves(0);
  write_ground();

  jy.calibrate();
  jy.exponent(GJ_CUBIC);
  seaweeds[0][0] = START_SCREEN;
  seaweeds[0][1] = 3;

  pinMode(4, INPUT_PULLUP);
  butt.attachClick(toggle_backlight);
  butt.attachDoubleClick(restart);
}

void loop() {
  butt.tick();
  game_tick();
}


// Game sessio functions
/**
* First if handles the game session
* Second if handles blinking of the screen when game over
* Third if handles game over after blinking 
*/
void game_tick() {
  ulong delta = millis() - last_time;
  if (!game_over) {
    fix_jump();

    if (delta < speed) return;
    update_hero();
    update_entities();
    update_ticks();

    last_time = millis();
  } else if (game_over && delta >= speed && blinks < BLINKS_COUNT) {
    screen.setCursor(GAME_OVER_INDENT, 0);
    if (blinks % 2 == 1) {
      screen.print("GAME");
      screen.setCursor(GAME_OVER_INDENT + 6, 0);
      screen.print("OVER");
    } else {
      write_waves(GAME_OVER_INDENT);
    }
    toggle_backlight();
    last_time = millis();
    blinks += 1;
  } else if (game_over && delta >= speed && blinks >= BLINKS_COUNT) {
    screen.setCursor(0, 3);
    screen.print("2x");
    screen.write(GROUND);
    screen.print("STICK");
    screen.write(GROUND);
    screen.print("FOR");
    screen.write(GROUND);
    screen.print("RESTART");
  }
}

void restart() {
  screen.clear();
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
      seaweeds[i][j] = 0;
    }
  }

  write_waves(0);
  write_ground();
  hero[0] = FLOOR;
  randomSeed(analogRead(A2));

  last_time = 0;
  start_time = millis();
  spawn_tick = 0;
  ticks = 0;
  jump_tick = 0;
  jump_type = 0;
  blinks = 0;
  speed = INITIAL_SPEED;
  game_over = false;
}

// Udpates
void update_ticks() {
  ticks += 1;
  if (speed > MAX_SPEED) {
    speed -= SPEED_STEP;
  }
  screen.home();
  int score = (millis() - start_time) / 1000;

  if (score > 999) {
    screen.write(243);  // Infinity symbol of people who lasted 17 minutes
  } else {
    screen.print(score);
  }
}

void update_entities() {
  int erase_pos = hero[0] == FLOOR ? JUMP : FLOOR;
  screen.setCursor(HERO_POS_X, erase_pos);
  screen.print(' ');

  if (ticks > FISH_START) update_fishes();
  update_seaweeds();

  screen.setCursor(HERO_POS_X, hero[0]);
  if (game_over) {
    screen.write(SHRIMP_DEAD);
  } else if (ticks % 2) {  // Walking animation
    screen.write(SHRIMP_1);
  } else {
    screen.write(SHRIMP_2);
  }
}

void update_fishes() {
  // TODO: fish logic
}

void update_seaweeds() {
  for (int i = 0; i < SEAWEEDS_MAX_COUNT; i++) {
    if (seaweeds[i][0] <= -1) {
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

void update_hero() {
  int j_coef = jump_type == JUMP_TYPE ? JUMP_COEF : BIG_JUMP_COEF;

  if (jump_type != 0 && hero[0] == FLOOR) {
    hero[0] = JUMP;
    jump_type = jump_type == JUMP_TYPE ? JUMP_TYPE : BIG_JUMP_TYPE;
    jump_tick = ticks;
  } else if (hero[0] == JUMP && ticks - jump_tick >= JUMP_DURATION * j_coef) {
    hero[0] = FLOOR;
    jump_type = 0;
  }
}

// Help functions
void toggle_backlight() {
  backlight = !backlight;
  if (backlight) {
    screen.backlight();
  } else {
    screen.noBacklight();
  }
}

void fix_jump() {
  if (jump_type != BIG_JUMP_TYPE && jy.tick()) {
    int j_val = -jy.value();

    if (j_val >= BIG_JUMP_START) {
      jump_type = BIG_JUMP_TYPE;
    } else if (j_val > 0) {
      jump_type = JUMP_TYPE;
    }
  }
}

void spawn_seaweed_spot(int i) {
  if (!random(0, SPAWN_CHANCE) && ticks - spawn_tick >= SPAWN_DURATION) {
    seaweeds[i][0] = START_SCREEN;
    int temp = random(10);
    if (temp < ONE_CHANCE) {
      seaweeds[i][1] = 1;
    } else if (temp < DOUBLE_CHANCE) {
      seaweeds[i][1] = 2;
    } else {
      seaweeds[i][1] = 3;
    }

    spawn_tick = ticks;
  }
}

void move_seaweed_spot(int i) {
  int* entity = seaweeds[i];

  screen.setCursor(entity[0], FLOOR);
  for (int i = 0; i < entity[1] && entity[0] + i <= START_SCREEN; i++) {
    if (hero[0] == FLOOR && entity[0] + i == HERO_POS_X) {
      game_over = true;
      return;
    } else {
      screen.write(random(SEAWEED_1, SEAWEED_2 + 1));  // Seaweed animation
    }
  }
  screen.print(" ");

  if (entity[0] == 0 && entity[1] > 0) {
    entity[1] -= 1;
  } else {
    entity[0] -= 1;
  }
}

void write_waves(int indent) {
  screen.setCursor(indent, 0);
  for (int i = indent; i <= START_SCREEN; i++) {
    screen.write(WAVE);
  }
}

void write_ground() {
  screen.setCursor(0, 3);
  for (int i = 0; i <= START_SCREEN; i++) {
    screen.write(GROUND);
  }
}
