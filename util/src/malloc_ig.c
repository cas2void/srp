/*******************************************************************************
 * File   : malloc_ig.c
 * Content: Enhanced memory management functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-15 10:14
 ******************************************************************************/

#include <stdlib.h>
#include <memory.h>
#include "datadef_ig.h"
#include "assert_ig.h"
#include "block_ig.h"
#include "malloc_ig.h"

/*----------------------------------------------------------------------------*/
/*                            Private data/functions                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Public functions                             */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * int IgNewMemory(void **new, size_t size)
 *
 * IgNewMemory uses malloc in standard lib to allocate memory, and creates
 * block info tag along with the allocated memory if debug enabled.
 *
 * Parameters:
 *     new - the address of the pointer to the memory about to be allocated
 *     size - request size
 * 
 * Return:
 *     TRUE if successful; otherwise, FALSE.
 */
int IgNewMemory(void **new, size_t size)
{
    byte **ppbNew = (byte **)new;

    ASSERTMSG(new != NULL && size != 0, 
              "IgNewMemory: argument 1 or 2 is illegal.");
    *ppbNew = (byte *)malloc(size);

#ifndef NDEBUG
    {
        if (*ppbNew != NULL)
        {
            memset(*ppbNew, GARBAGE, size);

            /* If unable to create the block information,
             * fake a total memory failure.
             */
            if (!IgCreateBlockInfo(*ppbNew, size))
            {
                free(*ppbNew);
                *ppbNew = NULL;
            }
        }
    }
#endif /* NDEBUG*/

    return (*ppbNew != NULL);
}

/*------------------------------------------------------------------------------
 * int IgResizeMemory(void **toResize, size_t newSize)
 *
 * IgResizeMemory uses realloc in standard lib to resize memory, and updates
 * the block info tag along with the memory if debug enabled.
 *
 * Parameters:
 *     toResize - the address of the pointer to the memory about to be resized
 *     newSize - request size
 * 
 * Return:
 *     TRUE if successful; otherwise, FALSE.
 */
int IgResizeMemory(void **toResize, size_t newSize)
{
    byte **ppbOld = (byte **)toResize;
    byte *pbNew;

#ifndef NDEBUG
    size_t oldSize;
    byte *pbForceNew;
#endif /* NDEBUG */

    ASSERTMSG(toResize != NULL && newSize != 0, 
        "IgResizeMemory: argument 1 or 2 is illegal");

#ifndef NDEBUG
    {
        oldSize = IgSizeOfBlock(*ppbOld);

        /* If the block is shrinking, pre-fill the soon-to-be-released
         * memory. If the block is expanding, force it to move (instead
         * of expanding in place) by faking a realloc. If the block is
         * is the same size, don't do anything.
         */

        if (newSize < oldSize)
        {
            memset((*ppbOld) + newSize, GARBAGE, oldSize - newSize);
        }
        else if (newSize > oldSize)
        {
            if (IgNewMemory((void *)&pbForceNew, newSize))
            {
                memcpy(pbForceNew, *ppbOld, oldSize);
                IgFreeMemory(*ppbOld);
                *ppbOld = pbForceNew;
            }
        }
    }
#endif /* NDEBUG */
    
    pbNew = (byte *)realloc(*ppbOld, newSize);
    if (pbNew != NULL)
    {
#ifndef NDEBUG
        {
            IgUpdateBlockInfo(*ppbOld, pbNew, newSize);

            /* If expanding, initialize the new tail. */
            if (newSize > oldSize)
            {
                memset(pbNew + oldSize, GARBAGE, newSize - oldSize);
            }
        }
#endif /* NDEBUG */

        *ppbOld = pbNew;
    }

    return (pbNew != NULL);
}

/*------------------------------------------------------------------------------
 * void IgFreeMemory(void *toFree)
 *
 * IgFreeMemory uses free in standard lib to free memory, and delete
 * the block info tag along with the memory if debug enabled.
 *
 * Parameters:
 *     toFree - the address of the memory about to be freed
 */
void IgFreeMemory(void *toFree)
{
#ifndef NDEBUG
    {
        memset(toFree, GARBAGE, IgSizeOfBlock(toFree));
        IgFreeBlockInfo(toFree);
    }
#endif /* NDEBUG */

    free(toFree);
}
