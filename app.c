#include "common.h"
#include "app.h"

//general
#include "mac_id.h"
#include "n_timer.h"
#include "gear.h"

//vendor
#include "key_event.h"
#include "my_display.h"

#define PENZUIWEIZHI_MIN 1
#define PENZUIWEIZHI_MAX 5

#define ZUOWEN_MIN       0
#define ZUOWEN_MAX       5

#define SHUIWEN_MIN      0
#define SHUIWEN_MAX      5

#define FENGWEN_MIN      0
#define FENGWEN_MAX      3

#define SHUIYA_MIN       1
#define SHUIYA_MAX       5

#define YEDENG_MIN       0
#define YEDENG_MAX       1

#define DEFAULT_SHUIWEN         3
#define DEFAULT_ZUOWEN          3
#define DEFAULT_FENGWEN         2
#define DEFAULT_PENZUI_WEIZHI   3
#define DEFAULT_SHUIYA          3

#define DEFAULT_ZIDONGFANGGAI   1
#define DEFAULT_SHOUSHIKAIGUAN  0
#define DEFAULT_ZIDONGCHONGSHUA 1
#define DEFAULT_GUANGANG        0
#define DEFAULT_SHENDIAN        0
#define DEFAULT_YEDENG          1

#define MAX_USER 2
#define MAX_GEAR_ITEM 12

_attribute_data_retention_ u32 uid;
_attribute_data_retention_ u32 new_id;
_attribute_data_retention_ u8 user_id = 0;
_attribute_data_retention_ gear_t user_value[2][MAX_GEAR_ITEM];

_attribute_data_retention_ u8 penzuiweizhi;
_attribute_data_retention_ u8 shuiya;

static void set_default_user_value ()
{
  for (u8 i = 0; i < MAX_USER; i++) {
    user_value[i][0].name      = "penzuiweizhi";
	user_value[i][0].value     = DEFAULT_PENZUI_WEIZHI;
	user_value[i][0].max_value = PENZUIWEIZHI_MAX;
	user_value[i][0].min_value = PENZUIWEIZHI_MIN;
	user_value[i][0].direction = GEAR_UP;
	user_value[i][0].method    = KEEP;

	user_value[i][1].name      = "zuowen";
	user_value[i][1].value     = DEFAULT_ZUOWEN;
	user_value[i][1].max_value = ZUOWEN_MAX;
	user_value[i][1].min_value = ZUOWEN_MIN;
	user_value[i][1].direction = GEAR_UP;
	user_value[i][1].method    = BACK;

	user_value[i][2].name      = "shuiwen";
	user_value[i][2].value     = DEFAULT_SHUIWEN;
	user_value[i][2].max_value = SHUIWEN_MAX;
	user_value[i][2].min_value = SHUIWEN_MIN;
	user_value[i][2].direction = GEAR_UP;
	user_value[i][2].method    = BACK;

	user_value[i][3].name      = "fengwen";
	user_value[i][3].value     = DEFAULT_FENGWEN;
	user_value[i][3].max_value = FENGWEN_MAX;
	user_value[i][3].min_value = FENGWEN_MIN;
	user_value[i][3].direction = GEAR_UP;
	user_value[i][3].method    = BACK;

	user_value[i][4].name      = "shuiya";
	user_value[i][4].value     = DEFAULT_SHUIYA;
	user_value[i][4].max_value = SHUIYA_MAX;
	user_value[i][4].min_value = SHUIYA_MIN;
	user_value[i][4].direction = GEAR_UP;
	user_value[i][4].method    = BACK;

	user_value[i][5].name      = "yedeng";
	user_value[i][5].value     = DEFAULT_YEDENG;
	user_value[i][5].max_value = YEDENG_MAX;
	user_value[i][5].min_value = YEDENG_MIN;
	user_value[i][5].direction = GEAR_UP;
	user_value[i][5].method    = RESET;

	user_value[i][6].name      = "guangang";
	user_value[i][6].value     = DEFAULT_GUANGANG;
	user_value[i][6].max_value = 1;
	user_value[i][6].min_value = 0;
	user_value[i][6].direction = GEAR_UP;
	user_value[i][6].method    = RESET;

	user_value[i][7].name      = "zidongfangai";
	user_value[i][7].value     = DEFAULT_ZIDONGFANGGAI;
	user_value[i][7].max_value = 1;
	user_value[i][7].min_value = 0;
	user_value[i][7].direction = GEAR_UP;
	user_value[i][7].method    = RESET;

	user_value[i][8].name      = "shoushikaiguan";
	user_value[i][8].value     = 0;
	user_value[i][8].max_value = 1;
	user_value[i][8].min_value = 0;
	user_value[i][8].direction = GEAR_UP;
	user_value[i][8].method    = RESET;

	user_value[i][9].name      = "zidongchongshua";
	user_value[i][9].value     = 1;
	user_value[i][9].max_value = 1;
	user_value[i][9].min_value = 0;
	user_value[i][9].direction = GEAR_UP;
	user_value[i][9].method    = RESET;

	user_value[i][10].name      = "shendian";
	user_value[i][10].value     = DEFAULT_SHENDIAN;
	user_value[i][10].max_value = 1;
	user_value[i][10].min_value = 0;
	user_value[i][10].direction = GEAR_UP;
	user_value[i][10].method    = RESET;

	user_value[i][11].name      = "jiaogankaiguan";
	user_value[i][11].value     = 1;
	user_value[i][11].max_value = 1;
	user_value[i][11].min_value = 0;
	user_value[i][11].direction = GEAR_UP;
	user_value[i][11].method    = RESET;
  }
}

