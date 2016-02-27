/*******************************************************************************
 * File   : malloc_ig.h
 * Content: Enhanced memory management functions
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-15 10:03
 ******************************************************************************/

#ifndef _MALLOC_IG_H
#define _MALLOC_IG_H

#include <stddef.h>

extern int IgNewMemory(void **new, size_t size);
extern int IgResizeMemory(void **toResize, size_t newSize);
extern void IgFreeMemory(void *toFree);

#endif /* _MALLOC_IG_H */
