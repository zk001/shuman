#ifndef __APP_H__
#define __APP_H__

//BYTE1:8λ Bit7-5��-----��ַ�룬Ĭ��Ϊ000
//			Bit4-0��Ϊ����ִ����
//BYTE2:8λ Bit7-4��ˮ�µ�λ 0-3
//			Bit3���Զ�����     0���ر�1������
//			Bit2�����ƿ���     0���ر�1������
//			Bit1���Զ���ˢ     0���ر�1������
//			Bit0����п�/��    0������1���ر�
//BYTE3:8λ Bit7��ʡ��ģʽ     0���ر�1������
//			Bit6-4������λ�� 1-5
//			Bit3-0�����µ�λ 0-5
//BYTE4:8λ Bit7��ҹ��       0���ر�1������
//			Bit6-4��ˮѹ��λ 1-5
//			Bit3-0�����µ�λ 0-3
//BYTE5:8λ Ԥ�� Ĭ��Ϊ0

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
