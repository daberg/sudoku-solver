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
#include <sys/stat.h>

#include "Defs.h"

boolean fileExists (const char* fileName) {
    struct stat buffer;
    return (stat(fileName, &buffer) == 0);
}

int fetchTableFromFile (const char* fileName, TABLE* p_table) {
    
    FILE* fp;
    
    if(!fileExists(fileName)) return -1;
    
    fp = fopen(fileName, "r");
    
    if (fp == NULL) return -1;
    
    char currCh;
    
    // Row and column counts start at 1 here
    
    int maxWidth = TABLE_WIDTH + 1; // +1 for an escape character
    int maxHeight = TABLE_HEIGHT;
    
    int currCol = 0;
    int currRow = 1;
    
    
    while ((currCh = fgetc(fp)) != EOF) {
        
        if (currRow > maxHeight) return -1;
        
        currCol++;
        
        if (currCol > maxWidth) return -1;
        
        if (currCh == '\n') {
            if (currCol != maxWidth) return -1;
            currCol = 0;
            currRow++;
        }
        
        else if (currCh >= '1' && currCh <= '9') {
            if (currCol == maxWidth) return -1;
            int val = currCh - '0';
            setTableCell(p_table, currRow-1, currCol-1, val);
        }
        
        else {
            if (currCol == maxWidth) return -1;
            setTableCell(p_table, currRow-1, currCol-1, 0);
        }
    }
    
    fclose(fp);
    
    if (currRow == maxHeight && currCol == maxWidth - 1)
        return 0;
    else if (currRow == maxHeight + 1 || currCol == maxWidth)
        return 0;
    else
        return -1;
}