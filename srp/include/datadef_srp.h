/*******************************************************************************
 * File   : datadef_srp.h
 * Content: Common used macros and data type definitions.
 *
 * Coder  : Ean Jee
 * Time   : 2011-02-18 11:07
 ******************************************************************************/

#ifndef _DATADEF_SRP_H
#define _DATADEF_SRP_H

#include "datadef_ig.h"

/* Matrix */
#define SRP_MODELVIEW      0x00000001
#define SRP_PROJECTION     0x00000002
#define SRP_TEXTURE        0x00000003
#define SRP_VIEWPORT       0x00000004

/* Object */
#define SRP_CULL_OBJECT    0x00000011

/* Polygon */
#define SRP_CULL_FACE      0x00000021

#define SET_BIT(word, flag)       ((word) = (word) | (flag))
#define RESET_BIT(word, flag)     ((word) = (word) & ~(flag))
#define CLEAN_BIT(word)           ((word) = (word) & 0x0)

#define ROUND2INT(f)      ((int)(f + 0.5f))

#endif /* _DATADEF_SRP_H */
