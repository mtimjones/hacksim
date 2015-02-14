// thread.h

typedef struct thread_s thread_t;

typedef enum
{
   UNDEF = 0x00,
   NOP   = 0x11,
   INCX  = 0x22,
   INCY  = 0x32,
   DECX  = 0x42,
   DECY  = 0x52,
   SWAP  = 0x64,
   JUMP  = 0x74,
   BXNE  = 0x84,
   BYNE  = 0x94,
} opcodes_t;

typedef unsigned short instruction_t;

#define GET_OPCODE( x )    ( x >> 8 )
#define GET_CYCLES( x )    ( GET_OPCODE( x ) & 0x0f )
#define GET_OPERAND( x )   ( x & 0xff )

#define INSTR( opcode, operand )   \
         ( instruction_t )( ( opcode << 8 ) | operand )

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



