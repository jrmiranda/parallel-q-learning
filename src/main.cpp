#include <iostream>
#include <omp.h>
#include <time.h>
#include "env.hpp"
#include "player.hpp"

using namespace std;

int main() {
  int nranks, rank;
  float Q[SIZE][4] = { 0 };
  int episodes = 10000;
  float lr = 0.05;
  float y = 0.9;
  float eps = 0.8;
  float eps_decay = 0.9;
  float r_eps;
  int e, a, state;
  unsigned seed;
  Env env;
  
  srand(time(NULL));

  int i, episodes_n;
  for (i = 1; i <= 16; i=i*2) {
    episodes_n = i * episodes;
    cout << "Episódios:" << episodes_n << "\n";

    clock_t tick = clock();
    #pragma omp parallel shared(episodes, episodes_n, lr, y, eps_decay, Q) private(e, a, env, state, seed, eps, r_eps)
      {
        seed = 12345 + 42 * omp_get_thread_num();

        #pragma omp for
        for (e = 0; e < episodes_n; e++) {
          state = env.reset(1);
          eps *= eps_decay;
          
          while (!env.done) {
            r_eps = ((float)rand_r(&seed)/(float)(RAND_MAX)) * 1.0;
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
    cout << "Tempo: " << elapsed << "s\n";
    play(env, Q);
    cout << "\n";
  }

  return 0;
}