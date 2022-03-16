#define MAX_GENES 10

typedef struct {
    int task;
    int processor;
} gene;

typedef struct {
    gene genes[MAX_GENES];
    int average_cost;
    int makespan;
} chromosome;

int main() {
    chromosome c;
    return 0;
}