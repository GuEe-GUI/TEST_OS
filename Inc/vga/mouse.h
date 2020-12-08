/*
    File:       mouse.h

    Contains:   Provide mouse operating

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_MOUSE__
#define __VGA_MOUSE__

#define Cursor_Default      0
#define Cursor_Help         1
#define Cursor_Progress     2
#define Cursor_Busy         3
#define Cursor_Cross        4
#define Cursor_Text         5
#define Cursor_Pen          6
#define Cursor_Unavailable  7
#define Cursor_SizeHorz     8
#define Cursor_SizeVert     9
#define Cursor_ResizeRight  10
#define Cursor_ResizeLeft   11
#define Cursor_Move         12
#define Cursor_Candidate    13
#define Cursor_Pointer      14

void mouse(void);
void mouseSet(int x, int y, int mode);

#endif
