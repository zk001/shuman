#include "common.h"
#include "display.h"

#include "ssd1306_oled.h"
#include "font.h"

#define UP   1
#define DOWN 2

#define START_POS 111

#define HIGH  16
#define WIDTH (16 + 1)

#define PAGE 2

#define LEVEL_LENGTH (8 + 1)
#define KAI_GUAN_POS(col) (col - 20)

static u8* all_level[5] = {(u8*)level1_table, (u8*)level2_table, (u8*)level3_table, (u8*)level4_table, (u8*)level5_table};
static u8  clear_level[16] = {0};
static u8  clear_kai_guan[32] = {0};

static u8 rever_next_pos (u8 col)
{
  col -= WIDTH;
  return col;
}

static void clear_level_indi (u8 page, u8 col)
{
  for (u8 i = 0; i < 5; i++) {
    display_graphic_8x16 (page, col, (u8*)clear_level);
    col -= LEVEL_LENGTH;
  }
}

static void or_level_with_kuang (u8 *dp, u8 col)
{
  for (u8 i = 0; i < 8; i++)
    dp[i] |= kuang_128x16[col + i];

  for (u8 i = 0; i < 8; i++)
    dp[i + 8] |= kuang_128x16[128 + col + i];
}

static void clear_level_with_kuang (u8 *dp, u8 col)
{
  for (u8 i = 0; i < 8; i++)
    dp[i] &= ~kuang_128x16[col + i];

  for (u8 i = 0; i < 8; i++)
    dp[i + 8] &= ~kuang_128x16[128 + col + i];
}

static void or_kuang (u8 *dp, u8 col)
{
  for (u8 i = 0; i < 16; i++)
    dp[i] |= kuang_128x16[col + i];

  for (u8 i = 0; i < 16; i++)
    dp[i + 16] |= kuang_128x16[128 + col + i];
}

static void clear_kuang (u8 *dp, u8 col)
{
  for (u8 i = 0; i < 16; i++)
    dp[i] &= ~ kuang_128x16[i + col];

  for (u8 i = 0; i < 16; i++)
    dp[i+16] &= ~ kuang_128x16[i + 128 + col];
}

void display_kuang ()
{
  display_graphic_128x16 (3, 1, (u8*)kuang_128x16);
}

void display (display_t* ptr_mode, char* name, u8* first, u8* second, u8* third, u8* fourth, u8 gear)
{
  u8 first_pos;
  u8 second_pos;
  u8 third_pos;
  u8 fourth_pos;

  u8 page;
  u8 is_clear;
  u8 is_first;

  page       = ptr_mode->page;
  is_clear   = ptr_mode->is_clear;
  is_first   = ptr_mode->is_first;

  first_pos  = START_POS;
  second_pos = rever_next_pos (first_pos);
  third_pos  = rever_next_pos (second_pos);
  fourth_pos = rever_next_pos (third_pos);

  if (is_first) {
    or_kuang (first, first_pos);
    or_kuang (second,second_pos);
    or_kuang (third, third_pos);
    or_kuang (fourth,fourth_pos);
    or_kuang (clear_kai_guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    or_kuang (kai_16x16, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    or_kuang (guan_16x16, KAI_GUAN_POS(rever_next_pos(fourth_pos)));

    u8 k = rever_next_pos (fourth_pos);

    for (u8 i = 0; i < 5; i++) {
      or_level_with_kuang (clear_level, k);
      k -= LEVEL_LENGTH;
    }
  } else {
    clear_kuang (first,  first_pos);
    clear_kuang (second, second_pos);
    clear_kuang (third,  third_pos);
    clear_kuang (fourth, fourth_pos);
    clear_kuang (clear_kai_guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    clear_kuang (guan_16x16, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    clear_kuang (kai_16x16, KAI_GUAN_POS(rever_next_pos(fourth_pos)));

    u8 k = rever_next_pos (fourth_pos);

    for (u8 i = 0; i < 5; i++) {
      clear_level_with_kuang (clear_level, k);
      k -= LEVEL_LENGTH;
    }
  }

  display_graphic_16x16_2 (1, page, first_pos,  (u8*)first);
  display_graphic_16x16_2 (1, page, second_pos, (u8*)second);
  display_graphic_16x16_2 (1, page, third_pos,  (u8*)third);
  display_graphic_16x16_2 (1, page, fourth_pos, (u8*)fourth);

  if (is_clear) {//clear adjust area
	u8 pos = KAI_GUAN_POS(rever_next_pos(fourth_pos));
    clear_level_indi (page, rever_next_pos(fourth_pos));//clear level
    display_graphic_16x16_2 (1, page, pos, (u8*)clear_kai_guan);//clear kai or guan
    display_graphic_16x16_2 (1, page, rever_next_pos(pos), (u8*)clear_kai_guan);//clear qian or hou
  }

  if(!gear)
    display_graphic_16x16_2 (1, page, KAI_GUAN_POS(rever_next_pos(fourth_pos)), (u8*)guan_16x16);
  else {
    if(strcmp (name, "yedeng") == 0)
      display_graphic_16x16_2 (1, page, KAI_GUAN_POS(rever_next_pos(fourth_pos)), (u8*)kai_16x16);
    else {
      u8 fifth_pos = KAI_GUAN_POS(rever_next_pos(fourth_pos));
      u8 k = rever_next_pos (fourth_pos);

      if (strcmp (name, "penzuiqianyi") == 0) {
        if (is_first) {
          or_kuang (qian_16x16, fifth_pos);
          or_kuang (yi_16x16, rever_next_pos(fifth_pos));
        } else {
          clear_kuang (qian_16x16, fifth_pos);
          clear_kuang (yi_16x16, rever_next_pos(fifth_pos));
        }
        display_graphic_16x16_2 (1, page, fifth_pos, (u8*)qian_16x16);
        display_graphic_16x16_2 (1, page, rever_next_pos(fifth_pos), (u8*)yi_16x16);
      } else if (strcmp (name, "penzuihouyi") == 0) {
        if (is_first) {
          or_kuang (hou_16x16, fifth_pos);
          or_kuang (yi_16x16, rever_next_pos(fifth_pos));
        } else {
          clear_kuang (hou_16x16, fifth_pos);
          clear_kuang (yi_16x16, rever_next_pos(fifth_pos));
        }
        display_graphic_16x16_2 (1, page, fifth_pos, (u8*)hou_16x16);
        display_graphic_16x16_2 (1, page, rever_next_pos(fifth_pos), (u8*)yi_16x16);
      } else {
        for (u8 i = 0; i < gear; i++) {
          if (is_first)
            or_level_with_kuang (all_level[i], k);//add kuang
          else
            clear_level_with_kuang (all_level[i], k);//clear kuang

          display_graphic_8x16 (page, k, (u8*)all_level[i]);
          k -= LEVEL_LENGTH;
        }
      }
    }
  }
}




