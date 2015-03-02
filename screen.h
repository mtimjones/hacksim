// screen.h

#ifndef __SCREEN_H__
#define __SCREEN_H__

// Screen Regions
typedef enum
{
   NONE=0,
   INSTR00,
   INSTR01,
   INSTR02,
   INSTR03,
   INSTR04,
   INSTR05,
   INSTR06,
   INSTR07,
   INSTR08,
   INSTR09,
   INSTR10,
   INSTR11,
   INSTR12,
   INSTR13,
   INSTR14,
   INSTR15,
   INSTR16,
   ACTION_RUN,
   ACTION_RESET,
   ACTION_EXIT,
   NUM_1,
   NUM_2,
   NUM_3,
   NUM_4,
   NUM_5,
   NUM_6,
   NUM_7,
   NUM_8,
   NUM_9,
   NUM_0,
   INSTR_INCX,
   INSTR_INCY,
   INSTR_SWAP,
   INSTR_DECX,
   INSTR_DECY,
   INSTR_NOP,
   INSTR_JUMP,
   INSTR_BXNE,
   INSTR_BYNE,
} screen_regions;

#endif // __SCREEN_H__
