#pragma once
#include "ga_structs.hpp"
#ifndef GA_FUNCTIONS
#define GA_FUNCTIONS
feasibility_details feasibility(const chromosome c);
float average_cost(const chromosome chromosomes);
offspring crossover(const chromosome A, const chromosome B);
chromosome mutation(chromosome off_spring, const float mutation_rate);
int makespan(const schedule &s);
float fitness(const float average_cost, const int make_span);
#endif // GA_FUNCTIONS