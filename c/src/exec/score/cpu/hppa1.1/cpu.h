/*  cpu.h
 *
 *  This include file contains information pertaining to the HP
 *  PA-RISC processor (Level 1.1).
 *
 *  COPYRIGHT (c) 1994 by Division Incorporated
 *
 *  To anyone who acknowledges that this file is provided "AS IS"
 *  without any express or implied warranty:
 *      permission to use, copy, modify, and distribute this file
 *      for any purpose is hereby granted without fee, provided that
 *      the above copyright notice and this notice appears in all
 *      copies, and that the name of Division Incorporated not be
 *      used in advertising or publicity pertaining to distribution
 *      of the software without specific, written prior permission.
 *      Division Incorporated makes no representations about the
 *      suitability of this software for any purpose.
 *
 *
 * Note:
 *      This file is included by both C and assembler code ( -DASM )
 *
 *  $Id$
 */

#ifndef __CPU_h
#define __CPU_h

#ifdef __cplusplus
extern "C" {
#endif

#include <rtems/score/hppa.h>              /* pick up machine definitions */
#ifndef ASM
#include <rtems/score/hppatypes.h>
#endif

/* conditional compilation parameters */

#define CPU_INLINE_ENABLE_DISPATCH       FALSE
#define CPU_UNROLL_ENQUEUE_PRIORITY      TRUE

/*
 *  RTEMS manages an interrupt stack in software for the HPPA.
 */

#define CPU_HAS_SOFTWARE_INTERRUPT_STACK TRUE
#define CPU_HAS_HARDWARE_INTERRUPT_STACK FALSE
#define CPU_ALLOCATE_INTERRUPT_STACK     TRUE

/*
 *  HPPA has hardware FP, it is assumed to exist by GCC so all tasks
 *  may implicitly use it (especially for integer multiplies).  Because
 *  the FP context is technically part of the basic integer context
 *  on this CPU, we cannot use the deferred FP context switch algorithm.
 */

#define CPU_HARDWARE_FP                  TRUE
#define CPU_ALL_TASKS_ARE_FP             TRUE
#define CPU_IDLE_TASK_IS_FP              FALSE
#define CPU_USE_DEFERRED_FP_SWITCH       FALSE

#define CPU_PROVIDES_IDLE_THREAD_BODY    FALSE
#define CPU_STACK_GROWS_UP               TRUE
#define CPU_STRUCTURE_ALIGNMENT          __attribute__ ((__aligned__ (32)))

/* constants */

#define CPU_MODES_INTERRUPT_LEVEL  0x00000001 /* interrupt level in mode */
#define CPU_MODES_INTERRUPT_MASK   0x00000001 /* interrupt level in mode */

/*
 * PSW contstants
 */

#define CPU_PSW_BASE (HPPA_PSW_C | HPPA_PSW_Q | HPPA_PSW_P | HPPA_PSW_D)
#define CPU_PSW_INTERRUPTS_ON    (CPU_PSW_BASE | HPPA_PSW_I)
#define CPU_PSW_INTERRUPTS_OFF   (CPU_PSW_BASE)

#define CPU_PSW_DEFAULT     CPU_PSW_BASE


#ifndef ASM

/*
 * Contexts
 *
 *  This means we have the following context items:
 *    1. task level context stuff::  Context_Control
 *    2. floating point task stuff:: Context_Control_fp
 *
 *  The PA-RISC is very fast so the expense of saving an extra register
 *  or two is not of great concern at the present.  So we are not making
 *  a distinction between what is saved during a task switch and what is
 *  saved at each interrupt.  Plus saving the entire context should make
 *  it easier to make gdb aware of RTEMS tasks.
 */

typedef struct {
    unsigned32 flags;      /* whatever */
    unsigned32 gr1;        /* scratch -- caller saves */
    unsigned32 gr2;        /* RP -- return pointer */
    unsigned32 gr3;        /* scratch -- callee saves */
    unsigned32 gr4;        /* scratch -- callee saves */
    unsigned32 gr5;        /* scratch -- callee saves */
    unsigned32 gr6;        /* scratch -- callee saves */
    unsigned32 gr7;        /* scratch -- callee saves */
    unsigned32 gr8;        /* scratch -- callee saves */
    unsigned32 gr9;        /* scratch -- callee saves */
    unsigned32 gr10;       /* scratch -- callee saves */
    unsigned32 gr11;       /* scratch -- callee saves */
    unsigned32 gr12;       /* scratch -- callee saves */
    unsigned32 gr13;       /* scratch -- callee saves */
    unsigned32 gr14;       /* scratch -- callee saves */
    unsigned32 gr15;       /* scratch -- callee saves */
    unsigned32 gr16;       /* scratch -- callee saves */
    unsigned32 gr17;       /* scratch -- callee saves */
    unsigned32 gr18;       /* scratch -- callee saves */
    unsigned32 gr19;       /* scratch -- caller saves */
    unsigned32 gr20;       /* scratch -- caller saves */
    unsigned32 gr21;       /* scratch -- caller saves */
    unsigned32 gr22;       /* scratch -- caller saves */
    unsigned32 gr23;       /* argument 3 */
    unsigned32 gr24;       /* argument 2 */
    unsigned32 gr25;       /* argument 1 */
    unsigned32 gr26;       /* argument 0 */
    unsigned32 gr27;       /* DP -- global data pointer */
    unsigned32 gr28;       /* return values -- caller saves */
    unsigned32 gr29;       /* return values -- caller saves */
    unsigned32 sp;         /* gr30 */
    unsigned32 gr31;

    /* Various control registers */

    unsigned32 sar;         /* cr11 */
    unsigned32 ipsw;        /* cr22; full 32 bits of psw */
    unsigned32 iir;         /* cr19; interrupt instruction register */
    unsigned32 ior;         /* cr21; interrupt offset register */
    unsigned32 isr;         /* cr20; interrupt space register (not used) */
    unsigned32 pcoqfront;   /* cr18; front que offset */
    unsigned32 pcoqback;    /* cr18; back que offset */
    unsigned32 pcsqfront;   /* cr17; front que space (not used) */
    unsigned32 pcsqback;    /* cr17; back que space (not used) */
    unsigned32 itimer;      /* cr16; itimer value */

} Context_Control;


/* Must be double word aligned.
 * This will be ok since our allocator returns 8 byte aligned chunks
 */

typedef struct {
    double      fr0;        /* status */
    double      fr1;        /* exception information */
    double      fr2;        /* exception information */
    double      fr3;        /* exception information */
    double      fr4;        /* argument */
    double      fr5;        /* argument */
    double      fr6;        /* argument */
    double      fr7;        /* argument */
    double      fr8;        /* scratch -- caller saves */
    double      fr9;        /* scratch -- caller saves */
    double      fr10;       /* scratch -- caller saves */
    double      fr11;       /* scratch -- caller saves */
    double      fr12;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr13;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr14;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr15;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr16;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr17;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr18;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr19;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr20;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr21;       /* callee saves -- (PA-RISC 1.1 CPUs) */
    double      fr22;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr23;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr24;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr25;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr26;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr27;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr28;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr29;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr30;       /* caller saves -- (PA-RISC 1.1 CPUs) */
    double      fr31;       /* caller saves -- (PA-RISC 1.1 CPUs) */
} Context_Control_fp;

/*
 *  The following structure defines the set of information saved
 *  on the current stack by RTEMS upon receipt of each interrupt.
 */

typedef struct {
  Context_Control             Integer;
  Context_Control_fp          Floating_Point;
} CPU_Interrupt_frame;

/*
 * The following table contains the information required to configure
 * the HPPA specific parameters.
 */

typedef struct {
  void       (*pretasking_hook)( void );
  void       (*predriver_hook)( void );
  void       (*postdriver_hook)( void );
  void       (*idle_task)( void );

                 /* HPPA simulator is slow enough; don't waste time
                  * zeroing memory that is already zero
                  */
  boolean      do_zero_of_workspace;

  unsigned32   interrupt_stack_size;
  unsigned32   extra_system_initialization_stack;

  /*
   * Control of external interrupts.
   * We keep a table of external vector numbers (0 - 31)
   * The table is sorted by priority, that is: the first entry
   * in the table indicates the vector that is highest priorty.
   * The handler function is stored in _ISR_Vector_Table[] and
   * is set by rtems_interrupt_catch()
   */

  unsigned32   external_interrupts;   /* # of external interrupts we use */
  unsigned32   external_interrupt[HPPA_EXTERNAL_INTERRUPTS];

  void       (*spurious_handler)( unsigned32 mask, CPU_Interrupt_frame *);

  unsigned32   itimer_clicks_per_microsecond; /* for use by Clock driver */
}   rtems_cpu_table;

/* variables */

EXTERN Context_Control_fp  _CPU_Null_fp_context;
EXTERN unsigned32          _CPU_Default_gr27;
EXTERN void               *_CPU_Interrupt_stack_low;
EXTERN void               *_CPU_Interrupt_stack_high;

#endif          /* ! ASM */

/*
 *  context size area for floating point
 */

#ifndef ASM
#define CPU_CONTEXT_FP_SIZE sizeof( Context_Control_fp )
#endif

/*
 *  size of a frame on the stack
 */

#define CPU_FRAME_SIZE (16 * 4)

/*
 * (Optional) # of bytes for libmisc/stackchk to check
 * If not specifed, then it defaults to something reasonable
 * for most architectures.
 */

#define CPU_STACK_CHECK_SIZE    (CPU_FRAME_SIZE * 2)

/*
 *  extra stack required by system initialization thread
 */

#define CPU_SYSTEM_INITIALIZATION_THREAD_EXTRA_STACK 0

/*
 * HPPA has 32 interrupts, then 32 external interrupts
 * Rtems (_ISR_Vector_Table) is aware of the first 64
 * A BSP may reserve more.
 *
 * External interrupts all come thru the same vector (4)
 * The external handler is the only person aware of the other
 * interrupts (genie, rhino, etc)
 */

#define CPU_INTERRUPT_NUMBER_OF_VECTORS  (HPPA_INTERRUPT_MAX)

/*
 * Don't be chintzy here; we don't want to debug these problems
 * Some of the tests eat almost 4k.
 * Plus, the HPPA always allocates chunks of 64 bytes for stack
 *       growth.
 */

#define CPU_STACK_MINIMUM_SIZE          (8 * 1024)

/*
 * HPPA double's must be on 8 byte boundary
 */

#define CPU_ALIGNMENT              8

/*
 * just follow the basic HPPA alignment for the heap and partition
 */

#define CPU_HEAP_ALIGNMENT         CPU_ALIGNMENT
#define CPU_PARTITION_ALIGNMENT    CPU_ALIGNMENT

/*
 * HPPA stack is best when 64 byte aligned.
 */

#define CPU_STACK_ALIGNMENT        64

#ifndef ASM

/* macros */

/*
 *  ISR handler macros
 *
 *  These macros perform the following functions:
 *     + disable all maskable CPU interrupts
 *     + restore previous interrupt level (enable)
 *     + temporarily restore interrupts (flash)
 *     + set a particular level
 */

/* Disable interrupts; returning previous level in _level */
#define _CPU_ISR_Disable( _isr_cookie ) \
  do { \
         HPPA_ASM_RSM(HPPA_PSW_I, _isr_cookie);   \
  } while(0)

/* Enable interrupts to previous level from _CPU_ISR_Disable
 * does not change 'level' */
#define _CPU_ISR_Enable( _isr_cookie )  \
  { \
        HPPA_ASM_MTSM( _isr_cookie ); \
  }

/* restore, then disable interrupts; does not change level */
#define _CPU_ISR_Flash( _isr_cookie ) \
  { \
        register int _ignore;  \
        _CPU_ISR_Enable( _isr_cookie ); \
        _CPU_ISR_Disable( _ignore ); \
  }

/*
 * Interrupt task levels
 *
 * Future scheme proposal
 *      level will be an index into a array.
 *      Each entry of array will be the interrupt bits
 *        enabled for that level.  There will be 32 bits of external
 *        interrupts (to be placed in EIEM) and some (optional) bsp
 *        specific bits
 *
 * For pixel flow this *may* mean something like:
 *      level 0:   all interrupts enabled (external + rhino)
 *      level 1:   rhino disabled
 *      level 2:   all io interrupts disabled (timer still enabled)
 *      level 7:   *ALL* disabled (timer disabled)
 */

/* set interrupts on or off; does not return new level */
#define _CPU_ISR_Set_level( new_level ) \
  { \
        volatile int ignore; \
        if ( new_level )  HPPA_ASM_RSM(HPPA_PSW_I, ignore); \
        else              HPPA_ASM_SSM(HPPA_PSW_I, ignore); \
  }

/* end of ISR handler macros */

/*
 *  Context handler macros
 *
 *  These macros perform the following functions:
 *     + initialize a context area
 *     + restart the current thread
 *     + calculate the initial pointer into a FP context area
 *     + initialize an FP context area
 *
 *  HPPA port adds two macros which hide the "indirectness" of the
 *  pointer passed the save/restore FP context assembly routines.
 */

#define _CPU_Context_Initialize( _the_context, _stack_base, _size, \
                                  _new_level, _entry_point ) \
  do { \
    unsigned32 _stack; \
    \
    (_the_context)->flags = 0xfeedf00d; \
    (_the_context)->pcoqfront = (unsigned32)(_entry_point); \
    (_the_context)->pcoqback  = (unsigned32)(_entry_point) + 4; \
    (_the_context)->pcsqfront = 0; \
    (_the_context)->pcsqback  = 0; \
    if ( (_new_level) ) \
        (_the_context)->ipsw = CPU_PSW_INTERRUPTS_OFF; \
    else \
        (_the_context)->ipsw = CPU_PSW_INTERRUPTS_ON; \
    \
    _stack = ((unsigned32)(_stack_base) + (CPU_STACK_ALIGNMENT - 1)); \
    _stack &= ~(CPU_STACK_ALIGNMENT - 1); \
    if ((_stack - (unsigned32) (_stack_base)) < CPU_FRAME_SIZE) \
       _stack += CPU_FRAME_SIZE; \
    \
    (_the_context)->sp = (_stack); \
    (_the_context)->gr27 = _CPU_Default_gr27; \
  } while (0)

