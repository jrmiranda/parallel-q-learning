#include <iostream>
#include <omp.h>
#include "env.hpp"
#include "player.hpp"

using namespace std;

int main() {
  int nthreads, tid;
  float Q[SIZE][4] = { 0 };
  int episodes = 8000;
  float lr = 0.9;
  float y = 0.9;
  float eps = 0.8;
  float eps_decay = 0.9;
  float r_eps;
  int e, a, state;
  Env env;
  
  srand(time(NULL));

  #pragma omp parallel for shared(episodes, lr, y, eps_decay) private(e, a, env, state, eps, r_eps) reduction(+:Q[:SIZE][:4])
  for (e = 0; e < episodes; e++) {
    state = env.reset(1);
    eps *= eps_decay;
    while (!env.done) {
      r_eps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
      if (r_eps < eps) {
        a = rand() % 4;
      } else {
        a = argmax(Q[env.state]);
      }
      env.step(a);
      Q[state][a] += lr * (env.reward + y*max(Q[env.state]) - Q[state][a]);
      state = env.state;
    }
  }

  print_Q(Q);
  play(env, Q);

  return 0;
}