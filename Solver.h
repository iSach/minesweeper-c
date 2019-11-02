//
//  Solver.h
//  


#ifndef Solver_h
#define Solver_h

#include <stdio.h>
#include "Grid.h"
#include "Engine.h"

Move human(Grid *grid);
Move heuristic(Grid *grid);
#endif /* Solver_h */
