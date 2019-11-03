//
//  Projet1.c
//  


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Solver.h"
#include "Engine.h"

int main(int argc, char **argv) {
    if (argc != 5) {
        printf("Wrong number of arguments");
        return -1;
    }
    
    int width = (int) strtol(argv[1], NULL, 0);
    int height = (int) strtol(argv[2], NULL, 0);
    int nbrBombs = (int) strtol(argv[3], NULL, 0);
    int player = (int) strtol(argv[4], NULL, 0);
    int success;
    if (player)
        success = play(width, height, nbrBombs, human, 1);
    else
        success = play(width, height, nbrBombs, heuristic, 1);
    
    if (success == -1) {
        printf("Bomb !\n");
    } else if (success == 1) {
        printf("Win !\n");
    } else {
        printf("Heuristic couldn't find a play\n");
    }
    return 0;
}
