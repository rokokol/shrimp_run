#include <OneButton.h>
#include <GyverJoy.h>
#include "chars.h"

#define ulong unsigned long

// Constants
#define START_SCREEN 19
#define BUTTON_PIN 2
#define JX_PIN A0
#define JY_PIN A1
#define BUZZER_VCC_PIN 9
#define BUZZER_PIN 4
#define SPEED_STEP 1  // How fast is the speed increasing
#define MAX_SPEED 80
#define BLINKS_COUNT 6  // Blinks count when game over
#define UPPER_MSG_INDENT 5
#define INITIAL_LOUD_LEVEL 3  // From MIN_LOUD_LEVEL to MAX_LOUD_LEVEL inclusive
#define MIN_LOUD_LEVEL 0
#define MAX_LOUD_LEVEL 4
#define LOUD_TICK 400
#define LOUD_STICK_DEADZONE 256
#define SHOW_LOUD_TICKS 4

#define HERO_POS_X 3
#define FLOOR 2
#define JUMP 1
#define JUMP_TYPE 1
#define BIG_JUMP_TYPE 2
#define JUMP_DURATION 1
#define JUMP_COEF 2         // How much JUMP_DURATIONs per one jump
#define BIG_JUMP_COEF 3     // How much JUMP_DURATIONs per one big jump
#define BIG_JUMP_START 254  // Joystick position for double jump

#define ONE_CHANCE 6              // The higher, the lower the chance of double and triple
#define DOUBLE_CHANCE 9           // The higher, the lower the chance of triple
#define SPAWN_SEAWEED_DURATION 4  // Min count of ticks to spawn a new seaweed spot
#define SPAWN_SEAWEED_CHANCE 4    // Chance to spawn is 1 / SPAWN_SEAWEED_CHANCE
#define SEAWEEDS_MAX_COUNT 5      // Maximum number of seaweed spots on the screen

#define SPAWN_FISH_DURATION 4  // Min count of ticks to spawn a new fish
#define SPAWN_FISH_CHANCE 2    // Chance to spawn is 1 / SPAWN_FISH_CHANCE
#define FISH_START 80          // Ticks amount needed to spawn fishes
#define FISHES_MAX_COUNT 2     // Maximum number of fish on the screen


// Sprites
#define SHRIMP_1 0
#define SHRIMP_2 1
#define SHRIMP_DEAD 2
#define SEAWEED_1 3
#define SEAWEED_2 4
#define WAVE 5
#define GROUND 6
#define FISH 7

// Music
#define STED_SOUND_DURAION 50
#define STEP_SOUND_A 440
#define STEP_SOUND_B 659
#define LOSE_SOUND_DURATION 200
#define DEAD_SOUND 146
#define BLINK_SOUND_A 293
#define BLINK_SOUND_B 349
#define BLINK_SOUND_C 698
#define JUMP_SOUND_DURATION 100
#define JUMP_SOUND 523
#define BIG_JUMP_SOUND 783
#define LAND_SOUND 1046
#define TOGGLE_SCREEN_SOUND_DURATION 30
#define SCREEN_ON_SOUND 262
#define SCREEN_OFF_SOUND 220
#define START_SOUND_DURATION 30
#define START_SOUND 493
#define END_GAME_SOUND_DURATION 400
#define END_GAME_SOUND 987
#define CHANGE_LOUD_SOUND_DURATION 60
#define INCREASE_LOUD_SOUND 392
#define DECREASE_LOUD_SOUND 330
#define CANNOT_CHANGE_LOUD_SOUND 164

// Classes
GyverJoy jx(JX_PIN);
GyverJoy jy(JY_PIN);
OneButton butt(BUTTON_PIN, true, true);
LiquidCrystal_I2C screen(0x27, START_SCREEN + 1, 4);


int hero[] = { FLOOR, 0 };
int seaweeds[SEAWEEDS_MAX_COUNT][2];
int fish[FISHES_MAX_COUNT];

