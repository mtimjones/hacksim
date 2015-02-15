// thread.h

typedef struct thread_s thread_t;

typedef enum
{
   UNDEF = 0x0000,
   NOP   = 0x0101,
   INCX  = 0x0201,   // INCX
   INCY  = 0x0301,   // INCY
   DECX  = 0x0401,   // DECX
   DECY  = 0x0501,   // DECY
   SWAP  = 0x0601,   // SWAP
   JUMP  = 0x0701,   // JUMP <pc>
   BXNE  = 0x0801,   // BXNE <#> <pc>   | If X != #, JUMP <pc>
   BYNE  = 0x0901,   // BYNE <#> <pc>
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
   Avatar = 65,
   Goal = 71,
   Trap = 84,
   Data = 68,
   Virus = 86,
} entity_t;

typedef int ( *load_func_t )( thread_t *thread, entity_t entity, 
                              instruction_t *instructions,
                              unsigned int num_instr,
                              unsigned int x, unsigned int y );

typedef int ( *step_func_t )( thread_t *thread );

typedef void ( *get_pos_t )( thread_t *thread, int *x, int *y );

typedef int ( *get_err_t )( thread_t *thread );

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
   entity_t       entity;
   load_func_t    load;
   step_func_t    step;
   get_pos_t      get_pos;
   get_err_t      get_err;
   registers_t    registers;
   instruction_t  instructions[ MAX_INSTRUCTIONS ];
};

// Faults
#define ADDRESS_FAULT    0x0001


thread_t *thread_create( void );
void      thread_destroy( thread_t * );


