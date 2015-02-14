// thread.h

typedef struct thread_s thread_t;

typedef enum
{
   UNDEF = 0x0000,
   NOP   = 0x0101,
   INCX  = 0x0202,   // INCX
   INCY  = 0x0302,   // INCY
   DECX  = 0x0402,   // DECX
   DECY  = 0x0502,   // DECY
   SWAP  = 0x0604,   // SWAP
   JUMP  = 0x0704,   // JUMP <pc>
   BXNE  = 0x0804,   // BXNE <#> <pc>   | If X != #, JUMP <pc>
   BYNE  = 0x0904,   // BYNE <#> <pc>
} opcodes_t;

typedef unsigned int instruction_t;

#define GET_OPCODE( x )    (opcodes_t)( ( x >> 16 ) & 0xffff )
#define GET_CYCLES( x )    ( ( x >> 16 ) & 0xff   )
#define GET_OPERAND1( x )  ( ( x >> 8  ) & 0xff   )
#define GET_OPERAND2( x )  ( ( x       ) & 0xff   )

#define INSTR_FMT1( opcode )   \
                  ( instruction_t )( opcode << 16 )
#define INSTR_FMT2( opcode, op1 )   \
                  ( instruction_t )( ( opcode << 16 ) | ( op1 << 8 ) )
#define INSTR_FMT3( opcode, op1, op2 )   \
                  ( instruction_t )( ( opcode << 16 ) |  \
                                     ( op1 << 8 ) | ( op2 ) )

typedef enum {
   Avatar = 0,
   Goal,
   Trap,
   Data,
} entity_t;

typedef int ( *load_func_t )( thread_t *thread, entity_t entity, 
                              instruction_t *instructions,
                              unsigned int num_instr,
                              unsigned int x, unsigned int y );

typedef int ( *step_func_t )( thread_t *thread );

#define MAX_INSTRUCTIONS 22

typedef struct {
   unsigned int x;
   unsigned int y;
   unsigned int pc;
   unsigned int err;
   unsigned int cyc;
   unsigned int cur_instr_cyc;
} registers_t;

struct thread_s {
   load_func_t    load;
   step_func_t    step;
   registers_t    registers;
   instruction_t  instructions[ MAX_INSTRUCTIONS ];
   entity_t       entity;
};

// Faults
#define ADDRESS_FAULT    0x0001


thread_t *thread_create( void );



