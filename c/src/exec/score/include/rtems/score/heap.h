/*  heap.h
 *
 *  This include file contains the information pertaining to the Heap
 *  Handler.  A heap is a doubly linked list of variable size
 *  blocks which are allocated using the first fit method.  Garbage
 *  collection is performed each time a block is returned to the heap by
 *  coalescing neighbor blocks.  Control information for both allocated
 *  and unallocated blocks is contained in the heap space.  A heap header
 *  contains control information for the heap.
 *
 *  COPYRIGHT (c) 1989, 1990, 1991, 1992, 1993, 1994.
 *  On-Line Applications Research Corporation (OAR).
 *  All rights assigned to U.S. Government, 1994.
 *
 *  This material may be reproduced by or for the U.S. Government pursuant
 *  to the copyright license under the clause at DFARS 252.227-7013.  This
 *  notice must appear in all copies of this file and its derivatives.
 *
 *  $Id$
 */

#ifndef __RTEMS_HEAP_h
#define __RTEMS_HEAP_h

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Status codes for heap_extend
 */

typedef enum {
  HEAP_EXTEND_SUCCESSFUL,
  HEAP_EXTEND_ERROR,
  HEAP_EXTEND_NOT_IMPLEMENTED
}  Heap_Extend_status;

/*
 *  Constants used in the size/used field of each heap block to
 *  indicate when a block is free or in use.
 */

#define HEAP_BLOCK_USED    1           /* indicates block is in use */
#define HEAP_BLOCK_FREE    0           /* indicates block is free */

/*
 *  The size/used field value for the dummy front and back flags.
 */

#define HEAP_DUMMY_FLAG    (0 + HEAP_BLOCK_USED)

/*
 *  The following constants reflect various requirements of the
 *  heap data structures which impact the management of a heap.
 *
 * NOTE:   Because free block overhead is greater than used block
 *         overhead AND a portion of the allocated space is from
 *         the extra free block overhead, the absolute lower bound
 *         of the minimum fragment size is equal to the size of
 *         the free block overhead.
 */

#define HEAP_OVERHEAD \
  (sizeof( unsigned32 ) * 2)         /* size dummy first and last blocks */
#define HEAP_BLOCK_USED_OVERHEAD \
  (sizeof( void * ) * 2)             /* num bytes overhead in used block */
#define HEAP_MINIMUM_SIZE \
  (HEAP_OVERHEAD + sizeof (Heap_Block))
                                     /* min number of bytes the user may */
                                     /*   specify for the heap size      */

/*
 *  The following defines the data structure used to manage
 *  individual blocks in a heap.   When the block is allocated, the
 *  next and previous fields are not used by the Heap Handler
 *  and thus the address returned for the block starts at
 *  the address of the next field.
 *
 *  NOTE:  The next and previous pointers are only valid when the
 *         block is free.  Caution must be exercised to insure that
 *         allocated blocks are large enough to contain them and
 *         that they are not accidentally overwritten when the
 *         block is actually allocated.
 */

typedef struct Heap_Block_struct Heap_Block;

struct Heap_Block_struct {
  unsigned32  back_flag;   /* size and status of prev block */
  unsigned32  front_flag;  /* size and status of block */
  Heap_Block *next;        /* pointer to next block */
  Heap_Block *previous;    /* pointer to previous block */
};

/*
 *  The following defines the control block used to manage each heap.
 *
 *  NOTE:
 *
 *  This structure is layed out such that it can be used a a dummy
 *  first and last block on the free block chain.  The extra padding
 *  insures the dummy last block is the correct size.
 *
 *  The first Heap_Block starts at first while the second starts at
 *  final.  This is effectively the same trick as is used in the Chain
 *  Handler.
 */

typedef struct {
  Heap_Block *start;       /* first valid block address in heap */
  Heap_Block *final;       /* last valid block address in heap */

  Heap_Block *first;       /* pointer to first block in heap */
  Heap_Block *permanent_null;  /* always NULL pointer */
  Heap_Block *last;        /* pointer to last block in heap */
  unsigned32  page_size;   /* allocation unit */
  unsigned32  reserved;
}   Heap_Control;

/*
 *  _Heap_Initialize
 *
 *  DESCRIPTION:
 *
 *  This routine initializes the_heap record to manage the
 *  contiguous heap of size bytes which starts at starting_address.
 *  Blocks of memory are allocated from the heap in multiples of
 *  page_size byte units.
 */

unsigned32 _Heap_Initialize(
  Heap_Control *the_heap,
  void         *starting_address,
  unsigned32    size,
  unsigned32    page_size
);

/*
 *  _Heap_Extend
 *
 *  DESCRIPTION:
 *
 *  This routine grows the_heap memory area using the size bytes which
 *  begin at starting_address.
 */

Heap_Extend_status _Heap_Extend(
  Heap_Control *the_heap,
  void         *starting_address,
  unsigned32    size,
  unsigned32   *amount_extended
);

