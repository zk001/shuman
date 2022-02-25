#ifndef __MY_DISPLAY_H__
#define __MY_DISPLAY_H__

typedef enum {
  NORMAL_LED_ON = 1,
  TWO_ITEM_ON
} led_on_t;

extern bool is_led_on;

extern void common_key_display (u8 key);
extern void qidong_display ();
extern void long_key_chengong_display ();
extern void long_tingzhi_display ();
extern void long_tunxi_display ();
extern void long_dachong_display_kai ();
extern void long_dachong_display_guan ();
extern void long_xiaochong_display_kai ();
extern void long_xiaochong_display_guan ();
extern void long_fangai_display_guan ();
extern void long_fangai_display_kai ();
extern void long_fanquan_display_kai ();
extern void display_two_item(bool is_cur_clr, bool is_next_clr);
extern void long_fanquan_display_guan ();
extern bool is_two_item_display ();
extern led_on_t get_led_on_type ();

#endif
