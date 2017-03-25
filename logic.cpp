#include "logic.h"
#include <fstream>
#include <windows.h>
#include <commdlg.h>
void init_desicion(desicion *p);
using namespace std;
extern int sudoku_board[9][9];
/* the desicion stack*/
stack <desicion> des_stack;
/* store previously selected x*/
stack <int> prev_x;
/* store previously selected y*/
stack <int> prev_y;

/* open the file */
BOOL open_file(HWND hwnd)
{
   OPENFILENAME ofn;
   fstream input_file;
   char szFileName[MAX_PATH];
   char ch;
   ZeroMemory(&ofn, sizeof(ofn));
   szFileName[0] = 0;

   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
   ofn.lpstrFile = szFileName;
   ofn.nMaxFile = MAX_PATH;
   ofn.lpstrDefExt = "txt";
   ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
   if(GetOpenFileName(&ofn))
   {
        input_file.open(szFileName,ios::in);
        
        for(int i = 0 ;i < 9 ;i++)
        {
           for(int j = 0 ; j < 9 ; j++)
           {
                   input_file>>ch;
                   sudoku_board[i][j] = ch - 48;
           }
        }   
        InvalidateRect(hwnd,NULL,TRUE); 
        input_file.close();
   } 
   return TRUE ;   
}
/* save the file*/
BOOL save_file(HWND hwnd)
{
   OPENFILENAME ofn;
   fstream output_file;
   char szFileName[MAX_PATH];
   char ch;
   ZeroMemory(&ofn, sizeof(ofn));
   szFileName[0] = 0;

   ofn.lStructSize = sizeof(ofn);
   ofn.hwndOwner = hwnd;
   ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
   ofn.lpstrFile = szFileName;
   ofn.nMaxFile = MAX_PATH;
   ofn.lpstrDefExt = "txt";
   ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
         OFN_OVERWRITEPROMPT;
   if(GetSaveFileName(&ofn))
   {
       output_file.open(szFileName,ios::out);
       for(int i = 0 ; i < 9 ;i++)
       {
           for(int j =0 ; j < 9 ; j++)
           {
                   output_file<<sudoku_board[i][j]<<"  ";
           }
           output_file<<"\n";
       }
       InvalidateRect(hwnd,NULL,TRUE);
       output_file.close();
    }                                       
    return TRUE;       
}
/* calculate the possible moves at x & y */
void calc_possible_numbers(desicion *p,int x,int y)
{
 int already_present[81];
 int cntr = 0;
 int index = 0;
 int u , v ;
 bool discard = false;
 init_desicion(p);
 for(int i = 0;i < 81 ;i++)
 {
         already_present[i] =0;
 }
 /* find all numbers present*/
 
 /* a) row-wise and column wise*/
 for(int k = 0 ; k < 9 ; k++)
 {
     if( sudoku_board[x][k] != 0  && k != y )
	 {
		 already_present[cntr++] = sudoku_board[x][k];
	 }
	 if( sudoku_board[k][y] != 0 &&  k != x)
	 {
		 already_present[cntr++] = sudoku_board[k][y];
	 }
 }
 u = ( x /3) * 3;
 v = ( y / 3) * 3;
 /* b) in the bounding rectangle*/
 for(int i =0 ; i < 3 ; i++)
 {
	 for(int j =0 ; j < 3 ;j++)
	 {

        if(sudoku_board[u+i][v+j] != 0 && !(( (u+i) == x ) && ( (v+j) == y)))
		{
			already_present[cntr++] = sudoku_board[i+u][j+v];
		}

	 }
 }
 
 /*store all the numbers not present into the array*/
 for(int k = 1 ; k <=9 ; k++)
 {
     for(int z= 0 ; z < cntr ;z++)
	 {
		 if(k == already_present[z])
		 {
			 discard = true;
			 break;
		 }
	  
	 }
	 if(!discard)
	 {
           discard = false;
           p->desicions[index++] = k;
	 }
	 discard = false;
  
 }
 
        
}
/* Simple brute - force like method for solving sudoku*/
void solve_sudoku(HWND hwnd)
{
     int x=0,y=0;
	desicion cur_desicion;
	init_desicion(&cur_desicion);
		
	while(1)
	{
		/* if last position is reached quit*/
        if((x > 8))break;
		if( sudoku_board[x][y] != 0 )
		{
			/* if aready marked .. bypass*/
			
			y++;
			if( y > 8 )
			{
				y -= 9;
				x++;
			}
			if((x == 8 && y == 8))break;
		}
		else
		{
            /* else calculate possible numbers*/
			calc_possible_numbers(&cur_desicion , x ,y );
			/* if none found & stack empty -- no solution*/
            if(cur_desicion.desicions[cur_desicion.position] == 0 && des_stack.empty())
			{
			    
                MessageBox(NULL,"No Solution!!","Sorry",MB_OK);
				exit(0);
			}
			else
			{
				/* if stack not empty ... go back and change  the previous desicion(s)*/
                if(cur_desicion.desicions[cur_desicion.position] ==0 && !des_stack.empty())
				{
					
				    sudoku_board[x][y] = 0;
                    x = prev_x.top();
					prev_x.pop();
					y = prev_y.top();
					prev_y.pop();
					cur_desicion = des_stack.top();
					cur_desicion.position++;
					des_stack.pop();
				    while(((cur_desicion.position >= 9) || (cur_desicion.desicions[cur_desicion.position] == 0))&& !des_stack.empty())
					{ 
						
                            sudoku_board[x][y] = 0;
						    x = prev_x.top();
							prev_x.pop();
						    y = prev_y.top();
							
							prev_y.pop();
							
							cur_desicion = des_stack.top();
							cur_desicion.position++; 
							des_stack.pop();
					}
					
                    /* still none found .. then no solution*/
                    if(((cur_desicion.position  >= 9 )|| (cur_desicion.desicions[cur_desicion.position]==0))&&des_stack.empty())
					{
						 
                          
                          MessageBox(NULL,"No Solution!","Sorry",MB_OK);
						  exit(0);
					}
					else
				
                    {
							 
						  /* mark current desicion .. save it and proceed*/
						  
                           sudoku_board[x][y] = cur_desicion.desicions[cur_desicion.position];
			              
			               //MessageBox(NULL,"hi","hi",MB_OK);
			               
                           if((x > 8))break;
						   prev_x.push(x);
						   prev_y.push(y);
						   
						   des_stack.push(cur_desicion);
						   y++;
						   if( y > 8 ) 
						   {
							   y -= 9;
							   x++;
						   }
						   if((x>8))break;

						  
					}
					
               				
				}
				else 
				{
					/* some valid desicions exists ... then try the first .. save & proceed*/
                    
					sudoku_board[x][y] = cur_desicion.desicions[cur_desicion.position];
					
                    if((x>8))break;
					des_stack.push(cur_desicion);
                    prev_x.push(x);
					prev_y.push(y);
					y++;
					if( y > 8 ) 
					{
						y -= 9;
						x++;
					}
					
				
				}
			}
		}
	}

}
/* helper routine for storing the desicions*/
void init_desicion(desicion *p)
{
	for(int i = 0 ; i< 10;i++)
	{
		p->desicions[i] = 0;
	}
	p->position = 0;
}
