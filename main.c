
#include <stdio.h>
#include "thread.h"

#define MAX_ENTITIES	2

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

   entities[ 0 ] = thread_create( );
   entities[ 0 ]->load( entities[ 0 ], Avatar, prog2, sizeof( prog2 ), 0, 0 );

   entities[ 1 ] = thread_create( );
   entities[ 1 ]->load( entities[ 1 ], Virus,  prog2, sizeof( prog2 ), 2, 2 );

   for ( i = 0 ; i < 100 ; i++ )
   {

      for ( j = 0 ; j < MAX_ENTITIES ; j++ )
      {
   
         printf("Entity %d -- PC: %4d, CYC: %6d, X: %4d, Y: %4d\n",
                  entities[ j ]->entity,
                  entities[ j ]->registers.pc, entities[ j ]->registers.cyc, 
                  entities[ j ]->registers.x,  entities[ j ]->registers.y );

         entities[ j ]->step( entities[ j ] );

      }

//      display_environment( );

   }

//   thread_destroy( avatar_thread );
   return 0;
}

