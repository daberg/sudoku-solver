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

boolean isTableComplete (TABLE table) {
    
    int size = getTableSize(&table);
    
    int* cells = getTableCells(&table);
    
    int i;
    
    for (i = 0; i < size; i++) {
        if (cells[i] == 0) return FALSE;
    }
    
    return TRUE;
}

boolean isTableValid (TABLE table) {
    
    int size = getTableSize(&table);
    
    int* cells = getTableCells(&table);
    
    int i;
    
    for (i = 0; i < size; i++) {
        if (cells[i] < 0 && cells[i] > 9) return FALSE;
    }
    
    return TRUE;
}

boolean checkRow (TABLE table, int row) {
    
    boolean digitChecks[9];
    
    int i;
    for (i = 0; i < 9; i++) digitChecks[i] = FALSE;
    
    int col;
    
    for (col = 0; col < TABLE_WIDTH; col++) {
        
        int val = getTableCell(&table, row, col);
        
        if (val == 0) continue;
        
        if (digitChecks[val-1] == FALSE) digitChecks[val-1] = TRUE;
        else return FALSE;        
    }
    
    return TRUE;
}

boolean checkCol (TABLE table, int col) {
    
    boolean digitChecks[9];
    
    int i;
    for (i = 0; i < 9; i++) digitChecks[i] = FALSE;
    
    int row;
    
    for (row = 0; row < TABLE_HEIGHT; row++) {
        
        int val = getTableCell(&table, row, col);
        
        if (val == 0) continue;
        
        if (digitChecks[val-1] == FALSE) digitChecks[val-1] = TRUE;
        else return FALSE;        
    }
    
    return TRUE;
}

boolean checkBox (TABLE table, int boxStartRow, int boxStartCol) {
    
    boolean digitChecks[9];
    
    int i;
    for (i = 0; i < 9; i++) digitChecks[i] = FALSE;
    
    int boxEndRow = boxStartRow + 3;
    int boxEndCol = boxStartCol + 3;
    
    int row, col;
    
    for (row = boxStartRow; row < boxEndRow; row++) {
        for (col = boxStartCol; col < boxEndCol; col++) {
            
            int val = getTableCell(&table, row, col);
            
            if (val == 0) continue;
            
            if (digitChecks[val-1] == FALSE) digitChecks[val-1] = TRUE;
            else return FALSE; 
        }
    }
    
    return TRUE;
}

boolean checkCell (TABLE table, int cellNum) {
    
    int row, col;
    
    row = cellNum / TABLE_HEIGHT;
    col = cellNum % TABLE_HEIGHT;
    
    if (!checkRow(table, row)) return FALSE;
    if (!checkCol(table, col)) return FALSE;
    if (!checkBox(table, (row/3)*3, (col/3)*3)) return FALSE;
    
    return TRUE;
}

boolean checkTable (TABLE table) {
    
    int i, j;
    
    for (i = 0; i < TABLE_HEIGHT; i++) {
        if (checkRow(table, i) == FALSE) return FALSE;
    }
    
    for (i = 0; i < TABLE_WIDTH; i++) {
        if (checkCol(table, i) == FALSE) return FALSE;
    }
    
    for (i = 0; i < TABLE_HEIGHT; i += 3) {
        for (j = 0; j < TABLE_WIDTH; j += 3) {
            if(checkBox(table, i, j) == FALSE) return FALSE;
        }
    }
    
    return TRUE;
}

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

int backTrackingTableSolution (TABLE* p_startTable, TABLE* p_destTable) {
    
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
