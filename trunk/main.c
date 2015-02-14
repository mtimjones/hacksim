
#include <stdio.h>
#include "thread.h"

int main ( int argc, char *argv[] )
{
   int i;
   thread_t *sample;
   instruction_t program[ 5 ] = { INSTR( INCX, 0 ), INSTR( INCY, 0 ),
                                  INSTR( INCX, 0 ), INSTR( INCY, 0 ),
                                  INSTR( JUMP, 0 ) };

   sample = thread_create( );
   sample->load( sample, Avatar, program, 5, 0, 0 );

   for ( i = 0 ; i < 50 ; i++ )
   {
      printf("PC: %4d, CYC: %6d, X: %4d, Y: %4d\n",
               sample->registers.pc, sample->registers.cyc, 
               sample->registers.x,  sample->registers.y );
      sample->step( sample );
   }

   return;
}

