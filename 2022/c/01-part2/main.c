#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
#define BUF_SIZE 64
#define TOP_COUNT 3

typedef struct elf {
    uint32_t index;
    uint32_t calories;
} elf_t;

uint32_t calorie_sum(elf_t *elves, uint32_t size) {
    uint32_t sum = 0;
    for (uint32_t i = 0; i < size; ++i) {
        sum += elves[i].calories;
    }
    return sum;
}

void print_elf(elf_t *elf) {
    printf("elf (%u) carrying %u calories\n", elf->index, elf->calories);
}

void print_elves(elf_t *elves, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        print_elf(&elves[i]);
    }
}

void insert_elf(elf_t *elves, uint32_t max_size, elf_t candidate) {
    uint32_t i = 0;
    uint32_t j = 0;

#if DEBUG
    printf("BEGIN INSERT\n");
    print_elves(elves, max_size);
#endif // DEBUG

    // check which index candidate should go to
    for (i = 0; i < max_size; ++i) {
        if (candidate.calories > elves[i].calories) {
            break;
        }
    }
    
    if (i >= max_size) {
        // new elf does not belong in top-n
        return;
    }
#if DEBUG
    printf("candidate elf (%u) should be inserted at position %u\n", candidate.index, i);
#endif // DEBUG

    // shift "lower" elves towards end of array, 
    // with last one being popped out completely (overwritten by second to last one)
    for (j = (max_size - 1); j > i; --j) {
        elves[j] = elves[j-1];
    }
    elves[i] = candidate;

#if DEBUG
    print_elves(elves, max_size);
    printf("END INSERT\n");
#endif // DEBUG
}

int main() {
    elf_t current_elf = { .index = 0, .calories = 0 };
    elf_t top_elves[TOP_COUNT] = { 
        [0] = {.index = 0, .calories = 0},
        [1] = {.index = 0, .calories = 0},
        [2] = {.index = 0, .calories = 0},
    };
    uint32_t line_idx = 0;

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    // read stdin line by line
    while (fgets(buf, BUF_SIZE, stdin) != NULL) {
        if (buf[0] == '\n') {
            // empty line -> end of current elf
#if DEBUG
            printf("end of elf\n");
            print_elf(&current_elf);
#endif // DEBUG

            // store current elf in top three if needed
            if (current_elf.calories > top_elves[TOP_COUNT - 1].calories) {
                insert_elf(top_elves, TOP_COUNT, current_elf);
            }

            // reset current elf calories and increment index
            current_elf.calories = 0;
            current_elf.index++;

            memset(buf, 0, BUF_SIZE);
            continue;
        } 
        
        // non-empty line -> add calories to current elf
        current_elf.calories += atoi(buf);
        
        memset(buf, 0, BUF_SIZE);
    }

    printf("Top three elves are carrying %u calories\n", calorie_sum(top_elves, TOP_COUNT));
    print_elves(top_elves, TOP_COUNT);
    
    return 0;
}