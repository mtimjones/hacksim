// thread.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "thread.h"

int load_function( thread_t *thread, entity_t entity, 
                   instruction_t  *instructions, unsigned int num_instr,
                   unsigned int x, unsigned int y )
{
   int i;

   thread->entity = entity;

   // Initialize thread's registers
   thread->registers.x   = x;
   thread->registers.y   = y;
   thread->registers.pc  = 0;
   thread->registers.cyc = 0;
   thread->registers.cur_instr_cyc = 0;
   thread->registers.err = 0;

   // Load the user's program
   for ( i = 0 ; i < MAX_INSTRUCTIONS ; i++)
   {
      if ( i < num_instr ) thread->instructions[ i ] = instructions[ i ];
      else                 thread->instructions[ i ] = INSTR_FMT1( UNDEF );
   }

   return 0;
}


int step_function( thread_t *thread )
{
   int next;

   int interp( thread_t *thread );

   thread->registers.cyc++;

   if ( thread->registers.cur_instr_cyc == 0 )
   {
      // Schedule a new instruction
      assert ( GET_OPCODE ( thread->instructions[ thread->registers.pc ] ) != UNDEF );
      thread->registers.cur_instr_cyc = GET_CYCLES( thread->instructions[ thread->registers.pc ] );
   }

   thread->registers.cur_instr_cyc--;

   if ( thread->registers.cur_instr_cyc == 0 )
   {
      // Execute the instruction
      next = interp( thread );

      thread->registers.cur_instr_cyc = 0;

      if ( next )
      {
         thread->registers.pc++;
      }
   }

   return 0;
}


void get_pos_function( thread_t *thread, int *x, int *y )
{
   assert( thread );

   *x = thread->registers.x;
   *y = thread->registers.y;

   return;
}


int get_err_function( thread_t *thread )
{
   assert( thread );

   return( thread->registers.err );
}


thread_t *thread_create( void )
{
   thread_t *new_thread;

   new_thread = (thread_t *)malloc( sizeof( thread_t ) );

   new_thread->load = load_function;
   new_thread->step = step_function;
   new_thread->get_pos = get_pos_function;
   new_thread->get_err = get_err_function;

   return new_thread;
}


int interp( thread_t *thread )
{
   opcodes_t opcode;
   char operand1;
   char operand2;
   int  next = 1;

   assert( thread );

   opcode = GET_OPCODE( thread->instructions[ thread->registers.pc ] );
   operand1 = (char)GET_OPERAND1( thread->instructions[ thread->registers.pc ] );
   operand2 = (char)GET_OPERAND2( thread->instructions[ thread->registers.pc ] );

   // Interpret the instruction
   switch( opcode )
   {
      case UNDEF:
         break;
      case NOP:
         break;
      case INCX:
         thread->registers.x++;
         break;
      case INCY:
         thread->registers.y++;
         break;
      case DECX:
         thread->registers.x--;
         break;
      case DECY:
         thread->registers.y--;
         break;
      case SWAP:
         {
            unsigned int temp;
            temp = thread->registers.x;
            thread->registers.x = thread->registers.y;
            thread->registers.y = temp;
         }
         break;
      case JUMP:
         thread->registers.pc = operand1;
         next = 0;
         break;
      case BXNE:
         if ( thread->registers.x == operand1 )
         {
            thread->registers.pc++;
         }
         else
         {
            thread->registers.pc = operand2;
         }
         next = 0;
         break;
      case BYNE:
         if ( thread->registers.y == operand1 )
         {
            thread->registers.pc++;
         }
         else
         {
            thread->registers.pc = operand2;
         }
         next = 0;
         break;
      default:
         assert( 0 );
         break;
   }

   // Error checking
   if ( thread->registers.pc >= MAX_INSTRUCTIONS )
   {
      thread->registers.err &= ADDRESS_FAULT;
   }

   return next;
}
