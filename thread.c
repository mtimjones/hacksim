// thread.c

#include <stdio.h>
#include <stdlib.h>
#include "thread.h"

int load_function( thread_t *thread, entity_t entity, 
                   unsigned char *instructions, unsigned int num_instr,
                   unsigned int x, unsigned int y )
{
   int i;

   thread->entity = entity;

   // Initialize thread's registers
   thread->registers.x   = x;
   thread->registers.y   = y;
   thread->registers.pc  = 0;
   thread->registers.cyc = 0;

   // Load the user's program
   for ( i = 0 ; i < MAX_INSTRUCTIONS ; i++)
   {
      if ( i < num_instr ) thread->instructions[ i ] = instructions[ i ];
      else                 thread->instructions[ i ] = 0;
   }

   return 0;
}


int step_function( thread_t *thread )
{
   thread->registers.cyc++;
   printf( "Stepping %d (%d)\n", thread->entity, thread->registers.cyc );

   return 0;
}


thread_t *thread_create( void )
{
   thread_t *new_thread;

   new_thread = (thread_t *)malloc( sizeof( thread_t ) );

   new_thread->load = load_function;
   new_thread->step = step_function;

   return new_thread;
}

