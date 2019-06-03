#ifndef PLAYER_H
#define PLAYER_H

int argmax(float vector[4]);
float max(float vector[4]);
int random_argmax(float vector[4]);
void print_Q(float Q[SIZE][4]);
void play(Env env, float Q[SIZE][4]);

#endif