#include "common.h"
#include "key_event.h"
#include "display.h"
#include "my_display.h"
#include "app.h"
#include "gear.h"
#include "rf.h"
#include "pkg.h"
#include "app.h"
#include "key.h"
#include "n_timer.h"
#include "ssd1306_oled.h"
#include "mac_id.h"
#include "power_saving.h"
#include "adapter.h"

//short key tingzhi tunxi dachong xiaochong fanquan fangai
//long key  tingzhi tunxi dachong xiaochong fanquan fangai
//immediately zhubian fuxi nuanfeng set plus

typedef enum {
  name_long_key_tingzhi_peidui = 1,
  name_short_key_tingzhi,
  name_short_key_tunxi,
  name_short_key_zhubian_immediately,
  name_short_key_fuxi_immediately,
  name_short_key_nuanfeng_immediately,
  name_short_key_dachong,
  name_short_key_xiaochong,
  name_short_key_fanquan,
  name_short_key_fangai,
  name_short_key_immediately_plus,
  name_short_key_immediately_set,
  name_long_key_tunxi_qingjie,
  name_long_key_dachong_chongshua_kai,
  name_long_key_dachong_chongshua_guan,
  name_long_key_xiaochong_jiaogan,
  name_long_key_fanquan_fangai,
  name_long_key_fangai_jiedian_kai,
  name_long_key_fangai_jiedian_guan,
} fun_name_t;

static fun_name_t fun_name;
bool is_processed_key_dachong_zidongchongshua_one_period;
bool is_processed_key_fangai_zhinengjiedian_one_period;

static bool is_same_key (u8 key)
{
  if ((pre_key == KEY_SET || pre_key == KEY_PLUS) && (key == KEY_SET || key == KEY_PLUS))
    return true;

  return false;
}

void long_key_tingzhi_peidui ()
{
  rf_package_t rf_pack;
  u32 send_period;
  u32 start_time;

  fun_name = name_long_key_tingzhi_peidui;

  peidui (&rf_pack);

  send_period  = clock_time ();
  start_time   = clock_time ();

  long_tingzhi_display ();

  while (1) {
    if (n_clock_time_exceed(start_time, 30000000)) {//30s
      rf_8359_set_tx ();
      clear_screen ();
      return;
    }

    if (n_clock_time_exceed (send_period, 1000000)) {//1S
      decrease_rf_power_tx ();
      send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
      rf_8359_set_rx ();
      send_period = clock_time ();
    }

    if (receive_from_peer (new_id)) {
      rf_8359_set_tx ();

      long_key_chengong_display ();

      write_id (&new_id, 4);
      uid = new_id;

      idle_time_for_sleep (3000);
      reload_sys_time ();
      return;
    }

    if (app_read_single_key (KEY_TINGZHI, SHORT_KEY_IMMEDIATELY)) {
      exit_peidui = true;
      rf_8359_set_tx ();
      clear_screen ();
      return;
    }
  }
}

