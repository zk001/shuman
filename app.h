#ifndef __APP_H__
#define __APP_H__

//BYTE1:8位 Bit7-5：-----地址码，默认为000
//			Bit4-0：为功能执行码
//BYTE2:8位 Bit7-4：水温档位 0-3
//			Bit3：自动翻盖     0：关闭1：开启
//			Bit2：手势开关     0：关闭1：开启
//			Bit1：自动冲刷     0：关闭1：开启
//			Bit0：光感开/关    0：开启1：关闭
//BYTE3:8位 Bit7：省电模式     0：关闭1：开启
//			Bit6-4：喷嘴位置 1-5
//			Bit3-0：座温档位 0-5
//BYTE4:8位 Bit7：夜灯       0：关闭1：开启
//			Bit6-4：水压档位 1-5
//			Bit3-0：风温档位 0-3
//BYTE5:8位 预留 默认为0

typedef struct {
  u8  rf_len1;
  u16 vid;					// 5~6 vendor ID
  u32 pid;					// 7~10 product ID
  u8  control_key;			// 11 function control key
  u8  rf_seq_no; 			// 12 rf sequence total number, save this value in 3.3v analog register.

  u8 start_code;
  u8 byte1;
  u8 byte2;
  u8 byte3;
  u8 byte4;

  u8 byte5;
  u8 unused0;
  u8 unused1;
  u8 unused2;
}__attribute__((packed))rf_package_t;

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

#define COMBIN_KEY_SETUP_TIME  (1000*16*1000)
#define COMBIN_TIME            (3000*16*1000)//ms2tick(3000)
#define COMBIN_KEY_LAST_TIME   (3000*16*1000)

#define SHORT_TIME  (3000*16*1000)
#define LONG_TIME   (3000*16*1000)
#define STUCK_TIME  (15000*16*1000)

#define KEY_PROCESS_TIME        5000//US

#define ID_Flash_Addr 0x020000 //address store id

#define SLEEP_WAIT_TIME 10000 //MS

#define PAIR_KEY_VALUE	0x55
#define MAX_EVENT 20//the number is the register_key_event call times
extern void app_init();
extern void display_kuang();
extern void display_all(bool is_cur_clr, bool is_next_clr);

#endif
