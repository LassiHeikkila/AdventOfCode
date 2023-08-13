#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
#define BUF_SIZE 32

/*!
 * @brief Data structure representing a section ID range.
 */
typedef struct section_range {
    uint32_t begin_id;
    uint32_t end_id;
} section_range_t;

/*!
 * @brief Checks if range `a` is fully contained in `b`.
 * @short Containment is defined as: beginning of `a` >= beginning of `b` AND end of `a` <= end of `b`
 * @param a : @ref section_range_t defining first range
 * @param b : @ref section_range_t defining second range
 * @return boolean indicating if `a` is contained in `b`
 */
bool is_contained_in(section_range_t a, section_range_t b);

int main() {
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    
    uint32_t total_num_ranges = 0, num_fully_overlapping_ranges = 0;
    section_range_t a = {0,0}, b = {0,0};

    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        ++total_num_ranges;
        #if DEBUG
            printf("[DEBUG] current line: %s", buf);
        #endif // DEBUG

        sscanf(buf, "%u-%u,%u-%u", &a.begin_id, &a.end_id, &b.begin_id, &b.end_id);

        #if DEBUG
            printf("[DEBUG] range a: %u - %u\n", a.begin_id, a.end_id);
            printf("[DEBUG] range b: %u - %u\n", b.begin_id, b.end_id);
        #endif // DEBUG

        if (is_contained_in(a, b)) {
            #if DEBUG
                printf("[DEBUG] range a is fully contained in range b\n");
            #endif // DEBUG
            ++num_fully_overlapping_ranges;
        } else if (is_contained_in(b, a)) {
            #if DEBUG
                printf("[DEBUG] range b is fully contained in range a\n");
            #endif // DEBUG
            ++num_fully_overlapping_ranges;
        }
    }
    
    printf("total number of assignment pairs: %u\n", total_num_ranges);
    printf("number of assignments fully contained in pair's assignment: %u\n", num_fully_overlapping_ranges);

    return 0;
}

bool is_contained_in(section_range_t a, section_range_t b) {
    return a.begin_id >= b.begin_id && a.end_id <= b.end_id;
}