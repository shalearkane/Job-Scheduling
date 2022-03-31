#include "ga_cmp.hpp"

bool cmp_fitness_val(const chromosome &c1, const chromosome &c2) {
    return (c1.fitness_value > c2.fitness_value);
}

bool cmp_chromosome(const chromosome &c1, const chromosome &c2) {
    for (int i = 1; i <= MAX_TASKS; i++) {
        if (c1.genes[i].task != c2.genes[i].task ||
            c1.genes[i].processor != c2.genes[i].processor)
            return false;
    }
    return true;
}