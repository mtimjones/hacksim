// screen.c

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include "screen.h"

#define NLINES          34
#define NCOLS           70

static WINDOW *mainwin;
static MEVENT event;
int x=0, y=0;
int offsety, offsetx;


static int helpindex = 0;
static int contexthelpindex = 0;

char *help[5] = {
"Welcome.  Continue to click Help for more help.           ",
"Develop a program to guide your avatar to the shown goals.",
"Commands: Run your program, Reset the env, Exit the sim.  ",
"Click a location, and then an instruction to load it.     ",
};

char *contexthelp[4] = {
"                                                          ",
"Now select an instruction for that location.              ",
"For this instruction, select a number for the operand.    ",
"For this instruction, select another branch location.     ",
};

typedef struct
{
  int y;
  int x1;
  int x2;
} area;


area Areas[] = 
{
   [ ACTION_RUN   ] = { 25, 26, 28 },
   [ ACTION_EXIT  ] = { 26, 26, 29 },
   [ ACTION_RESET ] = { 25, 31, 35 },
   [ ACTION_HELP  ] = { 26, 31, 34 },
   [ INSTR00      ] = {  5, 51, 60 },
   [ INSTR01      ] = {  6, 51, 60 },
   [ INSTR02      ] = {  7, 51, 60 },
   [ INSTR03      ] = {  8, 51, 60 },
   [ INSTR04      ] = {  9, 51, 60 },
   [ INSTR05      ] = { 10, 51, 60 },
   [ INSTR06      ] = { 11, 51, 60 },
   [ INSTR07      ] = { 12, 51, 60 },
   [ INSTR08      ] = { 13, 51, 60 },
   [ INSTR09      ] = { 14, 51, 60 },
   [ INSTR10      ] = { 15, 51, 60 },
   [ INSTR11      ] = { 16, 51, 60 },
   [ INSTR12      ] = { 17, 51, 60 },
   [ INSTR13      ] = { 18, 51, 60 },
   [ INSTR14      ] = { 19, 51, 60 },
   [ INSTR15      ] = { 20, 51, 60 },
   [ INSTR16      ] = { 21, 51, 60 },
   [ INSTR_INCX   ] = { 25, 51, 54 },
   [ INSTR_DECX   ] = { 26, 51, 54 },
   [ INSTR_JUMP   ] = { 27, 51, 54 },
   [ INSTR_INCY   ] = { 25, 56, 59 },
   [ INSTR_DECY   ] = { 26, 56, 59 },
   [ INSTR_BXNE   ] = { 27, 56, 59 },
   [ INSTR_SWAP   ] = { 25, 61, 65 },
   [ INSTR_NOP    ] = { 26, 61, 65 },
   [ INSTR_BYNE   ] = { 27, 61, 65 },
   [ NUM_1        ] = { 25, 41, 41 },
   [ NUM_2        ] = { 25, 43, 43 },
   [ NUM_3        ] = { 25, 45, 45 },
   [ NUM_4        ] = { 26, 41, 41 },
   [ NUM_5        ] = { 26, 43, 43 },
   [ NUM_6        ] = { 26, 45, 45 },
   [ NUM_7        ] = { 27, 41, 41 },
   [ NUM_8        ] = { 27, 43, 43 },
   [ NUM_9        ] = { 27, 45, 45 },
   [ NUM_0        ] = { 28, 43, 43 },

};



int determine_screen_region( int x, int y )
{
   int i;

   for ( i = 0 ; i < ( sizeof( Areas ) / sizeof( area ) ) ; i++ )
   {
      if ( ( y == Areas[i].y ) && ( x >= Areas[i].x1 ) && ( x <= Areas[i].x2 ) )
      {
         return i;
      }
   }

   return NONE;
}


void win_startup( void )
{

   initscr( );
   cbreak( );
   noecho( );
   curs_set( 0 );
   nonl( );

   offsety = ( LINES - NLINES ) / 2;
   offsetx = ( COLS - NCOLS ) / 2;

   mainwin = newwin( NLINES, NCOLS, offsety, offsetx );
   nodelay( mainwin, TRUE );
   keypad( mainwin, TRUE );

   mousemask( BUTTON1_CLICKED, NULL );

   return;
}


