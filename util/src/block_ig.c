/*******************************************************************************
 * File   : block_ig.c
 * Content: Functions manipulate block info
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-11 11:56
 ******************************************************************************/

#ifndef NDEBUG

#include <stdlib.h>
#include <memory.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "block_ig.h"

/*------------------------------------------------------------------------------
 * The functions in this file must compare arbitrary pointers, an
 * operation that the ANSI standard does not guarantee to be portable.
 * 
 * The macros below isolate the pointer comparisons needed in this file.
 * The implementations assume "flat" pointers, for which straightforward
 * comparisons will always work. The definitions below will *NOT* work
 * for some of the common 80x86 memory models.
 */

#define PtrLess(left, right)    ((left) < (right))
#define PtrGrtr(left, right)    ((left) > (right))
#define PtrEqual(left, right)   ((left) == (right))
#define PtrLessEq(left, right)  ((left) <= (right))
#define PtrGrtrEq(left, right)  ((left) >= (right))

/*----------------------------------------------------------------------------*/
/*                            Private data/functions                          */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * s_blockInfoHead points to a singly linked list of debugging information
 * for the memory manager.
 */

static BLOCK_INFO *s_blockInfoHead = NULL;

/*------------------------------------------------------------------------------
 * BLOCK_INFO* IgGetBlockInfo(byte *block)
 *
 * IgGetBlockInfo searches the memory log to find the block that 'block'
 * points into and returns a pointer to the corresponding BLOCK_INFO
 * structure of the memory log.
 * Note: 'block' *must* point into an allocated block or you will get an
 * assertion failure; the function either asserts or succeeds -- it never
 * returns an error.
 *
 *     BLOCK_INFO *blockInfo;
 *     ...
 *     blockInfo = IgGetBlockInfo(block);
 *     // blockInfo->data points to the start of 'blockInfo''s block
 *     // blockInfo->size is the size of the block that 'block' points into
 */

static BLOCK_INFO* IgGetBlockInfo(byte *block)
{
    BLOCK_INFO *curBlockInfo;

    for (curBlockInfo = s_blockInfoHead; curBlockInfo != NULL;
         curBlockInfo = curBlockInfo->next)
    {
        byte *blockStart = curBlockInfo->data;
        byte *blockEnd = curBlockInfo->data + curBlockInfo->size - 1;

        if (PtrGrtrEq(block, blockStart) && PtrLessEq(block, blockEnd))
        {
            break;
        }
    }

    /* Couldn't find pointer? Is it
     * (a) garbage?
     * (b) pointing to a block that was freed?
     * (c) pointing to a block that moved when it was resized by ResizeMemory?
     */
    ASSERTMSG(curBlockInfo != NULL, "IgGetBlockInfo: Counldnt find block info");

    return (curBlockInfo);
}

/*----------------------------------------------------------------------------*/
/*                               Public functions                             */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int IgCreateBlockInfo(byte *newBlock, size_t size)
 *
 * This function creates a log entry for the memory block defined by 
 * 'newBlock':'size'.
 * The function returns TRUE if it successfully creates the log information;
 * FALSE otherwise.
 *
 *     if (IgCreateBlockInfo(newBlock, size))
 *         // success -- the memory log has an entry
 *     else
 *         // failure -- no entry, so release newBlock
 */
int IgCreateBlockInfo(byte *newBlock, size_t size)
{
    BLOCK_INFO *blockInfo;

    ASSERTMSG(newBlock != NULL && size != 0, 
              "IgCreateBlockInfo: argument 1 or 2 is illegal.");

    blockInfo = (BLOCK_INFO *)malloc(sizeof(BLOCK_INFO));
    if (blockInfo != NULL)
    {
        blockInfo->data = newBlock;
        blockInfo->size = size;
        blockInfo->next = s_blockInfoHead;
        s_blockInfoHead = blockInfo;
    }

    return (blockInfo != NULL);
}

/*------------------------------------------------------------------------------
 * void IgFreeBlockInfo(byte *toFree)
 * This function destroys the log entry for the memory block that 'toFree'
 * points to. 'toFree' *must* point to the start of an allocated block;
 * otherwise, you will get an assertion failure.
 */
void IgFreeBlockInfo(byte *toFree)
{
    BLOCK_INFO *curBlockInfo, *prevBlockInfo;

    prevBlockInfo = NULL;

    for (curBlockInfo = s_blockInfoHead; curBlockInfo != NULL; 
        curBlockInfo = curBlockInfo->next)
    {
        if (PtrEqual(curBlockInfo->data, toFree))
        { /* Block founded */
            if (prevBlockInfo == NULL)
            {
                s_blockInfoHead = curBlockInfo->next;
            }
            else
            {
                prevBlockInfo->next = curBlockInfo->next;
            }
            
            break;
        }
        prevBlockInfo = curBlockInfo;
    }

    /* If curBlockInfo is NULL, then toFree is invalid */
    ASSERTMSG(curBlockInfo != NULL, 
        "IgFreeBlockInfo: free invalid memory pointer");

    /* Destroy the contents of *curBlockInfo before freeing them */
    memset(curBlockInfo, GARBAGE, sizeof(BLOCK_INFO));

    free(curBlockInfo);
}

