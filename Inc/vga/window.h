/*
    File:       window.h

    Contains:   Drawing windows form handles and interfaces in vga mode

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#ifndef __VGA_WINDOW__
#define __VGA_WINDOW__

#include <sys/types.h>
#include <vga/image.h>

#define ShadowColor 85

typedef struct FormPos_t {
    uint32_t WindowPosX;
    uint32_t WindowPosY;
    uint32_t WindowPosZ;
} FormPos_t;

typedef struct FormIcon_t {
    ico_t ICON;
} FormIcon_t;

typedef struct FormCur_t {
    cur_t CUR;
} FormCur_t;

typedef struct FormMenu_t {
    cstring_t Name;
    uint32_t   Index;
} FormMenu_t;

typedef struct ComLi_t {
    string_t Name;
    bool_t   NextUl;
} ComLi_t;

typedef struct ComUl_t {
    string_t Name;
    ComLi_t  ComLi;
    uint32_t LiNumber;
} ComUl_t;

typedef struct FormComNav_t {
    uint32_t NavID;
    uint32_t NavPos;
    ComUl_t  ComUl;
} FormComNav_t;

typedef struct FormComponent_t {
   FormComNav_t WindowComNav;
} FormComponent_t;

typedef struct Form_t {
    uint32_t        WindowID;
    uint32_t        WindowProc;
    uint32_t        WindowPosX;
    FormPos_t       WindowPos;
    FormIcon_t      WindowIcon;
    cstring_t       WindowTitle;
    FormCur_t       WindowCur;
    FormComponent_t WindowComponent;
    cstring_t       ClassName;
} Form_t;

static int FormNumber;

void Window(void);

void CreatWindow(Form_t form, int x, int y, int width, int height, bool_t WindowLight);
void DrawWindowWidget(int x, int y, int mode[], long int color);

#endif
