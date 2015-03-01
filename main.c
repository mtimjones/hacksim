
#include <stdio.h>
#include "thread.h"

#define MAX_ENTITIES	3

int main ( int argc, char *argv[] )
{
   int i, j;
   thread_t *entities[ MAX_ENTITIES ];

   instruction_t prog1[ ] = { INSTR_FMT1( INCX ), INSTR_FMT3( BXNE, 2, 0 ),
                              INSTR_FMT1( INCY ), INSTR_FMT3( BYNE, 2, 2 ),
                              INSTR_FMT1( DECX ), INSTR_FMT3( BXNE, 0, 4 ),
                              INSTR_FMT1( DECY ), INSTR_FMT3( BYNE, 0, 6 ),
                              INSTR_FMT2( JUMP, 0 ) };

   instruction_t prog2[ ] = { INSTR_FMT1( INCX ), INSTR_FMT1( INCX ),
                              INSTR_FMT1( INCY ), INSTR_FMT1( INCY ),
                              INSTR_FMT1( DECX ), INSTR_FMT1( DECX ),
                              INSTR_FMT1( DECY ), INSTR_FMT1( DECY ),
                              INSTR_FMT2( JUMP, 0 ) };

   instruction_t prog3[ ] = { INSTR_FMT2( JUMP, 0 ) };

   extern void display_environment( thread_t **, int );

   entities[ 0 ] = thread_create( );
   entities[ 0 ]->load( entities[ 0 ], Avatar, prog2, sizeof( prog2 ), 0, 0 );

   entities[ 1 ] = thread_create( );
   entities[ 1 ]->load( entities[ 1 ], Virus,  prog2, sizeof( prog2 ), 5, 5 );

   entities[ 2 ] = thread_create( );
   entities[ 2 ]->load( entities[ 2 ], Goal,   prog3, sizeof( prog3 ), 9, 9 );

   for ( i = 0 ; i < 100 ; i++ )
   {

      for ( j = 0 ; j < MAX_ENTITIES ; j++ )
      {
   
//         printf("Entity %d -- PC: %4d, CYC: %6d, X: %4d, Y: %4d\n",
//                  entities[ j ]->entity,
//                  entities[ j ]->registers.pc, entities[ j ]->registers.cyc, 
//                  entities[ j ]->registers.x,  entities[ j ]->registers.y );

         entities[ j ]->step( entities[ j ] );

      }

      display_environment( entities, MAX_ENTITIES );

   }

   for ( j = 0 ; j < MAX_ENTITIES ; j++ )
   {
      thread_destroy( entities[ j ] );
   }

   return 0;
}


// Game Loop

// Pre-game, show environment, user defines avatar's program.

// Start-game, interpret each of the entities

//    Iterate entities to interp
//    Iterate entities to check for collision
//    Draw environment