/*
 *  _Heap_Allocate
 *
 *  DESCRIPTION:
 *
 *  DESCRIPTION:
 *
 *  This function attempts to allocate a block of size bytes from
 *  the_heap.  If insufficient memory is free in the_heap to allocate
 *  a  block of the requested size, then NULL is returned.
 */

void *_Heap_Allocate(
  Heap_Control *the_heap,
  unsigned32    size
);

/*
 *  _Heap_Size_of_user_area
 *
 *  DESCRIPTION:
 *
 *  This kernel routine sets size to the size of the given heap block.
 *  It returns TRUE if the starting_address is in the heap, and FALSE
 *  otherwise.
 */

boolean _Heap_Size_of_user_area(
  Heap_Control        *the_heap,
  void                *starting_address,
  unsigned32          *size
);

/*
 *  _Heap_Free
 *
 *  DESCRIPTION:
 *
 *  This routine returns the block of memory which begins
 *  at starting_address to the_heap.  Any coalescing which is
 *  possible with the freeing of this routine is performed.
 */

boolean _Heap_Free(
  Heap_Control *the_heap,
  void         *start_address
);

/*
 *  _Heap_Walk
 *
 *  DESCRIPTION:
 *
 *  This routine walks the heap to verify its integrity.
 */

void _Heap_Walk(
  Heap_Control *the_heap,
  int           source,
  boolean       do_dump
);

/*
 *  _Heap_Head
 *
 *  DESCRIPTION:
 *
 *  This function returns the head of the specified heap.
 */

STATIC INLINE Heap_Block *_Heap_Head (
  Heap_Control *the_heap
);

/*
 *  _Heap_Tail
 *
 *  DESCRIPTION:
 *
 *  This function returns the tail of the specified heap.
 */

STATIC INLINE Heap_Block *_Heap_Tail (
  Heap_Control *the_heap
);

/*
 *  _Heap_Previous_block
 *
 *  DESCRIPTION:
 *
 *  This function returns the address of the block which physically
 *  precedes the_block in memory.
 */

STATIC INLINE Heap_Block *_Heap_Previous_block (
  Heap_Block *the_block
);

/*
 *  _Heap_Next_block
 *
 *  DESCRIPTION:
 *
 *  This function returns the address of the block which physically
 *  follows the_block in memory.
 */

STATIC INLINE Heap_Block *_Heap_Next_block (
  Heap_Block *the_block
);

/*
 *  _Heap_Block_at
 *
 *  DESCRIPTION:
 *
 *  This function calculates and returns a block's location (address)
 *  in the heap based upad a base address and an offset.
 */

STATIC INLINE Heap_Block *_Heap_Block_at(
  void       *base,
  unsigned32  offset
);

/*PAGE
 *
 *  _Heap_User_Block_at
 *
 */

STATIC INLINE Heap_Block *_Heap_User_Block_at(
  void       *base
);

/*
 *  _Heap_Is_previous_block_free
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the previous block of the_block
 *  is free, and FALSE otherwise.
 */

STATIC INLINE boolean _Heap_Is_previous_block_free (
  Heap_Block *the_block
);

/*
 *  _Heap_Is_block_free
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the block is free, and FALSE otherwise.
 */

STATIC INLINE boolean _Heap_Is_block_free (
  Heap_Block *the_block
);

/*
 *  _Heap_Is_block_used
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the block is currently allocated,
 *  and FALSE otherwise.
 */

STATIC INLINE boolean _Heap_Is_block_used (
  Heap_Block *the_block
);

/*
 *  _Heap_Block_size
 *
 *  DESCRIPTION:
 *
 *  This function returns the size of the_block in bytes.
 */

STATIC INLINE unsigned32 _Heap_Block_size (
  Heap_Block *the_block
);

/*
 *  _Heap_Start_of_user_area
 *
 *  DESCRIPTION:
 *
 *  This function returns the starting address of the portion of the block
 *  which the user may access.
 */

STATIC INLINE void *_Heap_Start_of_user_area (
  Heap_Block *the_block
);

/*
 *  _Heap_Is_block_in
 *
 *  DESCRIPTION:
 *
 *  This function returns TRUE if the_block is within the memory area
 *  managed by the_heap, and FALSE otherwise.
 */

STATIC INLINE boolean _Heap_Is_block_in (
  Heap_Control *the_heap,
  Heap_Block   *the_block
);


/*
 *  _Heap_Is_page_size_valid
 *
 *  DESCRIPTION:
 *
 *  This function validates a specified heap page size.  If the page size
 *  is 0 or if lies outside a page size alignment boundary it is invalid
 *  and FALSE is returned.  Otherwise, the page size is valid and TRUE is
 *  returned.
 */

STATIC INLINE boolean _Heap_Is_page_size_valid(
  unsigned32 page_size
);

/*
 *  _Heap_Build_flag
 *
 *  DESCRIPTION:
 *
 *  This function returns the block flag composed of size and in_use_flag.
 *  The flag returned is suitable for use as a back or front flag in a
 *  heap block.
 */

STATIC INLINE unsigned32 _Heap_Build_flag (
  unsigned32 size,
  unsigned32 in_use_flag
);

#include <rtems/score/heap.inl>

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */
