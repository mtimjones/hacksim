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
      else                 thread->instructions[ i ] = INSTR( UNDEF, 0 );
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


thread_t *thread_create( void )
{
   thread_t *new_thread;

   new_thread = (thread_t *)malloc( sizeof( thread_t ) );

   new_thread->load = load_function;
   new_thread->step = step_function;

   return new_thread;
}


int interp( thread_t *thread )
{
   unsigned char opcode;
   char operand;
   int  next = 1;

   assert( thread );

   opcode = GET_OPCODE( thread->instructions[ thread->registers.pc ] );
   operand = (char)GET_OPERAND( thread->instructions[ thread->registers.pc ] );

printf("opcode = %x\n", opcode);

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
printf("operand = %d\n", operand);
         thread->registers.pc = operand;
         next = 0;
         break;
      case BXNE:
         if ( thread->registers.x == operand )
         {
            thread->registers.pc++;
         }
         else
         {
            thread->registers.pc += operand;
         }
         next = 0;
         break;
      case BYNE:
         if ( thread->registers.y == operand )
         {
            thread->registers.pc++;
         }
         else
         {
            thread->registers.pc += operand;
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
