// thread.h

typedef struct thread_s thread_t;

typedef enum {
   Avatar = 0,
   Goal,
   Trap,
   Data,
} entity_t;

typedef int ( *load_func_t )( thread_t *thread, entity_t entity, 
                              unsigned char *instructions,
                              unsigned int num_instr,
                              unsigned int x, unsigned int y );

typedef int ( *step_func_t )( thread_t *thread );

#define MAX_INSTRUCTIONS 22

typedef struct {
   unsigned int x;
   unsigned int y;
   unsigned int pc;
   unsigned int cyc;
} registers_t;

struct thread_s {
   load_func_t   load;
   step_func_t   step;
   registers_t   registers;
   unsigned char instructions[ MAX_INSTRUCTIONS ];
   entity_t      entity;
};


thread_t *thread_create( void );



