#include "Solver.h"
#include "Grid.h"

Move human(Grid *grid) {
    Move m;
    m.x = -1;
    m.y = -1;
    m.flag = -2;
    while(m.x == -1) {
        printf("Veuillez entrer une action, et des coordonnées :\n");
        printf("Actions possibles : 1 (placer un drapeau), 0 (révéler une case), -1 (abandonner).\n");
        printf("Format : <action> <x> <y>\n");

        int entered_flag, entered_x, entered_y;
        scanf("%d %d %d", &(entered_flag), &(entered_x), &(entered_y));

        if(entered_x < 0 || entered_y < 0 || entered_x > gridWidth(grid) - 1 || entered_y > gridHeight(grid) - 1) {
            printf("Position invalide. Veuillez réessayer.\n\n");
            continue;
        }
        if(entered_flag < -1 || entered_flag > 1) {
            printf("Action invalide. Veuillez réessayer.\n\n");
            continue;
        }
        m.flag = entered_flag;
        m.x = entered_x;
        m.y = entered_y;
    }
    return m;
}
Move heuristic(Grid *grid) {
    Move m;
    return m;
}