#define _CPU_Context_Restart_self( _the_context ) \
    do { \
         _CPU_Context_restore( (_the_context) ); \
    } while (0)

#define _CPU_Context_Fp_start( _base, _offset ) \
   ( (void *) (_base) + (_offset) )

#define _CPU_Context_Initialize_fp( _destination ) \
  do { \
    *((Context_Control_fp *) *((void **) _destination)) = _CPU_Null_fp_context;\
  } while(0)

#define _CPU_Context_save_fp( _fp_context ) \
   _CPU_Save_float_context( *(Context_Control_fp **)(_fp_context) )

#define _CPU_Context_restore_fp( _fp_context ) \
   _CPU_Restore_float_context( *(Context_Control_fp **)(_fp_context) )

/* end of Context handler macros */

/*
 *  Fatal Error manager macros
 *
 *  These macros perform the following functions:
 *    + disable interrupts and halt the CPU
 */

void    hppa_cpu_halt(unsigned32 type_of_halt, unsigned32 the_error);
#define _CPU_Fatal_halt( _error ) \
    hppa_cpu_halt(0, _error)

/* end of Fatal Error manager macros */

/*
 *  Bitfield handler macros
 *
 *  These macros perform the following functions:
 *     + scan for the highest numbered (MSB) set in a 16 bit bitfield
 *
 *  NOTE:
 *
 *  The HPPA does not have a scan instruction.  This functionality
 *  is implemented in software.
 */