void win_shutdown( void )
{
   delwin( mainwin );

   endwin( );

   mousemask( 0, NULL );

   return;
}


void win_update( void )
{
   int i;

   wborder( mainwin, 0, 0, 0, 0, 0, 0, 0, 0 );

   // Update the window.
   mvwprintw( mainwin, 1, 1, "/Hack/Sim" );

   mvwprintw( mainwin,  3, 6, "   0     1     2     3     4     5" );
   mvwprintw( mainwin,  5, 3, "0" );
   mvwprintw( mainwin,  8, 3, "1" );
   mvwprintw( mainwin, 11, 3, "2" );
   mvwprintw( mainwin, 14, 3, "3" );
   mvwprintw( mainwin, 17, 3, "4" );
   mvwprintw( mainwin, 20, 3, "5" );

   for ( i = 0 ; i < 19 ; i++ )
   {
      if ( ( i % 3 ) == 0 )
      {
         mvwprintw( mainwin, 4+i, 6, "+-----+-----+-----+-----+-----+-----+" );
      }
      else
      {
         mvwprintw( mainwin, 4+i, 6, "|     |     |     |     |     |     |" );
      }
   }

   mvwprintw( mainwin,  4, 51, "+--------+" );

   for ( i = 0 ; i < 17 ; i++ ) 
   {
      mvwprintw( mainwin, 5+i, 48, "%02d |        |", i );
   }

   mvwprintw( mainwin, 22, 51, "+--------+" );

   mvwprintw( mainwin, 3, 50, "y%3d, x%3d", y, x );

   mvwprintw( mainwin, 24, 6, "Registers:          Actions:   "
                              "   Numbers    Instructions" );

   mvwprintw( mainwin, 25,  6, "X:      PC:" );
   mvwprintw( mainwin, 26,  6, "Y:      CYC:" );
   mvwprintw( mainwin, 25, 26, "Run  Reset" );
   mvwprintw( mainwin, 26, 26, "Exit Help" );
   mvwprintw( mainwin, 25, 41, "1 2 3" );
   mvwprintw( mainwin, 26, 41, "4 5 6" );
   mvwprintw( mainwin, 27, 41, "7 8 9" );
   mvwprintw( mainwin, 28, 41, "  0  " );
   mvwprintw( mainwin, 25, 51, "INCX" );
   mvwprintw( mainwin, 25, 56, "INCY" );
   mvwprintw( mainwin, 25, 61, "SWAP" );
   mvwprintw( mainwin, 26, 51, "DECX" );
   mvwprintw( mainwin, 26, 56, "DECY" );
   mvwprintw( mainwin, 26, 61, "NOP" );
   mvwprintw( mainwin, 27, 51, "JUMP" );
   mvwprintw( mainwin, 27, 56, "BXNE" );
   mvwprintw( mainwin, 27, 61, "BYNE" );

   mvwprintw( mainwin, 30,  6, help[ helpindex ] );

   mvwprintw( mainwin, 31,  6, contexthelp[ contexthelpindex ] );

   wrefresh( mainwin );

   return;
}


int game_loop( void )
{
   int c;
   int selection;
   states state = INITIAL;
   int selected_location;
   int selected_target_location;

   // Load environment (entities)

   while ( 1 )
   {
      c = wgetch( mainwin );

      if ( c == KEY_MOUSE )
      {
         if ( getmouse( &event ) == OK )
         {
            if ( event.bstate & BUTTON1_CLICKED )
            {
               y = event.y - offsety;
               x = event.x - offsetx;
               selection = determine_screen_region( x, y );

               if ( selection == ACTION_EXIT )
               {
                  break;
               }
               else if ( selection == ACTION_HELP )
               {
                  if ( ++helpindex >= 4 ) helpindex = 0;
               }
               else if ( selection == ACTION_RUN )
               {
               }
               else if ( selection == ACTION_RESET )
               {
               }
               else if ( selection >= INSTR00 && selection <= INSTR16 )
               {
                  if ( state == INITIAL )
                  {
                     selected_location = selection;
                     contexthelpindex = 1;
                  }
                  else if ( state == LOCATION )
                  {
                  }
               }
            }
         }
      }

      win_update( );
   }

   return 0;
}


int main( int argc, char *argv[] )
{
   win_startup( );

   game_loop( );

   win_shutdown( );

   return 0;
}
