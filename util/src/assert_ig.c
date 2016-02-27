/*******************************************************************************
 * File   : assert_ig.c
 * Content: Assertion related functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-9 22:20
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "assert_ig.h"

/*----------------------------------------------------------------------------*/
/*                            Private data/functions                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                               Public functions                             */
/*----------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
 * void IgAssert(const char *fileName, int lineNo)
 *
 */
void IgAssert(const char *fileName, int lineNo)
{
    fflush(NULL);
    fprintf(stderr, "\n  Assertion failure in file %s, line %u\n", fileName, 
            lineNo);
    fflush(NULL);
    abort();
}
 
/*------------------------------------------------------------------------------
 * void IgAssertMsg(char *msg)
 *
 */
void IgAssertMsg(char *msg)
{
    fflush(NULL);
    fprintf(stderr, "\n  Assertion failure in function %s\n", msg);
    fflush(NULL);
    abort();
}
