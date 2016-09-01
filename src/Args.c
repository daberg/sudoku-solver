/*
 * SudokuSolver  Copyright (C) 2016  daberg
 * 
 * This file is part of SudokuSolver.
 *
 * SudokuSolver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SudokuSolver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with SudokuSolver.  If not, see <http://www.gnu.org/licenses/>.
 *  
 */

#include <stdio.h>
#include <string.h>

#include "Defs.h" 

int parseConfFromArgs (int argc, char** argv) {
    
    config.verbose = FALSE;
    config.algorithm = 0;
    config.programName = argv[0];
    
    boolean algChosen = FALSE;
    
    int i;
    
    for (i = 1; i < argc; i++) {
        
        if (argv[i][0] != '-') break;
        
        if (strcmp(argv[i], "-b") == 0) {
            if (algChosen) return -1;
            else {
                config.algorithm = BACKTRACKING;
                algChosen = TRUE;
            }
        }
        
        else if (strcmp(argv[i], "-bf") == 0) {
            if (algChosen) return -1;
            else {
                config.algorithm = BRUTEFORCE;
                algChosen = TRUE;
            }
        }
        
        else if (strcmp(argv[i], "-br") == 0) {
            if (algChosen) return -1;
            else {
                config.algorithm = BACKTRACKREC;
                algChosen = TRUE;
            }
        }
        
        else if (strcmp(argv[i], "-v") == 0) {
            config.verbose = TRUE;
        }
        
        else {
            return -1;
        }
    }
    
    if ((argc - i) != 1) {
        return -1;
    }
    else {
        targetFilename = argv[i];
        return 0;
    }
}

void printUsage (FILE* stream) {
    fprintf(stream, "USAGE: %s [FLAGS] [FILE]\n", config.programName);
}