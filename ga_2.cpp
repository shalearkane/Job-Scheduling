#include "ga_constants.hpp"
#include "ga_functions.hpp"
#include "ga_inputs.hpp"
#include "ga_structs.hpp"
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    srand(time(0));
    chromosome c;
    cout << (rand() % 10) + 1;
    chromosome population[20];
    fitness(12, 23);
}