/*------------------------------------------------------------------------------
 * void IgUpdateBlockInfo(byte *oldBlock, byte *newBlock, size_t size)
 *
 * IgUpdateBlockInfo looks up the log information for the memory block that
 * 'oldBlock' points to. The function then updates the log information to
 * reflect the fact that block now lives at 'newBlock' and is 'size' bytes
 * long. 'oldBlock' *must* point to the start of an allocated block;
 * otherwise, you will get an assertion failure.
 */
void IgUpdateBlockInfo(byte *oldBlock, byte *newBlock, size_t size)
{
    BLOCK_INFO *blockInfo;

    ASSERTMSG(newBlock != NULL && size != 0, 
              "IgUpdateBlockInfo: arguments 2 or 3 is illegal.");

    blockInfo = IgGetBlockInfo(oldBlock);
    ASSERTMSG(oldBlock == blockInfo->data, 
        "IgUpdateBlockInfo: argument 1 doesn't point to the start of an \
allocated block.");

    blockInfo->data = newBlock;
    blockInfo->size = size;
}

/*------------------------------------------------------------------------------
 * size_t IgSizeOfBlock(byte *block)
 * 
 * IgSizeOfBlock returns the size of the block that 'block' points to. 'block'
 * *must* point to the start of an allocated block; otherwise, you will get an
 * assertion failure.
 */
size_t IgSizeOfBlock(byte *block)
{
    BLOCK_INFO *blockInfo;

    blockInfo = IgGetBlockInfo(block);
    ASSERTMSG(block == blockInfo->data, 
        "IgSizeOfBlock: argument doesn't point to the start of an \
allocated block.");

    return (blockInfo->size);
}

/*----------------------------------------------------------------------------*/
/* The following routines are used to find dangling pointers and lost memory  */
/* blocks.                                                                    */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * void IgClearMemoryRefs(void)
 *
 * IgClearMemoryRefs marks all blocks in the memory log as being unreferenced.
 */
void IgClearMemoryRefs(void)
{
    BLOCK_INFO *blockInfo;

    for (blockInfo = s_blockInfoHead; blockInfo != NULL; 
        blockInfo = blockInfo->next)
    {
        blockInfo->refered = FALSE;
    }
}

/*------------------------------------------------------------------------------
 * void IgNoteMemoryRef(void *toNote)
 * IgNoteMemoryRef marks the block that 'toNote' points into as being
 * referenced. Note: 'toNote' dose *not* have to point to the start
 * of a block; it may point anywhere within an allocated block.
 */
void IgNoteMemoryRef(void *toNote)
{
    BLOCK_INFO *blockInfo;

    blockInfo = IgGetBlockInfo((byte *)toNote);
    blockInfo->refered = TRUE;
}

/*------------------------------------------------------------------------------
 * void IgCheckMemoryRefs(void)
 *
 * IgCheckMemoryRefs scans the memory log looking for blocks that have
 * not been marked with a call to IgNoteMemoryRef. If this function
 * finds an unmarked block, it asserts.
 */
void IgCheckMemoryRefs(void)
{
    BLOCK_INFO *blockInfo;

    for (blockInfo = s_blockInfoHead; blockInfo != NULL; 
        blockInfo = blockInfo->next)
    {
        /* A simple check for block integrity. If this
         * asserts fires, it means that something is
         * wrong with the debug code that manages
         * block info or, possibly, that a wild memory
         * store has trashed the data structrue. Either
         * way, there's a bug.
         */
        ASSERTMSG(blockInfo->data != NULL && blockInfo->size != 0,
            "IgCheckMemoryRefs: BLOCK_INFO structure is in mass.");

        /* A check for lost or leaky memory. If this
         * assert fires, it menas that the app has
         * either lost track of this block or that
         * not all global pointers have been accounted
         * for with IgNoteMemoryRef.
         */
        ASSERTMSG(blockInfo->refered,
            "IgCheckMemoryRefs: lost or leaky memory.");
    }
}

/*------------------------------------------------------------------------------
 * int IgValidPointer(void *toCheck, size_t size)
 *
 * IgValidPointer verifies that 'toCheck' points into an allocated
 * memory block and that there are at least 'size' allocated bytes
 * from 'toCheck' to the end of the block. If either condition is
 * not met, IgValidPointer will assert; the function will never
 * return FALSE.
 *
 * The reason IgValidPointer returns a flag at all (always TRUE)
 * is allow you to call the function within an ASSERT macro. While
 * this isn't the most efficient method to use, using the macro
 * neatly handles the debug-vs-ship version control issue without
 * your having to resort to #ifndef NDEBUG's or to introducing
 * other ASSERT-like macros.
 *
 *     ASSERT(validPointer(toCheck, size));
 */
int IgValidPointer(void *toCheck, size_t size)
{
    BLOCK_INFO *blockInfo;
    byte *block= (byte *)toCheck;

    ASSERTMSG(toCheck != NULL && size != 0, 
              "IgValidPointer: arguments 1 or 2 is illegal.");

    blockInfo = IgGetBlockInfo(block);    /* This validate toCheck */

    /* size isn't valid if 'block'+'size' overflows the block. */
    ASSERTMSG(PtrLessEq(block + size, blockInfo->data + blockInfo->size), 
        "IgValidPointer: block size overflows.");

    return (TRUE);
}

#endif /* NDEBUG */
