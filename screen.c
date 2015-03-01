// screen.c

#include <stdio.h>
#include <curses.h>

#define NLINES          32
#define NCOLS           70

static WINDOW *mainwin;

void win_startup( void )
{
   int offsety, offsetx;

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

   wborder( mainwin, 0, 0, 0, 0, 0, 0, 0, 0 );

   wrefresh( mainwin );

   return;
}


void win_shutdown( void )
{
   delwin( mainwin );

   endwin( );

   return;
}


void win_update( void )
{
   int i;

   // Update the window.
   mvwprintw( mainwin, 1, 1, "/Hack/Sim" );

   for ( i = 0 ; i < 19 ; i++ )
   {
      if ( ( i % 3 ) == 0 )
      {
         mvwprintw( mainwin, 4+i, 5, "+-----+-----+-----+-----+-----+-----+-----+" );
      }
      else
      {
         mvwprintw( mainwin, 4+i, 5, "|     |     |     |     |     |     |     |" );
      }
   }

   wrefresh( mainwin );
   
   return;
}


int main( int argc, char *argv[] )
{

   win_startup( );

   win_update( );
   sleep( 5 );
//   while ( 1 )
//   {
//
//   }

   win_shutdown( );

   return 0;
}
