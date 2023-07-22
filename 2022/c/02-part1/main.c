#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 1
#define BUF_SIZE 8

typedef enum rps {
    UnknownMove = -1,
    Rock = 1,
    Paper = 2,
    Scissors = 3
} rps_t;

typedef enum result {
    UnknownResult = -1,
    Loss = 0,
    Draw = 3,
    Win = 6
} result_t;

char *move_to_str(rps_t move) {
    switch (move) {
        case Rock:
            return "Rock";
        case Paper:
            return "Paper";
        case Scissors:
            return "Scissors";
        default:
            return "Unknown move";
    }
}
char *result_to_str(result_t res) {
    switch (res) {
        case Loss:
            return "Loss";
        case Draw:
            return "Draw";
        case Win:
            return "Win";
        default:
            return "Unknown result";
    }
}

rps_t opponent_strategy(char s) {
    switch (s) {
        case 'A':
            return Rock;
        case 'B':
            return Paper;
        case 'C':
            return Scissors;
    }
    return UnknownMove;
}

rps_t player_strategy(char s) {
    switch (s) {
        case 'X':
            return Rock;
        case 'Y':
            return Paper;
        case 'Z':
            return Scissors;
    }
}

result_t match_result(rps_t player, rps_t opponent) {
    switch (player) {
        case Rock: {
            switch (opponent) {
                case Rock: {
                    return Draw;
                }
                case Paper: {
                    return Loss;
                }
                case Scissors: {
                    return Win;
                }
            }
        }
        case Paper: {
            switch (opponent) {
                case Rock: {
                    return Win;
                }
                case Paper: {
                    return Draw;
                }
                case Scissors: {
                    return Loss;
                }
            }
        }
        case Scissors: {
            switch (opponent) {
                case Rock: {
                    return Loss;
                }
                case Paper: {
                    return Win;
                }
                case Scissors: {
                    return Draw;
                }
            }
        }
    }
    return UnknownResult;
}

char strategy_map;

int main() {
    uint32_t accumulated_score = 0;

    char buf[BUF_SIZE];
    memset(buf, 0, BUF_SIZE);

    char opponent_strtg, player_strtg;
    rps_t opponent_move, player_move;
    result_t res;
    int32_t points = 0;
    
    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        // TODO: handle possible errors
        sscanf(buf, "%c %c", &opponent_strtg, &player_strtg);

        opponent_move = opponent_strategy(opponent_strtg);
        player_move = player_strategy(player_strtg);

        res = match_result(player_move, opponent_move);
        points = (int32_t)player_move + (int32_t)res;
#if DEBUG
        printf("opponent (%c / %s), player (%c / %s), result: %s, points %d\n", 
            opponent_strtg, move_to_str(opponent_move), 
            player_strtg, move_to_str(player_move), 
            result_to_str(res), 
            points
        );
#endif // DEBUG
        accumulated_score += points;
    }
    
    printf("accumulated_score: %u\n", accumulated_score);

    return 0;
}