/*******************************************************************************
 * File   : block_ig.h
 * Content: BLOCK_INFO is a structure that contains the memory log information
 *          for one allocated memory block. Every allocated memory block has a
 *          corresponding BLOCK_INFO structure in the memory log.
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-11 11:34
 ******************************************************************************/

#ifndef _BLOCK_IG_H
#define _BLOCK_IG_H

#ifndef NDBUG

#include "datadef_ig.h"

typedef struct tagBLOCK_INFO
{
    struct tagBLOCK_INFO *next; /* Next one */
    byte   *data;               /* Start of block */
    size_t size;                /* Length of block */
    int    refered;             /* Ever referenced? */
} BLOCK_INFO;

extern int IgCreateBlockInfo(byte *newBlock, size_t size);
extern void IgFreeBlockInfo(byte *toFree);
extern void IgUpdateBlockInfo(byte *oldBlock, byte *newBlock, size_t size);
extern size_t IgSizeOfBlock(byte *block);

extern void IgClearMemoryRefs(void);
extern void IgNoteMemoryRef(void *toNote);
extern void IgCheckMemoryRefs(void);
extern int IgValidPointer(void *toCheck, size_t size);

#endif /* NDEBUG */

#endif /* _BLOCK_IG_H */
