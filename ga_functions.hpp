#pragma once
#include "ga_structs.hpp"
#ifndef GA_FUNCTIONS
#define GA_FUNCTIONS
feasibility_details feasibility(const chromosome &c);
int average_cost(chromosome chromosomes);
chromosome crossover(const chromosome A, const chromosome B);
void mutation(chromosome &off_spring, float mutation_rate);
int makespan(schedule s);
float fitness(float average_cost, int make_span);
#endif // GA_FUNCTIONS