
#include <stdio.h>
#include "thread.h"

int main ( int argc, char *argv[] )
{
   int i;
   thread_t *avatar_thread;

   instruction_t aprog[ ] = { INSTR_FMT1( INCX ), INSTR_FMT3( BXNE, 3, 0 ),
                              INSTR_FMT1( INCY ), INSTR_FMT3( BYNE, 3, 2 ),
                              INSTR_FMT1( DECX ), INSTR_FMT3( BXNE, 0, 4 ),
                              INSTR_FMT1( DECY ), INSTR_FMT3( BYNE, 0, 6 ),
                              INSTR_FMT2( JUMP, 0 ) };

   avatar_thread = thread_create( );
   avatar_thread->load( avatar_thread, Avatar, aprog, sizeof( aprog ), 0, 0 );

   for ( i = 0 ; i < 100 ; i++ )
   {
      printf("PC: %4d, CYC: %6d, X: %4d, Y: %4d\n",
               avatar_thread->registers.pc, avatar_thread->registers.cyc, 
               avatar_thread->registers.x,  avatar_thread->registers.y );
      avatar_thread->step( avatar_thread );
   }

   return 0;
}

