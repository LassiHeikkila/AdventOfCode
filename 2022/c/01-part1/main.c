#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
#define BUF_SIZE 64

typedef struct elf {
    uint32_t index;
    uint32_t calories;
} elf_t;

int main() {
    elf_t current_elf = { .index = 0, .calories = 0 };
    elf_t max_elf = { .index = 0, .calories = 0 };
    uint32_t line_idx = 0;

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    // read stdin line by line
    while (fgets(buf, BUF_SIZE, stdin) != NULL) {
        if (buf[0] == '\n') {
            // empty line -> end of current elf
#if DEBUG
            printf("end of elf (%u), count was %u calories\n", current_elf.index, current_elf.calories);
#endif // DEBUG

            // store current elf as max elf if needed
            if (current_elf.calories > max_elf.calories) {
                max_elf = current_elf;
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

    printf("Max elf (%u) is carrying %u calories\n", max_elf.index, max_elf.calories);

    return 0;
}