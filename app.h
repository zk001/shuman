#ifndef __APP_H__
#define __APP_H__

#include "common.h"
#include "key.h"

#define KEY_TINGZHI   KEY10
#define KEY_TUNXI     KEY6
#define KEY_ZHUBIAN   KEY7
#define KEY_FUXI      KEY8
#define KEY_NUANFENG  KEY9
#define KEY_XIAOCHONG KEY2
#define KEY_DACHONG   KEY3
#define KEY_FANQUAN   KEY1
#define KEY_FANGAI    KEY0
#define KEY_SET       KEY4
#define KEY_PLUS      KEY5

#define COMBIN_KEY_SETUP_TIME  MS2TICK(1000)
#define COMBIN_TIME            MS2TICK(3000)//ms2tick(3000)
#define COMBIN_KEY_LAST_TIME   MS2TICK(3000)

#define SHORT_TIME  MS2TICK(3000)
#define LONG_TIME   MS2TICK(3000)
#define STUCK_TIME  MS2TICK(15000)
#define SHORT_KEY_OVER_TIME MS2TICK(2000)
#define SHORT_KEY_IN_TIME   MS2TICK(3000)
#define DEBOUNCE_TIME MS2TICK(20) //ms

#define ONE_KEY_PRESSING_TWICE_INTERVAL MS2TICK(1000)

#define KEY_PROCESS_TIME   5000//US

#define ID_Flash_Addr 0x020000 //address store id

#define SLEEP_WAIT_TIME 5000 //MS

#define PAIR_KEY_VALUE	0x55
#define MAX_EVENT 22//the number is the register_key_event call times

extern u8 user_id;
extern u32 uid;
extern u32 new_id;

extern u8 penzuiweizhi;
extern u8 shuiya;

extern void app_init();
extern void update_app_gear ();

#endif
