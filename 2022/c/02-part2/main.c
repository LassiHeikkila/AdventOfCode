#include <stdbool.h> // bools
#include <stdint.h>  // uint32_t
#include <stdio.h>   // stdin, fgets
#include <string.h>  // memset
#include <stdlib.h>  // atoi

#define DEBUG 0
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

result_t desired_result(char s) {
    switch (s) {
        case 'X':
            return Loss;
        case 'Y':
            return Draw;
        case 'Z':
            return Win;
    }
}

rps_t required_move(rps_t opponent_move, result_t target_result) {
    switch (opponent_move) {
        case Rock:
            switch (target_result) {
                case Loss:
                    return Scissors;
                case Draw:
                    return Rock;
                case Win:
                    return Paper;
            }
        case Paper:
            switch (target_result) {
                case Loss:
                    return Rock;
                case Draw:
                    return Paper;
                case Win:
                    return Scissors;
            }
        case Scissors:
            switch (target_result) {
                case Loss:
                    return Paper;
                case Draw:
                    return Scissors;
                case Win:
                    return Rock;
            }
    }
    return UnknownMove;
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

    char opponent_strtg, required_rslt;
    rps_t opponent_move, player_move;
    result_t required_result;
    int32_t points = 0;
    
    while(fgets(buf, BUF_SIZE, stdin) != NULL) {
        // TODO: handle possible errors
        sscanf(buf, "%c %c", &opponent_strtg, &required_rslt);

        opponent_move = opponent_strategy(opponent_strtg);
        required_result = desired_result(required_rslt);
        player_move = required_move(opponent_move, required_result);

        points = (int32_t)player_move + (int32_t)required_result;
#if DEBUG
        printf("opponent (%c / %s), player (%s), result: %s, points %d\n", 
            opponent_strtg, move_to_str(opponent_move), 
            move_to_str(player_move), 
            result_to_str(required_result), 
            points
        );
#endif // DEBUG
        accumulated_score += points;
    }
    
    printf("accumulated_score: %u\n", accumulated_score);

    return 0;
}