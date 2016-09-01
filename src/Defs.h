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

#define TABLE_WIDTH 9
#define TABLE_HEIGHT TABLE_WIDTH

#define BACKTRACKREC 0
#define BACKTRACKING 1
#define BRUTEFORCE 2

typedef enum { FALSE, TRUE } boolean;

typedef struct {
    int* cells;
} TABLE;

typedef struct {
    int algorithm;
    boolean verbose;
    const char* programName;
} CONF;


extern CONF config;
extern char* targetFilename;


// Algos.c
int bruteForceTableSolution (TABLE* p_startTable, TABLE* p_destTable);
int backTrackTableSolution (TABLE* p_startTable, TABLE* p_destTable);
int backTrackTableSolutionRecursive (TABLE* p_startTable, TABLE* p_destTable);

// Args.c
int parseConfFromArgs (int argc, char** argv);
void printUsage (FILE* stream);

// File.c
int fetchTableFromFile (const char* fileName, TABLE* p_table);

// Solve.c
boolean isTableComplete (TABLE table);
boolean isTableValid (TABLE table);
boolean checkRow (TABLE table, int row);
boolean checkCol (TABLE table, int col);
boolean checkBox (TABLE table, int boxStartRow, int boxStartCol);
boolean checkCell (TABLE table, int cellNum);
boolean checkTable (TABLE table);


// Table.c
void initTable (TABLE* p_table);
void freeTable (TABLE* p_table);
int getTableSize (TABLE* p_table);
int getTableCell (TABLE* p_table, int row, int col);
void setTableCell (TABLE* p_table, int row, int col, int val);
int* getTableCells (TABLE* p_table);
void printTable (TABLE table);
void copyTable (TABLE* p_srcTable, TABLE* p_destTable);
char* tableToString (TABLE table);