void update_app_gear ()
{
  penzuiweizhi = get_gear (user_id, "penzuiweizhi");
  shuiya       = get_gear (user_id, "shuiya");
}

void app_init ()
{
  set_default_user_value ();
  read_id (&uid, 4);
  register_gear (0, NULL, &user_value[0][0], MAX_USER, MAX_GEAR_ITEM);
  update_app_gear ();

  qidong_display ();

  register_key_event (KEY_TINGZHI,  0, MS2TICK(3000), 0, LONG_KEY, long_key_tingzhi_peidui);
  register_key_event (KEY_TINGZHI,  0, MS2TICK(2000), 0, SHORT_KEY, short_key_tingzhi);

  register_key_event (KEY_TUNXI,    0, MS2TICK(2000), 0, SHORT_KEY, short_key_tunxi);
  register_key_event (KEY_TUNXI,    0, MS2TICK(3000), 0, LONG_KEY, long_key_tunxi_qingjie);

  register_key_event (KEY_ZHUBIAN,  0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_zhubian_immediately);
  register_key_event (KEY_FUXI,     0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_fuxi_immediately);
  register_key_event (KEY_NUANFENG, 0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_nuanfeng_immediately);

  register_key_event (KEY_DACHONG,  0, MS2TICK(2000), 0, SHORT_KEY, short_key_dachong);
  register_key_event (KEY_DACHONG,  0, MS2TICK(3000), 0, LONG_KEY, long_key_dachong_chongshua_kai);
  register_key_event (KEY_DACHONG,  0, MS2TICK(8000), 0, LONG_KEY, long_key_dachong_chongshua_guan);

  register_key_event (KEY_XIAOCHONG,0, MS2TICK(2000), 0, SHORT_KEY, short_key_xiaochong);
  register_key_event (KEY_XIAOCHONG,0, MS2TICK(3000), 0, LONG_KEY, long_key_xiaochong_jiaogan);

  register_key_event (KEY_FANQUAN,  0, MS2TICK(2000), 0, SHORT_KEY, short_key_fanquan);
  register_key_event (KEY_FANQUAN,  0, MS2TICK(3000), 0, LONG_KEY, long_key_fanquan_fangai);

  register_key_event (KEY_FANGAI,   0, MS2TICK(2000), 0, SHORT_KEY,  short_key_fangai);
  register_key_event (KEY_FANGAI,   0, MS2TICK(3000), 0, LONG_KEY,  long_key_fangai_jiedian_guan);
  register_key_event (KEY_FANGAI,   0, MS2TICK(8000), 0, LONG_KEY,  long_key_fangai_jiedian_kai);

  register_key_event (KEY_SET,      0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_immediately_set);
  register_key_event (KEY_PLUS,     0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_immediately_plus);
}
