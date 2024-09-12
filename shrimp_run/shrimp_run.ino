#include <OneButton.h>
#include <GyverJoy.h>
#include "chars.h"

#define ulong unsigned long

// Constants
#define BUTTON_PIN 2
#define JY_PIN A1
#define INITIAL_SPEED 500
#define SPEED_STEP 1
#define MAX_SPEED 80
#define HERO_POS 4
#define ONE_CHANCE 6     // The higher, the lower the chance of double and triple
#define DOUBLE_CHANCE 9  // The higher, the lower the chance of triple
#define SPAWN_DURATION 4
#define SPAWN_CHANCE 4
#define FLOOR 1
#define JUMP 0
#define JUMP_TYPE 1
#define BIG_JUMP_TYPE 2
#define JUMP_DURATION 1
#define JUMP_COEF 2         // How much JUMP_DURATIONs per one jump
#define BIG_JUMP_COEF 3     // How much JUMP_DURATIONs per one big jump
#define BIG_JUMP_START 254  // Joystick position for double jump
#define BLINKS_COUNT 6      // Blinks count when game over

// Sprites
#define SHRIMP_1 0
#define SHRIMP_2 1
#define SEAWEED_1 2
#define SEAWEED_2 3
#define SHRIMP_DEAD 4

// Classes
GyverJoy jy(JY_PIN);
OneButton butt(BUTTON_PIN, true, true);
LiquidCrystal_I2C screen(0x27, 16, 2);


// Arrays
int entities[5][2];
int hero[] = { FLOOR, 0 };

// Variables
ulong last_time = 0;
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
  jy.calibrate();
  jy.exponent(GJ_CUBIC);
  entities[0][0] = 15;
  entities[0][1] = 3;

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
    screen.setCursor(3, 0);
    if (blinks % 2 == 1) {
      screen.print("!!GAME OVER!!");
    } else {
      screen.print("             ");
    }
    toggle_backlight();
    last_time = millis();
    blinks += 1;
  } else if (game_over && delta >= speed && blinks >= BLINKS_COUNT) {
    screen.setCursor(0, 1);
    screen.print("DBL CLICK TO CNT");
  }
}

void restart() {
  screen.clear();
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 2; j++) {
      entities[i][j] = 0;
    }
  }

  hero[0] = FLOOR;
  randomSeed(analogRead(A2));

  last_time = 0;
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
  int score = ticks / (1000 / speed);

  if (score > 999) {
    screen.write(243);  // Infinity symbol of people who lasted 17 minutes
  } else {
    screen.print(score);
  }
}

void update_entities() {
  screen.setCursor(HERO_POS, (hero[0] + 1) % 2);
  screen.print(' ');

  for (int i = 0; i < 5; i++) {
    if (entities[i][0] == -1) {
      if (!random(0, SPAWN_CHANCE) && ticks - spawn_tick >= SPAWN_DURATION) {
        entities[i][0] = 15;
        int temp = random(10);
        if (temp < ONE_CHANCE) {
          entities[i][1] = 1;
        } else if (temp < DOUBLE_CHANCE) {
          entities[i][1] = 2;
        } else {
          entities[i][1] = 3;
        }

        spawn_tick = ticks;
      } else {
        continue;
      }
    }

    move_entity(i);
    if (game_over) {
      speed = INITIAL_SPEED;
      return;
    }
  }


  screen.setCursor(HERO_POS, hero[0]);
  if (ticks % 2) {  // Walking animation
    screen.write(SHRIMP_1);
  } else {
    screen.write(SHRIMP_2);
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

void move_entity(int i) {
  int* entity = entities[i];

  screen.setCursor(entity[0], FLOOR);
  for (int i = 0; i < entity[1]; i++) {
    if (hero[0] == FLOOR && entity[0] + i == HERO_POS) {
      game_over = true;
      screen.write(SHRIMP_DEAD);
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
