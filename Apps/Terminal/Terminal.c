/*
    File:       Terminal.cpp

    Contains:   The Terminal of GuEeOS in VGA

    Written by: GUI

    Copyright:  (C) 2018 by GuEe Studio. All rights reserved.
*/

#include <api.h>
#include <vga/window.h>
#include <lib/encoding.h>

int GUEEAPI FormMain(){
    Form_t form;
    form.WindowTitle = "Form";
    CreatWindow(form, 99, 119, 332, 352, true);
    CreatWindow(form, 150, 180, 332, 352, false);
}
