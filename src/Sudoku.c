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

#include "Defs.h"

CONF config;
char* targetFilename;

int main (int argc, char** argv) {
    
    if (parseConfFromArgs(argc, argv) != 0) {
        fprintf(stderr, "Invalid arguments.\n");
        printUsage(stderr);
        return -1;
    }
    
    TABLE table;
    initTable(&table);
    
    if (fetchTableFromFile(targetFilename, &table) == -1) {
        printf("Error reading file '%s'.\n", targetFilename);
        return -1;
    }
    
    int (*solveAlgs[3]) (TABLE* p_startTable, TABLE* p_destTable);
    solveAlgs[BACKTRACKREC] = backTrackTableSolutionRecursive;
    solveAlgs[BACKTRACKING] = backTrackTableSolution;
    solveAlgs[BRUTEFORCE] = bruteForceTableSolution;
    
    TABLE destTable;
    initTable(&destTable);
    
    int ret = solveAlgs[config.algorithm](&table, &destTable);
    
    if (ret == 0) {
        printf("Solution found:\n");
        printTable(destTable);
    }
    
    else if (ret == 1) {
        printf("No solutions were found.\n");
    }
    
    else {
        printf("Error.\n");
    }
    
    freeTable(&table);
    freeTable(&destTable);
}