int hppa_rtems_ffs(unsigned int value);
#define _CPU_Bitfield_Find_first_bit( _value, _output ) \
    _output = hppa_rtems_ffs(_value)

/* end of Bitfield handler macros */

/*
 *  Priority handler macros
 *
 *  These macros perform the following functions:
 *    + return a mask with the bit for this major/minor portion of
 *      of thread priority set.
 *    + translate the bit number returned by "Bitfield_find_first_bit"
 *      into an index into the thread ready chain bit maps
 *
 *  Note: 255 is the lowest priority
 */

#define _CPU_Priority_Mask( _bit_number ) \
  ( 1 << (_bit_number) )

#define _CPU_Priority_Bits_index( _priority ) \
  (_priority)

/* end of Priority handler macros */

/* functions */

/*
 *  _CPU_Initialize
 *
 *  This routine performs CPU dependent initialization.
 */

void _CPU_Initialize(
  rtems_cpu_table  *cpu_table,
  void      (*thread_dispatch)
);

/*
 *  _CPU_ISR_install_raw_handler
 *
 *  This routine installs a "raw" interrupt handler directly into the 
 *  processor's vector table.
 */
 
void _CPU_ISR_install_raw_handler(
  unsigned32  vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_ISR_install_vector
 *
 *  This routine installs an interrupt vector.
 */

void _CPU_ISR_install_vector(
  unsigned32  vector,
  proc_ptr    new_handler,
  proc_ptr   *old_handler
);

/*
 *  _CPU_Context_switch
 *
 *  This routine switches from the run context to the heir context.
 */

void _CPU_Context_switch(
  Context_Control  *run,
  Context_Control  *heir
);

/*
 *  _CPU_Context_restore
 *
 *  This routine is generally used only to restart self in an
 *  efficient manner and avoid stack conflicts.
 */

void _CPU_Context_restore(
  Context_Control *new_context
);

/*
 *  _CPU_Save_float_context
 *
 *  This routine saves the floating point context passed to it.
 *
 *  NOTE:  _CPU_Context_save_fp is implemented as a macro on the HPPA
 *         which dereferences the pointer before calling this.
 */

void _CPU_Save_float_context(
  Context_Control_fp *fp_context
);

/*
 *  _CPU_Restore_float_context
 *
 *  This routine restores the floating point context passed to it.
 *
 *  NOTE:  _CPU_Context_save_fp is implemented as a macro on the HPPA
 *         which dereferences the pointer before calling this.
 */

void _CPU_Restore_float_context(
  Context_Control_fp *fp_context
);


/*  The following routine swaps the endian format of an unsigned int.
 *  It must be static so it can be referenced indirectly.
 */

static inline unsigned int
CPU_swap_u32(unsigned32 value)
{
  unsigned32 swapped;

  HPPA_ASM_SWAPBYTES(value, swapped);

  return( swapped );
}

/*
 * Unused; I think it should go away
 */

#if 0
#define enable_tracing()
#endif

#endif   /* ! ASM */

#ifdef __cplusplus
}
#endif

#endif   /* ! __CPU_h */
