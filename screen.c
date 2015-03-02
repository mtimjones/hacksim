// screen.c

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>
#include "screen.h"

#define NLINES          32
#define NCOLS           70

static WINDOW *mainwin;
static MEVENT event;
int x=0, y=0;
int offsety, offsetx;

typedef struct
{
  int y;
  int x1;
  int x2;
} area;


area Areas[] = 
{
   [ ACTION_EXIT ] = { 26, 26, 29 },
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
   mvwprintw( mainwin, 26, 26, "Exit" );
   mvwprintw( mainwin, 25, 41, "1 2 3" );
   mvwprintw( mainwin, 26, 41, "4 5 6" );
   mvwprintw( mainwin, 27, 41, "7 8 9" );
   mvwprintw( mainwin, 28, 41, "  0  " );
   mvwprintw( mainwin, 25, 51, "INCX INCY SWAP" );
   mvwprintw( mainwin, 26, 51, "DECX DECY NOP" );
   mvwprintw( mainwin, 27, 51, "JUMP BXNE BYNE" );

   wrefresh( mainwin );

   return;
}


int game_loop( void )
{
   int c;
   int selection;

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

               if ( selection == ACTION_EXIT ) break;
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
