#ifndef _LOGIC_H_
#define _LOGIC_H_   
#include <windows.h>
#include <stack>

/* the structure for storing desicion at each move*/
struct desicion
{
	 int desicions[10];
	 int position;
};
extern int sudoku_board[9][9];
    
extern BOOL open_file(HWND hwnd);
extern void calc_possible_numbers(desicion *p,int x,int y);
extern void solve_sudoku(HWND hwnd);
extern BOOL save_file(HWND hwnd);    
     
#endif
