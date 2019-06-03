#include "env.hpp"
#include <iostream>
#include <cstring>
#include <omp.h>

using namespace std;

Env::Env(void) {
  int i, j, k, s;
  int state_actions[4], state_avail[4];
  s = 0;
  for (i = 0; i < ROWS; i++) {
    for (j = 0; j < COLS; j++) {
      state_actions[0] = i == 0 ? s : s - ROWS;
      state_actions[1] = j == (COLS-1) ? s : s + 1;
      state_actions[2] = i == (ROWS-1) ? s : s + ROWS;
      state_actions[3] = j == 0 ? s : s - 1;

      state_avail[0] = i == 0 ? 0 : 1;
      state_avail[1] = j == (COLS-1) ? 0 : 1;
      state_avail[2] = i == (ROWS-1) ? 0 : 1;
      state_avail[3] = j == 0 ? 0 : 1;

      for (k = 0; k < 4; k++) {
        actions[s][k] = state_actions[k];
        avail_actions[s][k] = state_avail[k];
      }

      s++;
    }
  }
}

void Env::consume_reward(int s) {
  rewards[s] = STEP;
}

int Env::reset(bool random_start = RANDOM_START) {
  if (random_start) {
    int r;
    do {
      r = rand() % SIZE;    
    } while (startRewards[r] != STEP);   
    state = r;
  } else {
    state = START;
  }
  reward = 0;
  energy = 0;
  done = false;
  memcpy(rewards, startRewards, sizeof(rewards));
  return state;
}

void Env::step(int a) {
  if (avail_actions[state][a]) {
    consume_reward(state);
    state = actions[state][a];
    reward = rewards[state];
    energy += reward;

    if (reward == END) done = true;
    if (reward == TRAP)  done = true;
  } else {
    done = true;
    reward = -10;
  }
}

void Env::debug() {
  unsigned int i;
  for (i = 0; i < sizeof(rewards)/sizeof(rewards[0]); i++) {
    cout << i << ": " << rewards[i] << "\n";
  }
}