 #include <windows.h>
#include <commdlg.h>
#include "Globals.h"
#include "Resource.h"
#include "Logic.h"
void init_sudoku_board();

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use custom icon and default mouse-pointer */
    wincl.hIcon = LoadIcon (GetModuleHandle(NULL),"IDI_SUDOKUICON");
    wincl.hIconSm = (HICON)LoadImage (GetModuleHandle(NULL),"IDI_SUDOKUICON",IMAGE_ICON,16,16,0);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = "IDR_SUDOKUMENU";                 /* Sudokus menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH)GetStockObject(BLACK_PEN);
    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "WinSudoku",       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int len_x ,len_y;
    HDC hdc;
    HPEN hPen;
    HPEN hPen2;
    HFONT hFont;
    int fontHeight;
    PAINTSTRUCT ps;
    char display_str[2];
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
             init_sudoku_board();
             break;
        case WM_DESTROY:
            DeleteObject(hPen);
            DeleteObject(hPen2);
            DeleteObject(hFont);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_SIZE:
              client_x = LOWORD(lParam);
              client_y = HIWORD(lParam);  
              len_x = client_x / 8 ;
              len_y = client_y / 8;
              
              break;
        case WM_PAINT:
              /* drawing stuff goes here ...*/
              InvalidateRect(hwnd,NULL,TRUE);   
              hdc = BeginPaint(hwnd,&ps);
              hPen = CreatePen(PS_SOLID,9,RGB(255,0,0));
              hPen2 = CreatePen(PS_SOLID,2,RGB(255,0,0));
              
              SelectObject(hdc,hPen2);
              len_x = client_x /9 ;
              len_y = client_y / 9;
              fontHeight = len_y - 12 ;
              for(int j=1 ; j <= 8 ; j++)
              {
                  MoveToEx(hdc,0,j*len_y,NULL);
                  LineTo(hdc,client_x,j*len_y);
                  MoveToEx(hdc,j*len_x,0,NULL);
                  LineTo(hdc,j*len_x,client_y);
              
              
              }
              SelectObject(hdc,hPen);
              MoveToEx(hdc,0,client_y/3,NULL);
              LineTo(hdc,client_x,client_y/3); 
              MoveToEx(hdc,0,2*client_y/3,NULL);
              LineTo(hdc,client_x,2*client_y/3);
              MoveToEx(hdc,client_x/3,0,NULL);
              LineTo(hdc,client_x/3,client_y);
              MoveToEx(hdc,client_x*2/3,0,NULL);
              LineTo(hdc,client_x*2/3,client_y);        
              hFont = CreateFont(fontHeight, 0, 0, 0, 0, TRUE, 0, 0, 0, 0,0, 0, 0, "Times New Roman");
              SelectObject(hdc,hFont);
              SetTextColor(hdc,RGB(0,0,255));
              SetBkColor(hdc,RGB(0,0,0));
              for(int i = 0 ; i < 9 ; i++)
              {
                      for(int j =0 ; j < 9 ; j++)
                      {
                        display_str[0] = sudoku_board[i][j] + 48;
                        display_str[1] = '\0';
                        TextOut(hdc,j*len_x+9,i*len_y+9,display_str,1);
                      }
              }        
              EndPaint(hwnd,&ps);
             break;
        
                    
             
        case WM_COMMAND:
             switch(LOWORD(wParam))
             {
                case ID_FILE_OPEN:
                                  open_file(hwnd);
                                  break;
                case ID_FILE_SAVE:
                                  save_file(hwnd);
                                  break;
                
                case ID_FILE_SOLVE:
                                   solve_sudoku(hwnd);
                                   InvalidateRect(hwnd,NULL,FALSE);
                                   break;
                case ID_FILE_EXIT:
                                  PostQuitMessage(0);
                                  break;
                case ID_HELP_ABOUT:
                                   MessageBox(NULL,"Sudoku Solver App \nCreated by Sandeep Mathew \nEnjoy!!!",
                                   "About",MB_ICONINFORMATION|MB_OK);
                                   break; 
             } 
             break;         
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
 void init_sudoku_board()
{
     for(int i = 0 ; i < 9 ;i++)
      for(int j =0 ; j < 9;j++)
         sudoku_board[i][j] = 0;
} 
