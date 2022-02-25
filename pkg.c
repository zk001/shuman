#include "common.h"
#include "pkg.h"

//general
#include "gear.h"
#include "mac_id.h"

//vendor
#include "app.h"

#define SHUIWEN_POSITION_BYTE2_BIT4(x)        (x << 4)
#define ZIDONGFANGAI_POSITION_BYTE2_BIT3(x)   (x << 3)
#define SHOUSHIKAIGUAN_POSITION_BYTE2_BIT2(x) (x << 2)
#define ZIDONGCHONGSHUA_POSITION_BYTE2_BIT1(x)(x << 1)
#define GUANGANG_POSITION_BYTE2_BIT1(x)       (x << 0)
#define SHENDIANMOSHI_POSITION_BYTE3_BIT7(x)  (x << 7)
#define PENZUIWEIZHI_POSITION_BYTE3_BIT4(x)   (x << 4)
#define ZUOWEN_POSITION_BYTE3_BIT0(x)         (x << 0)
#define YEDENG_POSITION_BYTE4_BIT7(x)         (x << 7)
#define SHUIYA_POSITION_BYTE4_BIT4(x)         (x << 4)
#define FENGWEN_POSITION_BYTE4_BIT0(x)        (x << 0)

#define MAX_FUNCTION 32

u8 const function_code[MAX_FUNCTION] = {
  0,//ֹͣ
  1,//��ϴ
  2,//�����¿�
  3,//��ϴ
  4,//���
  5,//�ƶ���Ħ
  6,//ǿ����Ħ
  7,//��ϴȫ�Զ�
  8,//��ϴ�ƶ�
  9,//������ϴ���Զ�������
  10,//ǿϴ
  11,//��ϴ�ƶ�
  12,//ȫ�Զ�
  13,//ͯϴ
  14,//��ˢ��ˮȫ��
  15,//��ˢ��ˮ����
  16,//����ȫ��
  17,//���ǰ뿪
  18,//����
  19,//����ȫ��
  20,//����ҹ��
  21,//���ܽڵ�
  22,//�ֶ�����
  23,
  24,//��ϴ���ڣ�λ�ú�ˮѹ��
  25,//�������ڣ�ˮ��/����/����/ҹ�ƿ���/ʡ��ģʽ��
  26,//�������£����������죩
  27,//ǿϴ�ƶ�
  28,//��ѯ��ˮ��ص���������             ������//��ϴ
  29,//��ȡ����汾����ɺ���Ȧ�������   ������//��ϴ�ƶ�
  30,//״̬��ѯ
  31,//ϵͳ��ʼ��/ѧϰȫ��/ѧϰȫ�ص�����
};

void fix_pack_with_user_value (rf_package_t *rf_pack, function_t fn)
{
  u8 fun = fn;

  rf_pack->rf_len1     = 0;
  rf_pack->vid         = 0x5453;


  rf_pack->pid           = uid;
  rf_pack->control_key   = 0;

  rf_pack->rf_seq_no     = 0;

  rf_pack->start_code    = 0x4c;

  rf_pack->byte1         = function_code[fun];
  rf_pack->byte2         = SHUIWEN_POSITION_BYTE2_BIT4(get_gear (0, "shuiwen")) |\
                           ZIDONGFANGAI_POSITION_BYTE2_BIT3(get_gear (0, "zidongfangai")) |\
                           SHOUSHIKAIGUAN_POSITION_BYTE2_BIT2(get_gear (0, "shoushikaiguan")) |\
                           ZIDONGCHONGSHUA_POSITION_BYTE2_BIT1(get_gear (0, "zidongchongshua")) |\
                           GUANGANG_POSITION_BYTE2_BIT1(get_gear (0, "guangang"));

  rf_pack->byte3         = SHENDIANMOSHI_POSITION_BYTE3_BIT7(get_gear (0, "shendian")) |\
                           PENZUIWEIZHI_POSITION_BYTE3_BIT4(penzuiweizhi) |\
                           ZUOWEN_POSITION_BYTE3_BIT0(get_gear (0, "zuowen"));

  rf_pack->byte4 = YEDENG_POSITION_BYTE4_BIT7(get_gear (0, "yedeng")) |\
                   SHUIYA_POSITION_BYTE4_BIT4(shuiya) |\
                   FENGWEN_POSITION_BYTE4_BIT0(get_gear (0, "fengwen"));


  rf_pack->byte5 = rf_pack->byte1+\
                   rf_pack->byte2+\
                   rf_pack->byte3+\
                   rf_pack->byte4;

  rf_pack->unused0 = 0;
  rf_pack->unused1 = 0;
  rf_pack->unused2 = 0;
}

void fix_xuexi_pack_with_user_value (rf_package_t *rf_pack, u8 byte2, u8 byte3)
{
  rf_pack->rf_len1     = 0;
  rf_pack->vid         = 0x5453;
  rf_pack->pid         = uid;
  rf_pack->control_key = 0;
  rf_pack->rf_seq_no   = 0;

  rf_pack->start_code    = 0x4c;

  rf_pack->byte1         = function_code[XUEXI];
  rf_pack->byte2         = byte2;

  rf_pack->byte3         = byte3;

  rf_pack->byte4 = YEDENG_POSITION_BYTE4_BIT7(get_gear (0, "yedeng")) |\
                   SHUIYA_POSITION_BYTE4_BIT4(shuiya) |\
                   FENGWEN_POSITION_BYTE4_BIT0(get_gear (0, "fengwen"));

  rf_pack->byte5 = rf_pack->byte1+\
                   rf_pack->byte2+\
                   rf_pack->byte3+\
                   rf_pack->byte4;

  rf_pack->unused0 = 0;
  rf_pack->unused1 = 0;
  rf_pack->unused2 = 0;
}

void peidui (rf_package_t *rf_pack)
{
  gen_random_id(&new_id);

  rf_pack->rf_len1       = 0;
  rf_pack->vid           = 0x5453;
  rf_pack->pid           = new_id;
  rf_pack->control_key   = PAIR_KEY_VALUE;
  rf_pack->rf_seq_no     = 0;

  rf_pack->start_code    = 0;

  rf_pack->byte1         = 0;
  rf_pack->byte2         = 0;
  rf_pack->byte3         = 0;
  rf_pack->byte4         = 0;
  rf_pack->byte5         = 0;

  rf_pack->unused0 = 0;
  rf_pack->unused1 = 0;
  rf_pack->unused2 = 0;
}
