#include "display.h"
#include "gear.h"
#include "adapter.h"
#include "app.h"
#include "ssd1306_oled.h"
#include "board.h"
#include "key.h"
#include "app.h"
#include "font.h"
#include "my_display.h"

typedef struct {
 u8 first_pos;
 u8 second_pos;
 u8 third_pos;
 u8 fourth_pos;
 u8 fifth_pos;
 u8 sixth_pos;
 u8* first;
 u8* second;
 u8* third;
 u8* fourth;
 u8* fifth;
 u8* sixth;
} shuman_display_t;

bool is_led_on;

void display_20x16 (shuman_display_t* display)
{
  if (display) {
	is_led_on = true;

	clear_screen ();

	if (display->first_pos && display->first)
	  display_graphic_20x16 (2, display->first_pos, (u8*)display->first);

	if (display->second_pos && display->second)
      display_graphic_20x16 (2, display->second_pos, (u8*)display->second);

	if (display->third_pos && display->third)
      display_graphic_20x16 (2, display->third_pos, (u8*)display->third);

	if (display->fourth_pos && display->fourth)
      display_graphic_20x16 (2, display->fourth_pos, (u8*)display->fourth);

	if (display->fifth_pos && display->fifth)
      display_graphic_20x16 (2, display->fifth_pos, (u8*)display->fifth);

	if (display->sixth_pos && display->sixth)
      display_graphic_20x16 (2, display->sixth_pos, (u8*)display->sixth);
  }
}

static void display_32x32 (shuman_display_t* display)
{
  if (display) {
	is_led_on = true;

    clear_screen ();

    if (display->first_pos && display->first)
	  display_graphic_32x32 (1, display->first_pos, (u8*)display->first);

    if (display->second_pos && display->second)
	  display_graphic_32x32 (1, display->second_pos, (u8*)display->second);

    if (display->third_pos && display->third)
	  display_graphic_32x32 (1, display->third_pos, (u8*)display->third);

    if (display->fourth_pos && display->fourth)
	  display_graphic_32x32 (1, display->fourth_pos, (u8*)display->fourth);
  }
}

static void key2display (u8 key, u8** first, u8** second)
{
  if (key == KEY_TINGZHI) {
    *first = (u8*)ting_32x32;
	*second = (u8*)zhi_32x32;
  } else if (key == KEY_TUNXI){
    *first = (u8*)tun_32x32;
	*second = (u8*)xi_32x32;
  }else if (key == KEY_DACHONG) {
	*first = (u8*)da_32x32;
	*second = (u8*)chong_32x32;
  }else if (key == KEY_XIAOCHONG) {
	*first = (u8*)xiao_32x32;
	*second = (u8*)chong_32x32;
  }else if (key == KEY_FANQUAN) {
	*first = (u8*)fan_32x32;
	*second = (u8*)quan_32x32;
  }else if (key == KEY_FANGAI) {
	*first = (u8*)fan_32x32;
	*second = (u8*)gai_32x32;
  }else if (key == KEY_ZHUBIAN) {
	*first = (u8*)zhu_32x32;
	*second = (u8*)bian_32x32;
  }else if (key == KEY_FUXI) {
	*first = (u8*)fu_32x32;
	*second = (u8*)xi_32x32;
  }else if (key == KEY_NUANFENG) {
	*first = (u8*)nuan_32x32;
	*second = (u8*)feng_32x32;
  }
}

void common_key_display (u8 key)
{
  u8* first  = NULL;
  u8* second = NULL;
  shuman_display_t display;

  key2display (key, &first, &second);

  display.first_pos  = 72;
  display.second_pos = 24;
  display.third_pos  = 0;
  display.fourth_pos = 0;
  display.first  = (u8*)first;
  display.second = (u8*)second;
  display.third  = NULL;
  display.fourth = NULL;

  display_32x32 (&display);
}

void long_key_chengong_display ()
{
  shuman_display_t display;

  display.first_pos  = 96;
  display.second_pos = 64;
  display.third_pos  = 32;
  display.fourth_pos = 1;
  display.first  = (u8*)pei_32x32;
  display.second = (u8*)dui_32x32;
  display.third  = (u8*)cheng_32x32;
  display.fourth = (u8*)gong_32x32;

  display_32x32 (&display);
}

void long_tingzhi_display ()
{
  shuman_display_t display;

  display.first_pos  = 96;
  display.second_pos = 64;
  display.third_pos  = 32;
  display.fourth_pos = 1;
  display.first  = (u8*)zheng_32x32;
  display.second = (u8*)zai_32x32;
  display.third  = (u8*)pei_32x32;
  display.fourth = (u8*)dui_32x32;

  display_32x32 (&display);
}

