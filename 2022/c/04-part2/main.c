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
 * @brief Checks if range `a` and range `b` overlap.
 * @param a : @ref section_range_t defining first range
 * @param b : @ref section_range_t defining second range
 * @return boolean indicating the given ranges overlap
 */
bool ranges_overlap(section_range_t a, section_range_t b);


int main() {
    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);
    
    uint32_t total_num_ranges = 0, num_overlapping_ranges = 0;
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

        if (ranges_overlap(a, b)) {
            #if DEBUG
                printf("[DEBUG] ranges overlap\n");
            #endif // DEBUG
            ++num_overlapping_ranges;
        }
        
    }
    
    printf("total number of assignment pairs: %u\n", total_num_ranges);
    printf("number of overlapping assignment pairs: %u\n", num_overlapping_ranges);

    return 0;
}

bool ranges_overlap(section_range_t a, section_range_t b) {
    // how is overlapping determined?
    // beginning and/or end of one range is inside the other range
    
    // it is necessary to also check the other way around.

    return (a.begin_id >= b.begin_id && a.begin_id <= b.end_id) 
        || (a.end_id >= b.begin_id && a.end_id <= b.end_id)
        || (b.begin_id >= a.begin_id && b.begin_id <= a.end_id) 
        || (b.end_id >= a.begin_id && b.end_id <= a.end_id);
}