// Variables
ulong last_time = 0;
ulong start_time = 0;
int initial_speed = 500;  // FPS = 1000 / SPEED
int spawn_seaweed_tick = 0;
int spawn_fish_tick = FISH_START;
int ticks = 0;
int jump_tick = 0;
int jump_type = 0;
int prev_state = 0;
int blinks = 0;
int speed = initial_speed;
int loud_level = INITIAL_LOUD_LEVEL;
bool allow_fish;
bool game_over = false;
bool backlight = true;
bool stop_flag = false;
bool end_game = false;
bool end_game_flag = false;


// Main functions
void setup() {
  set_PWM();
  Serial.begin(9600);

  randomSeed(analogRead(A2));
  screen.init();
  screen.backlight();
  init_chars(screen);
  reset_types();

  write_waves(0);
  write_ground();

  jy.calibrate();
  jy.exponent(GJ_CUBIC);
  jx.calibrate();
  jx.exponent(GJ_CUBIC);
  jx.deadzone(LOUD_STICK_DEADZONE);

  // seaweeds[0][0] = START_SCREEN - 8;
  // seaweeds[0][1] = 1;
  // fish[0] = START_SCREEN;

  pinMode(4, INPUT_PULLUP);
  butt.attachClick(toggle_backlight);
  butt.attachDoubleClick(restart);
  pinMode(BUZZER_VCC_PIN, OUTPUT);
  set_loud();
}

void loop() {
  butt.tick();
  loud_tick();
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
  if (!game_over && !end_game) {
    fix_jump();

    if (delta >= speed) {
      update_entities();
      update_ticks();

      last_time = millis();
    }
  } else if (game_over && delta >= speed) {
    if (blinks < BLINKS_COUNT) {
      game_over_animation();
    } else if (!stop_flag) {
      game_over_tip();
    }
  } else if (end_game && !end_game_flag) {
    end_game_msg();
  }
}


// Udpates
void update_entities() {
  allow_fish = true;

  update_hero();

  int erase_pos = hero[0] == FLOOR ? JUMP : FLOOR;
  screen.setCursor(HERO_POS_X, erase_pos);
  screen.print(' ');

  update_seaweed_spots();
  update_fish();
  Serial.println("---");

  screen.setCursor(HERO_POS_X, hero[0]);
  bool sound_cond = hero[0] == FLOOR && prev_state == FLOOR;
  if (game_over) {
    screen.write(SHRIMP_DEAD);
    tone(BUZZER_PIN, DEAD_SOUND, LOSE_SOUND_DURATION);
  } else if (ticks % 2 == 0 && sound_cond) {  // Walking animation
    screen.write(SHRIMP_1);
    tone(BUZZER_PIN, STEP_SOUND_A, STED_SOUND_DURAION);
  } else if (ticks % 2 == 1 && sound_cond) {
    screen.write(SHRIMP_2);
    tone(BUZZER_PIN, STEP_SOUND_B, STED_SOUND_DURAION);
  } else {
    screen.write(SHRIMP_1);
  }
}


// Help functions
void toggle_backlight() {
  backlight = !backlight;
  if (backlight) {
    screen.backlight();
    if (!game_over) {
      tone(BUZZER_PIN, SCREEN_ON_SOUND, TOGGLE_SCREEN_SOUND_DURATION);
    }
  } else {
    screen.noBacklight();
    if (!game_over) {
      tone(BUZZER_PIN, SCREEN_OFF_SOUND, TOGGLE_SCREEN_SOUND_DURATION);
    }
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

void reset_types() {
  for (int i = 0; i < SEAWEEDS_MAX_COUNT; i++) {
    for (int j = 0; j < 2; j++) {
      seaweeds[i][j] = -1;
    }
  }

  for (int i = 0; i < FISHES_MAX_COUNT; i++) {
    fish[i] = -2;
  }
}

void restart() {
  tone(BUZZER_PIN, START_SOUND, START_SOUND_DURATION);

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
}

void set_PWM() {
  TCCR1A = 0b00000001;
  TCCR1B = 0b00001001;
}
