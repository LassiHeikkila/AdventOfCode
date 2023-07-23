#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
#define BUF_SIZE 64
#define COMPARTMENT_MAX_SIZE 32

typedef struct compartment {
    char items[COMPARTMENT_MAX_SIZE];
    uint32_t len;
} compartment_t;

typedef struct rucksack {
    compartment_t first;
    compartment_t second;
} rucksack_t;

uint8_t item_to_priority(char item) {
    if (item >= 'A' && item <= 'Z') {
        // A -> 27 ... Z -> 52
        return (uint8_t)(item - 65 + 27);
    } else if (item >= 'a' && item <= 'z') {
        // a -> 1 ... z -> 26
        return (uint8_t)(item - 97 + 1);
    } else {
        return 0;
    }
}

void sort_compartment_by_prio(compartment_t *comp) {
    // let's use bubble sort
    for (int step = 0; step < (comp->len - 1); ++step) {
        for (int i = 0; i < (comp->len - step - 1); ++i) {
            if (item_to_priority(comp->items[i]) > item_to_priority(comp->items[i+1])) {
                char temp = comp->items[i];
                comp->items[i] = comp->items[i+1];
                comp->items[i+1] = temp;
            }
        }
    }
}

char find_common_item(compartment_t *first, compartment_t *second) {
    // assume compartments are sorted by item priority

    // iterate item by item from first compartment
    // check against items in second compartment one by one until priority matches or is greater
    int i = 0, j = 0;

    for (i = 0; i < first->len; ++i) {
        uint8_t first_prio = item_to_priority(first->items[i]);

        for (j = 0; j < second->len; ++j) {
            uint8_t second_prio = item_to_priority(second->items[j]);
            if (first_prio == second_prio) {
                return first->items[i];
            } else if (first_prio < second_prio) {
                break; // inner loop
            }
        }
    }

    // return NULL if no match found
    return '\0';
}

void print_compartment_items(compartment_t *comp) {
    for (int i = 0; i < comp->len; ++i) {
        printf("%c", comp->items[i]);
    }
}

void print_compartment_priorities(compartment_t *comp) {
    for (int i = 0; i < comp->len; ++i) {
        printf("%02u ", item_to_priority(comp->items[i]));
    }
}

void print_rucksack(rucksack_t *ruck) {
    printf("compartment 1:\t[ items\t");
    print_compartment_items(&ruck->first);
    printf(" ]\t[ priorities\t");
    print_compartment_priorities(&ruck->first);
    printf(" ]\n");

    printf("compartment 2:\t[ items\t");
    print_compartment_items(&ruck->second);
    printf(" ]\t[ priorities\t");
    print_compartment_priorities(&ruck->second);
    printf(" ]\n");
    }

int main() {
    uint32_t sum_of_duplicate_priorities = 0;
    size_t line_len = 0;
    size_t half_line_len = 0;
    size_t line_idx = 0;
    char common_item = '\0';

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    rucksack_t rucksack;
    
    
    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        ++line_idx;
        memset(&rucksack, 0, sizeof(rucksack_t)); // reset rucksack

        line_len = strlen(buf) - 1; // ignore newline
        half_line_len = line_len / 2;

        if ((line_len % 2) != 0) {
            printf("WARNING: uneven line %lu\n", line_idx);
            // TODO: what to do?
        }

        // copy first half of line into first compartment
        strncpy(rucksack.first.items, &buf[0], half_line_len);
        rucksack.first.len = half_line_len;
        // copy second half of line into second compartment
        strncpy(rucksack.second.items, &buf[half_line_len], half_line_len);
        rucksack.second.len = half_line_len;

#if DEBUG
        printf("\nnew rucksack: line %lu\n", line_idx);
        print_rucksack(&rucksack);
#endif // DEBUG
        
        sort_compartment_by_prio(&rucksack.first);
        sort_compartment_by_prio(&rucksack.second);

#if DEBUG
        printf("after sorting:\n");
        print_rucksack(&rucksack);
#endif // DEBUG
        common_item = find_common_item(&rucksack.first, &rucksack.second);
        if (common_item != '\0') {
#if DEBUG
            printf("found common item %c\n", common_item);
#endif // DEBUG
            sum_of_duplicate_priorities += item_to_priority(common_item);
        }
    }
    
    printf("sum_of_duplicate_priorities: %u\n", sum_of_duplicate_priorities);

    return 0;
}