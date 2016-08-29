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
#include <stdlib.h>
#include <string.h>

#include "Defs.h"

#define TABLE_PRINT_BUF_LEN (TABLE_HEIGHT*(TABLE_WIDTH*2+1)+1)

void initTable (TABLE* p_table) {
    int size = TABLE_WIDTH*TABLE_HEIGHT+1;
    p_table->cells = (int*) malloc (sizeof(int)*size);
    memset(p_table->cells, 0, size);
    p_table->cells[size-1] = -1;
}

void freeTable (TABLE* p_table) {
    free(p_table->cells);
}

int getTableSize (TABLE* p_table) {
    int i;
    for (i = 0; p_table->cells[i] != -1; i++);
    return i;
}

int getTableCell (TABLE* p_table, int row, int col) {
    return p_table->cells[row*TABLE_HEIGHT+col];
}

void setTableCell (TABLE* p_table, int row, int col, int val) {
    p_table->cells[row*TABLE_HEIGHT+col] = val;
}

int* getTableCells (TABLE* p_table) {
    return p_table->cells;
}

char* tableToString (TABLE table) {
    
    int i, j, bufIndex;
    
    char* buffer = (char*) malloc (sizeof(char)*TABLE_PRINT_BUF_LEN);
    
    buffer[TABLE_PRINT_BUF_LEN-1] = '\0';
    bufIndex = 0;
    
    for (i = 0; i < TABLE_HEIGHT; i++) {
        for (j = 0; j < TABLE_WIDTH; j++) {
            buffer[bufIndex] = getTableCell(&table, i, j) + '0';
            buffer[bufIndex+1] = ' ';
            bufIndex += 2;
        }
        buffer[bufIndex] = '\n';
        bufIndex++;
    }
    
    return buffer;
}

void printTable (TABLE table) {
    fputs(tableToString(table), stdout);
}

void copyTable (TABLE* p_srcTable, TABLE* p_destTable) {
    
    int size = getTableSize(p_srcTable);
    
    int i;
    
    for (i = 0; i < size; i++) {
        p_destTable->cells[i] = p_srcTable->cells[i];
    }
}
