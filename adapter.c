#include "common.h"
#include "adapter.h"
#include "gear.h"
#include "display.h"
#include "font.h"

#define MAX_INDEX 7

_attribute_data_retention_ u8 cur_index  = 0;
_attribute_data_retention_ u8 next_index = 1;

static char* display_arry[MAX_INDEX] = {"shuiwen", "shuiya", "zuowen", "fengwen", "penzuiqianyi", "penzuihouyi", "yedeng"};

static const u32 wenshuiwendu[4]  = {(u32)wen_16x16,  (u32)shui_16x16, (u32)wen_16x16,  (u32)du_16x16};
static const u32 wenshuiyali[4]   = {(u32)wen_16x16,  (u32)shui_16x16, (u32)ya_16x16,   (u32)li_16x16};
static const u32 zuoquanwendu[4]  = {(u32)zuo_16x16,  (u32)quan_16x16, (u32)wen_16x16,  (u32)du_16x16};
static const u32 nuanfengwendu[4] = {(u32)nuan_16x16, (u32)feng_16x16, (u32)wen_16x16,  (u32)du_16x16};
static const u32 penzuiqianyi[4]  = {(u32)pen_16x16,  (u32)zui_16x16,  (u32)wei_16x16, (u32)zhi1_16x16};
static const u32 penzuihouyi[4]   = {(u32)pen_16x16,  (u32)zui_16x16,  (u32)wei_16x16,  (u32)zhi1_16x16};
static const u32 yedengkaiguan[4] = {(u32)ye_16x16,   (u32)deng_16x16, (u32)kai_16x16,  (u32)guan_16x16};

void update_display ()
{
  cur_index++;

  if (cur_index == MAX_INDEX)
    cur_index = 0;

  next_index++;

  if (next_index == MAX_INDEX)
    next_index = 0;
}

char* get_first_display ()
{
  return display_arry[cur_index];
}

char* get_second_display ()
{
  return display_arry[next_index];
}

u32* name2display (char* name)
{
  if (strcmp (name, "penzuiqianyi") == 0)
    return (u32*)&penzuiqianyi[0];
  else if (strcmp (name, "penzuihouyi") == 0)
	return (u32*)&penzuihouyi[0];
  else if (strcmp (name, "zuowen") == 0)
    return (u32*)&zuoquanwendu[0];
  else if (strcmp (name, "shuiwen") == 0)
    return (u32*)&wenshuiwendu[0];
  else if (strcmp (name, "fengwen") == 0)
    return (u32*)&nuanfengwendu[0];
  else if (strcmp (name, "shuiya") == 0)
    return (u32*)&wenshuiyali[0];
  else if (strcmp (name, "yedeng") == 0)
    return (u32*)&yedengkaiguan[0];
  else
    return NULL;
}
