// screen.c

#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <sys/time.h>

#define NLINES          32
#define NCOLS           70

static WINDOW *mainwin;
static MEVENT event;
int x=0, y=0;
int offsety, offsetx;


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

   mvwprintw( mainwin, 2, 20, "y%3d, x%3d", y, x );

   wrefresh( mainwin );

   return;
}


int game_loop( void )
{
   int c;

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
            }
         }
      }
      else if ( c == 27 )
      {
         break;
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
