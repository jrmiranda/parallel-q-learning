#include <iostream>
#include <omp.h>
#include <time.h>
#include "env.hpp"
#include "player.hpp"

using namespace std;

int main() {
  int nranks, rank;
  float Q[SIZE][4] = { 0 };
  int episodes = 1000;
  float lr = 0.05;
  float y = 0.9;
  float eps = 0.8;
  float eps_decay = 0.9;
  float r_eps;
  int e, a, state;
  Env env;
  
  srand(time(NULL));

  clock_t tick = clock();
  #pragma omp parallel shared(episodes, lr, y, eps_decay) private(e, a, env, state, eps, r_eps)
  {
    rank = omp_get_thread_num();
    cout << rank << " ";
    
    for (e = 0; e < episodes; e++) {
      state = env.reset(1);
      eps *= eps_decay;
      
      while (!env.done) {
        r_eps = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        a = r_eps < eps ? rand() % 4 : argmax(Q[env.state]);
        env.step(a);

        #pragma omp atomic
        Q[state][a] += lr * (env.reward + y*max(Q[env.state]) - Q[state][a]);
        
        state = env.state;
      }
    }
  }
  clock_t tock = clock();
  double elapsed = (double)(tock-tick)/CLOCKS_PER_SEC;

  //print_Q(Q);
  cout << "Execution time: " << elapsed << "s\n";
  play(env, Q);

  return 0;
}