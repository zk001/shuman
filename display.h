#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "common.h"

#define FIRST_PAGE  3
#define SECOND_PAGE 1

typedef struct {
  u8 is_clear;
  u8 is_first;
  u8 page;
}display_t;

extern void display(display_t *ptr_mode, char* name, u8* first, u8* second, u8* third, u8* fourth, u8 gear);
extern void display_kuang();
extern void clr_led_status ();

#endif
