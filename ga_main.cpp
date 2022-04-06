#include "ga_cmp.hpp"
#include "ga_constants.hpp"
#include "ga_functions.hpp"
#include "ga_heft.hpp"
#include "ga_inputs.hpp"
#include "ga_print.hpp"
#include "ga_structs.hpp"
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iostream>
using namespace std;

vector<chromosome> population_array;
float average_fitness_val;

vector<chromosome> population_gen_random() {
    vector<chromosome> result;
    for (int i = 0; i < MAX_POPULATION - 1; i++) {
        chromosome c;
        set<int> queued;
        for (int j = 1; j <= MAX_TASKS; j++) {
            int task = (rand() % MAX_TASKS) + 1;
            while (queued.count(task) != 0) {
                task = (rand() % MAX_TASKS) + 1;
            }
            queued.insert(task);
            c.genes[j].task = task;
            c.genes[j].processor = (rand() % 3) + 1;
        }

        if (feasibility(c).is_feasible) {
            result.push_back(c);
            cerr << "First population : \n";
            print_chromosome(c);
        } else {
            i--;
        }
    }
    return result;
}

void population(const chromosome heft) {
    int i = 0;
    chromosome temp;
    population_array = population_gen_random();
    // while (i < MAX_POPULATION - 1) {
    //     temp = mutation(heft, 0.1);
    //     if (feasibility(temp).is_feasible) {
    //         population_array.push_back(temp);
    //         i++;
    //     };
    // }
    // population_array.push_back(heft);
}

void generation() {
    vector<chromosome>::iterator itr;
    float sum_fitness;
    for (itr = population_array.begin(); itr < population_array.end(); itr++) {
        itr->average_cost = average_cost(*itr);
        itr->sched = feasibility(*itr).sched;
        assert(feasibility(*itr).is_feasible);
        itr->makespan = makespan(itr->sched);

        itr->fitness_value = fitness(itr->average_cost, itr->makespan);
        sum_fitness += itr->fitness_value;

        // cerr << "average_cost : " << itr->average_cost
        //      << ", makespan : " << itr->makespan
        //      << ", fitness : " << itr->fitness_value << '\n';
    }

    average_fitness_val = sum_fitness / (float)population_array.size();

    // sort(population_array.begin(), population_array.end(), cmp_fitness_val);

    for (int i = 0; i < 14; i += 2) {
        chromosome temp_1 = mutation(
            (crossover(population_array[i], population_array[i + 1])).c1, 0.1);

        chromosome temp_2 = mutation(
            (crossover(population_array[i], population_array[i + 1])).c2, 0.1);

        if (feasibility(temp_1).is_feasible) {
            temp_1.average_cost = average_cost(temp_1);
            temp_1.sched = feasibility(temp_1).sched;
            temp_1.makespan = makespan(temp_1.sched);
            temp_1.fitness_value =
                fitness(temp_1.average_cost, temp_1.makespan);
            population_array.push_back(temp_1);
        };

        if (feasibility(temp_2).is_feasible) {
            temp_2.average_cost = average_cost(temp_2);
            temp_2.sched = feasibility(temp_2).sched;
            temp_2.makespan = makespan(temp_2.sched);
            temp_2.fitness_value =
                fitness(temp_2.average_cost, temp_2.makespan);
            population_array.push_back(temp_2);
        };
    }
    population_array = roulette(population_array);
    if (DEB)
        cerr << "size : " << population_array.size() << '\n';

    sort(population_array.begin(), population_array.end(), cmp_fitness_val);
    if (population_array.size() > MAX_POPULATION)
        population_array.resize(MAX_POPULATION);
}

int main() {
    srand(time_t(0));
    chromosome heft = get_heft_chromosome();
    print_genes(heft.genes);
    feasibility_details fd_heft = feasibility(heft);
    int mk_heft = makespan(fd_heft.sched);
    cout << "makespan heft : " << mk_heft << '\n';

    population(heft);
    population_array.push_back(heft);
    for (int i = 0; i < 200; i++) {
        generation();
    }

    print_genes(population_array.front().genes);
    feasibility_details fd_final = feasibility(population_array.front());
    print_schedule(fd_final.sched);
    int mk_final = makespan(fd_final.sched);
    cout << "makespan final : " << mk_final << '\n';

    for (int i = 0; i < population_array.size(); i++) {
        print_genes(population_array[i].genes);
    }

    return 0;
}