void short_key_tingzhi ()
{
  rf_package_t rf_pack;

  user_id = 0;

  shuiya = get_gear (user_id, "shuiya");
  penzuiweizhi = get_gear (user_id, "penzuiweizhi");

  fix_pack_with_user_value (&rf_pack, TINGZHI);
  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_tingzhi) {
    common_key_display (KEY_TINGZHI);
    fun_name = name_short_key_tingzhi;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_tunxi ()
{
  rf_package_t rf_pack;

  user_id = 0;

  shuiya = get_gear (user_id, "shuiya");
  penzuiweizhi = get_gear (user_id, "penzuiweizhi");

  fix_pack_with_user_value (&rf_pack, TUNXIYIDONG);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_tunxi) {
    common_key_display (KEY_TUNXI);
    fun_name = name_short_key_tunxi;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_zhubian_immediately ()
{
  rf_package_t rf_pack;

  user_id = 0;

  shuiya = get_gear (user_id, "shuiya");
  penzuiweizhi = get_gear (user_id, "penzuiweizhi");

  fix_pack_with_user_value (&rf_pack, QIANGXIYIDONG);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_zhubian_immediately) {
    common_key_display (KEY_ZHUBIAN);
    fun_name = name_short_key_zhubian_immediately;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_fuxi_immediately ()
{
  rf_package_t rf_pack;

  user_id = 1;

  shuiya = get_gear (user_id, "shuiya");
  penzuiweizhi = get_gear (user_id, "penzuiweizhi");

  fix_pack_with_user_value (&rf_pack, FUXIYIDONG);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_fuxi_immediately) {
    common_key_display (KEY_FUXI);
    fun_name = name_short_key_fuxi_immediately;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_nuanfeng_immediately ()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value (&rf_pack, HONGGAN);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_nuanfeng_immediately) {
    common_key_display (KEY_NUANFENG);
    fun_name = name_short_key_nuanfeng_immediately;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_dachong ()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value (&rf_pack, QUANPAI);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_dachong) {
    common_key_display (KEY_DACHONG);
    fun_name = name_short_key_dachong;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_xiaochong ()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value (&rf_pack, BANPAI);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_xiaochong) {
    common_key_display (KEY_XIAOCHONG);
    fun_name = name_short_key_xiaochong;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_fanquan ()
{
  rf_package_t rf_pack;

  fix_xuexi_pack_with_user_value (&rf_pack, 0x5e, 0x24);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_fanquan) {
    common_key_display (KEY_FANQUAN);
    fun_name = name_short_key_fanquan;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_fangai ()
{
  rf_package_t rf_pack;

  fix_xuexi_pack_with_user_value (&rf_pack, 0x5e, 0x23);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (fun_name != name_short_key_fangai) {
    common_key_display (KEY_FANGAI);
    fun_name = name_short_key_fangai;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void short_key_immediately_plus ()
{
  rf_package_t rf_pack;

  fun_name = name_short_key_immediately_plus;

  if (is_same_key (KEY_PLUS)) {
    if (strcmp ("penzuiqianyi", get_first_display ()) == 0) {
      if (penzuiweizhi == 5) {
        penzuiweizhi = 6;

        fix_pack_with_user_value (&rf_pack, CHONGXITIAOJIE);
        send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
        penzuiweizhi = get_gear (user_id, "penzuiweizhi");
      } else {
    	update_gear (user_id, "penzuiweizhi", GEAR_UP);
    	penzuiweizhi = get_gear (user_id, "penzuiweizhi");

        fix_pack_with_user_value (&rf_pack, CHONGXITIAOJIE);
        send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
      }
    } else if (strcmp ("penzuihouyi", get_first_display ()) == 0) {
      if (penzuiweizhi == 1) {
        penzuiweizhi = 0;

        fix_pack_with_user_value (&rf_pack, CHONGXITIAOJIE);
        send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
    	penzuiweizhi = get_gear (user_id, "penzuiweizhi");
      } else {
        update_gear (user_id, "penzuiweizhi", GEAR_DOWN);
    	penzuiweizhi = get_gear (user_id, "penzuiweizhi");

        fix_pack_with_user_value (&rf_pack, CHONGXITIAOJIE);
        send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
      }
    } else if (strcmp ("shuiya", get_first_display ()) == 0) {
      update_gear (user_id, get_first_display (), GEAR_NONE);
      shuiya = get_gear (user_id, "shuiya");

      fix_pack_with_user_value (&rf_pack, CHONGXITIAOJIE);
      send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
    } else {
      update_gear (0, get_first_display (), GEAR_NONE);
      update_gear (1, get_first_display (), GEAR_NONE);
      update_app_gear ();

      fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
      send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
    }
  } else
    clear_screen ();

  if ((strcmp ("penzuiqianyi", get_first_display ()) == 0 ||\
		  strcmp ("penzuihouyi", get_first_display ()) == 0) &&\
		  is_same_key (KEY_PLUS)) {
	  ;
  } else
	display_two_item (1, 0);
}

void short_key_immediately_set ()
{
  fun_name = name_short_key_immediately_set;

  if (is_same_key (KEY_SET))
    update_display ();
  else
	clear_screen ();

  display_two_item (1, 1);
}

void long_key_tunxi_qingjie ()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value (&rf_pack, PENZUIQINGJIE);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if(fun_name != name_long_key_tunxi_qingjie) {
    long_tunxi_display ();
    fun_name = name_long_key_tunxi_qingjie;
    idle_time_for_sleep (3000);
    reload_sys_time ();
  }
}

void long_key_dachong_chongshua_kai ()//3s
{
  rf_package_t rf_pack;

  if (!zidongchongshua) {
    update_gear (0, "zidongchongshua", GEAR_NONE);
    update_gear (1, "zidongchongshua", GEAR_NONE);
    zidongchongshua = get_gear (user_id, "zidongchongshua");

	fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
	send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
	is_processed_key_dachong_zidongchongshua_one_period = true;

	if (fun_name != name_long_key_dachong_chongshua_kai) {
	  long_dachong_display_kai ();
	  fun_name = name_long_key_dachong_chongshua_kai;
	  idle_time_for_sleep (3000);
	  reload_sys_time ();
	}
  } else
    is_processed_key_dachong_zidongchongshua_one_period = false;
}

void long_key_dachong_chongshua_guan ()//8s
{
  rf_package_t rf_pack;

  if (!is_processed_key_dachong_zidongchongshua_one_period) {
    if (zidongchongshua) {
      update_gear (0, "zidongchongshua", GEAR_NONE);
      update_gear (1, "zidongchongshua", GEAR_NONE);
      zidongchongshua = get_gear (user_id, "zidongchongshua");

      fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
      send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

      if (fun_name != name_long_key_dachong_chongshua_guan) {
        long_dachong_display_guan ();
        fun_name = name_long_key_dachong_chongshua_guan;
        idle_time_for_sleep (3000);
        reload_sys_time ();
      }
    }
  }
}

void long_key_xiaochong_jiaogan ()
{
  rf_package_t rf_pack;

  fun_name = name_long_key_xiaochong_jiaogan;

  update_gear (0, "jiaogankaiguan", GEAR_NONE);
  update_gear (1, "jiaogankaiguan", GEAR_NONE);
  jiaogankaiguan = get_gear (user_id, "jiaogankaiguan");

  if (jiaogankaiguan) {
	fix_xuexi_pack_with_user_value (&rf_pack, 0x24, 0x01);//kai
	send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
	long_xiaochong_display_kai ();
  }else {
	fix_xuexi_pack_with_user_value (&rf_pack, 0x24, 0x02);//guan
	send_rf_data_kemu (&rf_pack, sizeof(rf_pack));
	long_xiaochong_display_guan ();
  }
  idle_time_for_sleep (3000);
  reload_sys_time ();
}

void long_key_fanquan_fangai ()
{
  rf_package_t rf_pack;

  fun_name = name_long_key_fanquan_fangai;

  update_gear (0, "zidongfangai", GEAR_NONE);
  update_gear (1, "zidongfangai", GEAR_NONE);
  zidongfangai = get_gear (user_id, "zidongfangai");

  fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
  send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

  if (zidongfangai)
    long_fanquan_display_kai ();
  else
    long_fanquan_display_guan ();

  idle_time_for_sleep (3000);
  reload_sys_time ();
}

void long_key_fangai_jiedian_kai ()//8s
{
  rf_package_t rf_pack;

  if (!is_processed_key_fangai_zhinengjiedian_one_period) {
    if (!shendian) {
      update_gear (0, "shendian", GEAR_NONE);
      update_gear (1, "shendian", GEAR_NONE);
      shendian = get_gear (user_id, "shendian");

      fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
      send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

      if (fun_name != name_long_key_fangai_jiedian_kai) {
        long_fangai_display_kai ();
        fun_name = name_long_key_fangai_jiedian_kai;
        idle_time_for_sleep (3000);
        reload_sys_time ();
      }
    }
  }
}

void long_key_fangai_jiedian_guan ()//3s
{
  rf_package_t rf_pack;

  if (shendian) {
    update_gear (0, "shendian", GEAR_NONE);
    update_gear (1, "shendian", GEAR_NONE);
    shendian = get_gear (user_id, "shendian");

    fix_pack_with_user_value (&rf_pack, CANSHUTIAOJIE);
    send_rf_data_kemu (&rf_pack, sizeof(rf_pack));

    is_processed_key_fangai_zhinengjiedian_one_period = true;

    if (fun_name != name_long_key_fangai_jiedian_guan) {
      long_fangai_display_guan ();
      fun_name = name_long_key_fangai_jiedian_guan;
      idle_time_for_sleep (3000);
      reload_sys_time ();
    }
  } else
    is_processed_key_fangai_zhinengjiedian_one_period = false;
}
