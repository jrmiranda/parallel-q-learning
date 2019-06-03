#include <iostream>
#include <iomanip>
#include <ctime>
#include "env.hpp"
#include "player.hpp"

using namespace std;

int argmax(float vector[4]) {
  int i = 0, max;
  while (i < 4) {
    if (i == 0) max = i;
    else max = vector[i] > vector[i-1] ? i : max;
    i++;
  }
  return max;
}

float max(float vector[4]) {
  int i = 0, max;
  while (i < 4) {
    if (i == 0) max = i;
    else max = vector[i] > vector[i-1] ? i : max;
    i++;
  }
  return vector[max];
}

int random_argmax(float vector[4]) {
  int i = 0, count = 0, max_val = max(vector);
  int r;

  while (i < 4) {
    if (vector[i] == max_val) count++;
    i++;
  }

  if (count > 1) {
    int *equals = new int[count];
    i = 0;
    while (i < 4) {
      equals[i] = vector[i];
      i++;
    }
    r = rand() % count;
  } else {
    r = argmax(vector);
  }

  return r;
}

void print_Q(float Q[SIZE][4]) {
  int i, j, s = 0;
  for (i = 0; i < SIZE; i++) {
    cout << " " << s << " |\t";
    for (j = 0; j < 4; j++) {
      cout << fixed << setprecision(2) << Q[i][j] << "\t|\t";
    }
    s++;
    cout << "\n-----------------------------------------------------------------\n";
  }
}

void play(Env env, float Q[SIZE][4]) {
  int a, steps = 0, state = env.reset(0);
  cout << "[";
  while (!env.done) {
    a = random_argmax(Q[env.state]);
    env.step(a);
    cout << env.state << ",";
    if (steps > 100) break;
    steps++;
  }
  cout << "]\n";
}