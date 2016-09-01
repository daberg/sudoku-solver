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