void long_tunxi_display ()
{
  shuman_display_t display;

  display.first_pos  = 96;
  display.second_pos = 64;
  display.third_pos  = 32;
  display.fourth_pos = 1;
  display.first  = (u8*)pen_32x32;
  display.second = (u8*)zui_32x32;
  display.third  = (u8*)qing_32x32;
  display.fourth = (u8*)jie_32x32;

  display_32x32 (&display);
}

void long_dachong_display_kai ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zi_20x16;
  display.second = (u8*)dong_20x16;
  display.third  = (u8*)chong_20x16;
  display.fourth = (u8*)shua_20x16;
  display.fifth  = (u8*)kai_20x16;
  display.sixth  = (u8*)qi_20x16;

  display_20x16 (&display);
}

void long_dachong_display_guan ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zi_20x16;
  display.second = (u8*)dong_20x16;
  display.third  = (u8*)chong_20x16;
  display.fourth = (u8*)shua_20x16;
  display.fifth  = (u8*)guan_20x16;
  display.sixth  = (u8*)bi_20x16;

  display_20x16 (&display);
}

void long_xiaochong_display_kai ()
{
  shuman_display_t display;

  display.first_pos  = 96;
  display.second_pos = 64;
  display.third_pos  = 32;
  display.fourth_pos = 1;

  display.first  = (u8*)jiao_32x32;
  display.second = (u8*)gan_32x32;
  display.third  = (u8*)kai_32x32;
  display.fourth = (u8*)qi_32x32;
  display.fifth  = (u8*)NULL;
  display.sixth  = (u8*)NULL;

  display_32x32 (&display);
}

void long_xiaochong_display_guan ()
{
  shuman_display_t display;

  display.first_pos  = 96;
  display.second_pos = 64;
  display.third_pos  = 32;
  display.fourth_pos = 1;

  display.first  = (u8*)jiao_32x32;
  display.second = (u8*)gan_32x32;
  display.third  = (u8*)guan_32x32;
  display.fourth = (u8*)bi_32x32;
  display.fifth  = (u8*)NULL;
  display.sixth  = (u8*)NULL;

  display_32x32 (&display);
}

void long_fangai_display_guan ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zhi_20x16;
  display.second = (u8*)neng_20x16;
  display.third  = (u8*)jie_20x16;
  display.fourth = (u8*)dian_20x16;
  display.fifth  = (u8*)guan_20x16;
  display.sixth  = (u8*)bi_20x16;

  display_20x16 (&display);
}

void long_fangai_display_kai ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zhi_20x16;
  display.second = (u8*)neng_20x16;
  display.third  = (u8*)jie_20x16;
  display.fourth = (u8*)dian_20x16;
  display.fifth  = (u8*)kai_20x16;
  display.sixth  = (u8*)qi_20x16;

  display_20x16 (&display);
}

void long_fanquan_display_kai ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zi_20x16;
  display.second = (u8*)dong_20x16;
  display.third  = (u8*)fan_20x16;
  display.fourth = (u8*)gai_20x16;
  display.fifth  = (u8*)kai_20x16;
  display.sixth  = (u8*)qi_20x16;

  display_20x16 (&display);
}

void long_fanquan_display_guan ()
{
  shuman_display_t display;

  display.first_pos  = 107;
  display.second_pos = 87;
  display.third_pos  = 67;
  display.fourth_pos = 47;
  display.fifth_pos  = 27;
  display.sixth_pos  = 7;

  display.first  = (u8*)zi_20x16;
  display.second = (u8*)dong_20x16;
  display.third  = (u8*)fan_20x16;
  display.fourth = (u8*)gai_20x16;
  display.fifth  = (u8*)guan_20x16;
  display.sixth  = (u8*)bi_20x16;

  display_20x16 (&display);
}

void display_two_item (bool is_cur_clr, bool is_next_clr)
{
  char* name;
  u8 gear;
  u32* display_item  = NULL;
  display_t display_handler;

  is_led_on = true;

  for (u8 i = 0; i < 2; i++) {
    if (i == 0) {
      name = get_first_display ();
      display_handler.is_clear  = is_cur_clr;
      display_handler.is_first  = 1;
      display_handler.page      = FIRST_PAGE;
    } else {
      name = get_second_display ();
      display_handler.is_clear  = is_next_clr;
      display_handler.is_first  = 0;
      display_handler.page      = SECOND_PAGE;
    }

    display_item = name2display (name);

    if (strcmp ("penzuiqianyi", name) == 0 ||\
		  strcmp ("penzuihouyi", name) == 0)
	  gear = get_gear (user_id, "penzuiweizhi");
    else
	  gear = get_gear (user_id, name);

    if (strcmp ("fengwen", name) == 0) {
	  if (gear == 2)
	    gear = 3;
	  else if (gear == 3)
	    gear = 5;
    }

    if (display_item) {
      if (i == 0 )
	      display_kuang ();
	  display (&display_handler, name, (u8*)display_item[0],\
			(u8*)display_item[1], (u8*)display_item[2], (u8*)display_item[3], gear);
    }
  }
}
