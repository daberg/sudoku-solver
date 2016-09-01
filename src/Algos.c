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

#define SCREEN_PRINT_WAIT_CYCLES 10000


int bruteForceTableSolution (TABLE* p_startTable, TABLE* p_destTable) {
    
    if (!isTableValid(*p_startTable)) return -1;
    
    if (isTableComplete(*p_startTable)) {
        if (checkTable(*p_startTable) == FALSE) {
            p_destTable = NULL;
            return 0;
        }
        else {
            p_destTable = p_startTable;
            return 0;
        }
    }
    
    TABLE currTable;
    initTable(&currTable);
    
    copyTable(p_startTable, &currTable);
    
    int* cells = getTableCells(&currTable);
    
    int* startCells = getTableCells(p_startTable);
    
    int i;
    
    int size = getTableSize(&currTable);
    
    for (i = 0; i < size; i++) {
        if (cells[i] == 0) cells[i] = 1;
    }
    
    int firstGuessCell;
    for (i = 0; i < size; i++) {
        if (startCells[i] == 0) break;
    }
    firstGuessCell = i;
    
    int currCell = firstGuessCell;
    
    boolean needsRestart = FALSE;
    
    int printCount = SCREEN_PRINT_WAIT_CYCLES;
    
    while (!checkTable(currTable)) {
        
        if (config.verbose) {
            if (printCount == SCREEN_PRINT_WAIT_CYCLES) {
                printf("\n\nAttempt:\n%s\n\n\n\n...\n\n\n\n\n", tableToString(currTable));
                printCount = 0;
            }
            else
                printCount++;
        }
        
        if (cells[currCell] == 9) {
            
            while (cells[currCell] == 9) {
                
                if (startCells[currCell] == 0) {
                    
                    cells[currCell] = 1;              
                    currCell++;
                    
                    if (currCell == size) {
                        freeTable(&currTable);
                        return 1;
                    }
                }
                
                while (startCells[currCell] != 0) {
                    currCell++;
                    if (currCell == size) {
                        freeTable(&currTable);
                        return 1;
                    }
                }
            }
            
            needsRestart = TRUE;
        }
        
        cells[currCell]++;
        
        if (needsRestart) currCell = firstGuessCell;
    }
    
    copyTable(&currTable, p_destTable);
    freeTable(&currTable);
    
    return 0;
}

int backTrackTableSolution (TABLE* p_startTable, TABLE* p_destTable) {
    
    if (!isTableValid(*p_startTable)) return -1;
    
    TABLE currTable;
    
    initTable(&currTable);
    copyTable(p_startTable, &currTable);
    
    int* cells = getTableCells(&currTable);
    
    int* startCells = getTableCells(p_startTable);
    
    int size = getTableSize(&currTable);
    
    int currCell = 0;
    
    int printCount = SCREEN_PRINT_WAIT_CYCLES;
    
    boolean hasSolution = FALSE;
    boolean stopSearch = FALSE;
    boolean moveToNext;
    
    while (!stopSearch) {
        
        while (startCells[currCell] != 0) {
            currCell++;
            if (currCell == size) {
                hasSolution = TRUE;
                stopSearch = TRUE;
            }
        }
        
        moveToNext = FALSE;
        
        while (!stopSearch && !moveToNext) {
            
            cells[currCell]++;
            
            if (cells[currCell] == 10) {
                
                cells[currCell] = 0;
                
                do {
                    
                    currCell--;
                    
                    if (currCell == -1) {
                        stopSearch = TRUE;
                        hasSolution = FALSE;
                    }
                    
                } while (startCells[currCell] != 0);
                
                moveToNext = TRUE;
            }
            
            else {
                
                if (config.verbose) {
                    if (printCount == SCREEN_PRINT_WAIT_CYCLES) {
                        printf("\n\nAttempt:\n%s\n\n\n\n...\n\n\n\n", tableToString(currTable));
                        printCount = 0;
                    }
                    else
                        printCount++;
                }
            
                if (checkCell(currTable, currCell)) {
                
                    currCell++;
                    
                    if (currCell == size) {
                        stopSearch = TRUE;
                        hasSolution = TRUE;
                    }
                    
                    moveToNext = TRUE;
                }
            }
        }
    }
    
    if (hasSolution) {
        copyTable(&currTable, p_destTable);
        freeTable(&currTable);
        return 0;
    }
    
    else {
        freeTable(&currTable);
        return 1;
    }
}

int backTrackRecurse (int* startCells, TABLE* p_currTable, int currCell) {
    
    int size = getTableSize(p_currTable);
    
    if (currCell == size) return 0;
    
    while (startCells[currCell] != 0) {
        currCell++;
            if (currCell == size) {
                return 0;
            }
    }
    
    int* cells = getTableCells(p_currTable);
    
    int attempt;
    
    for (attempt = 1; attempt < 10; attempt++) {
        
        cells[currCell] = attempt;
        
        if (checkCell(*p_currTable, currCell)) {
            if (backTrackRecurse(startCells, p_currTable, currCell + 1) == 0) return 0;
        }
    }
    
    cells[currCell] = 0;
    
    return 1;
}

int backTrackTableSolutionRecursive (TABLE* p_startTable, TABLE* p_destTable) {
    
    if (!isTableValid(*p_startTable)) return -1;
    
    TABLE currTable;
    
    initTable(&currTable);
    copyTable(p_startTable, &currTable);
    
    int* startCells = getTableCells(p_startTable);
    
    int size = getTableSize(&currTable);
    
    int ret;
    
    ret = backTrackRecurse(startCells, &currTable, 0);
    
    if (ret == 0) {
        copyTable(&currTable, p_destTable);
    }
    
    freeTable(&currTable);
    
    return ret;
}
