#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
#define BUF_SIZE 64
#define RUCKSACK_MAX_SIZE 64
#define GROUP_SIZE 3

typedef struct rucksack {
    char items[RUCKSACK_MAX_SIZE];
    uint32_t len;
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

void sort_item_array_by_prio(char *items, uint32_t len) {
    // let's use bubble sort
    for (int32_t step = 0; step < (len - 1); ++step) {
        for (int32_t i = 0; i < (len - step - 1); ++i) {
            if (item_to_priority(items[i]) > item_to_priority(items[i+1])) {
                char temp = items[i];
                items[i] = items[i+1];
                items[i+1] = temp;
            }
        }
    }
}

void sort_rucksack_by_prio(rucksack_t *rucksack) {
    sort_item_array_by_prio(rucksack->items, rucksack->len);
}

int32_t find_first_triplicate(char *items, uint32_t len) {
    // assumes sorted array
    for (int32_t i = 0; i < (len - 2); ++i) {
        if (items[i] == items[i+1] && items[i] == items[i+2]) {
            return i;
        }
    }

    return -1;
}

uint32_t remove_duplicate_items(char *items, uint32_t len) {
    // assumes sorted array
    uint32_t items_removed = 0;

    for (uint32_t idx = 0; idx < (len - items_removed); ) {
        // idx and idx+1 duplicates?
        if (items[idx] == items[idx+1]) {
            // shift items[idx+1:] onto items[idx:]
            memmove(&items[idx], &items[idx+1], len-idx-1);
            // set last item to NULL
            items[len-1] = '\0';
            
            ++items_removed;
        } else {
            // only increment index if we didn't encounter a duplicate, 
            // otherwise we would end up skipping successive duplicates
            ++idx;
        }
    }

    // return new length
    return len-items_removed;
}

int32_t find_common_item_idx(rucksack_t *first, rucksack_t *second, int32_t starting_idx) {
    // assume rucksacks are sorted by item priority

    // iterate item by item from first rucksack
    // check against items in second rucksack one by one until priority matches or is greater
    int32_t i, j;

    for (i = starting_idx; i < first->len; ++i) {
        uint8_t first_prio = item_to_priority(first->items[i]);

        for (j = 0; j < second->len; ++j) {
            uint8_t second_prio = item_to_priority(second->items[j]);
            if (first_prio == second_prio) {
                return i;
            } else if (first_prio < second_prio) {
                break; // inner loop
            }
        }
    }

    // return -1 if no match found
    return -1;
}

int32_t find_all_common_items(rucksack_t *first, rucksack_t *second, char *items) {
    // this also finds duplicate matches
    int32_t common_idx = find_common_item_idx(first, second, 0);
    int32_t idx = 0;
    while (common_idx != -1 ) {
        items[idx] = first->items[common_idx];
        ++idx;
        common_idx = find_common_item_idx(first, second, common_idx+1);
    }
    return idx;
}

char find_threeway_common_item(rucksack_t *A, rucksack_t *B, rucksack_t *C) {
    // common item should be the intersection of common items between A-B, B-C, and C-A

    char common_AB[RUCKSACK_MAX_SIZE];
    char common_BC[RUCKSACK_MAX_SIZE];
    char common_CA[RUCKSACK_MAX_SIZE];
    char combo[RUCKSACK_MAX_SIZE];

    memset(common_AB, 0, RUCKSACK_MAX_SIZE);
    memset(common_BC, 0, RUCKSACK_MAX_SIZE);
    memset(common_CA, 0, RUCKSACK_MAX_SIZE);
    memset(combo, 0, RUCKSACK_MAX_SIZE);
    
    int32_t num_matching_AB = find_all_common_items(A, B, common_AB);
    int32_t num_matching_BC = find_all_common_items(B, C, common_BC);
    int32_t num_matching_CA = find_all_common_items(C, A, common_CA);

#if DEBUG
    printf("raw common sets:\n");
    printf("matching item counts: AB = %d, BC = %d, CA = %d\n", num_matching_AB, num_matching_BC, num_matching_CA);
    printf("matching items: AB = %s, BC = %s, CA = %s\n", common_AB, common_BC, common_CA);
#endif // DEBUG

    // remove duplicates
    num_matching_AB = remove_duplicate_items(common_AB, num_matching_AB);
    num_matching_BC = remove_duplicate_items(common_BC, num_matching_BC);
    num_matching_CA = remove_duplicate_items(common_CA, num_matching_CA);

#if DEBUG
    printf("cleaned common sets:\n");
    printf("matching item counts: AB = %d, BC = %d, CA = %d\n", num_matching_AB, num_matching_BC, num_matching_CA);
    printf("matching items: AB = %s, BC = %s, CA = %s\n", common_AB, common_BC, common_CA);
#endif // DEBUG

    // how to find intersection of three strings?
    // assuming there's at most one match, or at least we don't care about extras,
    // we can mash together the sets AB, BC and CA, sort by priority, and find first triplicate item.

    memcpy(&combo[0], common_AB, num_matching_AB);
    memcpy(&combo[num_matching_AB], common_BC, num_matching_BC);
    memcpy(&combo[num_matching_AB + num_matching_BC], common_CA, num_matching_CA);

    uint32_t combo_len = num_matching_AB + num_matching_BC + num_matching_CA;

#if DEBUG
    printf("matching items union: %s length %u\n", combo, combo_len);
#endif // DEBUG

    sort_item_array_by_prio(combo, combo_len);

#if DEBUG
    printf("sorted matching items union: %s length %u\n", combo, combo_len);
#endif // DEBUG

    int32_t common_idx = find_first_triplicate(combo, combo_len);

    if (common_idx == -1) {
#if DEBUG
        printf("no common item found\n");
#endif // DEBUG
        return '\0';
    }

    char common_item = combo[common_idx];
#if DEBUG
    printf("common item found: %c\n", common_item);
#endif // DEBUG

    return common_item;
}

void print_rucksack_items(rucksack_t *comp) {
    for (int32_t i = 0; i < comp->len; ++i) {
        printf("%c", comp->items[i]);
    }
}

void print_rucksack_priorities(rucksack_t *comp) {
    for (int32_t i = 0; i < comp->len; ++i) {
        printf("%02u ", item_to_priority(comp->items[i]));
    }
}

void print_rucksack(rucksack_t *ruck) {
    printf("rucksack:\t[ items\t");
    print_rucksack_items(ruck);
    printf(" ]\t[ priorities\t");
    print_rucksack_priorities(ruck);
    printf(" ]\n");
}

int main() {
    uint32_t sum_of_badge_priorities = 0;
    size_t line_len = 0;
    size_t line_idx = 0;
    size_t elf_idx = 0;
    char common_item = '\0';

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    rucksack_t rucksacks[GROUP_SIZE];
        
    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        ++line_idx;
        memset(&rucksacks[elf_idx % GROUP_SIZE], 0, sizeof(rucksack_t)); // reset rucksack

        line_len = strlen(buf) - 1; // ignore newline

        if ((line_len % 2) != 0) {
            printf("WARNING: uneven line %lu\n", line_idx);
            // TODO: what to do?
        }

        // copy line into rucksack
        strncpy(rucksacks[elf_idx % GROUP_SIZE].items, buf, line_len);
        rucksacks[elf_idx % GROUP_SIZE].len = line_len;

#if DEBUG
        printf("\nnew rucksack: line %lu\n", line_idx);
        print_rucksack(&rucksacks[elf_idx % GROUP_SIZE]);
#endif // DEBUG
        
        sort_rucksack_by_prio(&rucksacks[elf_idx % GROUP_SIZE]);

#if DEBUG
        printf("after sorting:\n");
        print_rucksack(&rucksacks[elf_idx % GROUP_SIZE]);
#endif // DEBUG
        if ((elf_idx % GROUP_SIZE) == 2) {
            common_item = find_threeway_common_item(&rucksacks[0], &rucksacks[1], &rucksacks[2]);
            if (common_item != '\0') {
#if DEBUG
                printf("found common item %c\n", common_item);
#endif // DEBUG
                sum_of_badge_priorities += item_to_priority(common_item);
            }
        }

        ++elf_idx;
    }
    
    printf("sum_of_badge_priorities: %u\n", sum_of_badge_priorities);

    return 0;
}