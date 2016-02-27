/*******************************************************************************
 * File   : winport_srp.h
 * Content: Porting function for Windows platform
 *
 * Coder  : Ean Jee
 * Time   : 2011-01-28 16:45
 ******************************************************************************/

#ifndef _WINPORT_SRP_H
#define _WINPORT_SRP_H

#include <windows.h>

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

extern int SrpInit(HWND hwnd, int width, int height, int bit);
extern void SrpMain(void);
extern void SrpQuit(HWND hwnd);
extern void SrpWinportSetDemoEntry(void (*init)(void), void (*main)(float), 
                                   void (*quit)(void));

#endif /* _WINPORT_SRP_H */
