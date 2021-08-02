#include "../../drivers.h"
#include "../../genfsk_ll/genfsk_ll.h"
#include "key.h"
#include "app.h"
#include "led.h"
#include "key.h"
#include "board.h"
#include "mac_id.h"
#include "rf.h"
#include "n_timer.h"
#include "main.h"
#include "power_saving.h"
#include "ssd1306_oled.h"
#include "wakeup.h"

#define MAX_FUNCTION 32

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

#define DEFAULT_PENZUI_WEIZHI   5
#define DEFAULT_ZUOWEN          2
#define DEFAULT_SHUIWEN         3
#define DEFAULT_FENGWEN         2
#define DEFAULT_SHUIYA          3
#define DEFAULT_YEDENG          1
#define DEFAULT_GUANGANG        0
#define DEFAULT_ZIDONGFANGGAI   1
#define DEFAULT_SHOUSHIKAIGUAN  0
#define DEFAULT_ZIDONGCHONGSHUA 1
#define DEFAULT_SHENDIAN        1
#define DEFAULT_YEDENG          1

typedef enum {
  TINGZHI = 0,    //停止
  TUNXI,	      //臀洗
  ZHINENGWENKONG, //智能温控
  FUXI,	          //妇洗
  HONGGAN,	      //烘干
  YIDONGANMO,	  //移动按摩
  QIANGRUOANMO,	  //强弱按摩
  FUXIQUANZIDONG, //妇洗全自动
  TUNXIYIDONG,	  //臀洗移动
  PENZUIQINGJIE,  //喷嘴清洗（自动保养）
  QIANGXI,        //强洗
  FUXIYIDONG,	  //妇洗移动
  QUANZIDONG,	  //全自动
  TONGXI,	      //童洗
  QUANPAI,	      //冲刷排水全排
  BANPAI,	      //冲刷排水半排
  FANGAIQUANGUAN, //翻盖全关
  FANGAIBANKAI,	  //翻盖半开
  CHUCHOU,	      //除臭
  FANGAIQUANKAI,	//翻盖全开
  ZHINENGYEDENG,	//智能夜灯
  ZHINENGJIEDIAN,	//智能节电
  SHOUDONGBAOYANG,	//手动保养
  NOTHING,	        //NOTHING
  CHONGXITIAOJIE,	//冲洗调节（位置和水压）
  CANSHUTIAOJIE,	//参数调节（水温/座温/风温/夜灯开关/省电模式）
  CANSHUGENXIN,	    //参数更新（蜂鸣器不响）
  QIANGXIYIDONG,    //强洗移动
  CHAXUNSHUI,	    //查询与水相关的数据命令             服务用//柔洗
  BANBEN,	        //读取软件版本、烘干和座圈相关数据   服务用//柔洗移动
  ZHUANGTAICHAXUN,	//状态查询
  XUEXI	            //系统初始化/学习全开/学习全关等命令
}function_t;

bool is_led_on;

//阴码 列行式 逆向
u8  wen[32] = {
  0x00,0x02,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x02,0x62,0x3C,0x06,0x00,
  0x00,0x00,0x00,0x3F,0x29,0x29,0x29,0x29,0x29,0x3F,0x00,0x00,0x32,0x22,0x04,0x00,/*"温",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  shui[32] = {
  0x00,0x08,0x08,0x18,0x30,0xC0,0x80,0xFE,0xFE,0x03,0x00,0xC0,0x30,0x18,0x08,0x00,
  0x00,0x00,0x0C,0x04,0x02,0x01,0x01,0x7F,0x7F,0x00,0x07,0x04,0x04,0x04,0x04,0x00,/*"水",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  ya[32] = {
  0x00,0x02,0x02,0x12,0x72,0x02,0xFE,0xFE,0x02,0x02,0x02,0x02,0xC0,0xFC,0x06,0x00,
  0x00,0x00,0x21,0x21,0x21,0x21,0x2F,0x2F,0x21,0x21,0x21,0x20,0x3F,0x3F,0x00,0x00,/*"压",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  li[32] = {
  0x00,0x00,0xFC,0x06,0x02,0x02,0x00,0x00,0xC0,0xF0,0x18,0x0C,0x06,0x03,0x02,0x00,
  0x00,0x04,0x07,0x04,0x04,0x04,0x04,0x04,0x7F,0x04,0x04,0x04,0x04,0x04,0x00,0x00,/*"力",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  zuo[32] = {
  0x00,0x82,0xE2,0x22,0xA2,0x62,0xFE,0x22,0xE2,0x22,0x22,0xE2,0x42,0xFC,0x06,0x00,
  0x00,0x00,0x11,0x17,0x11,0x10,0x1F,0x70,0x50,0x15,0x17,0x10,0x10,0x1F,0x00,0x00,/*"座",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  quan[32] = {
  0x00,0xFF,0xFF,0x42,0x5A,0xCA,0xEA,0x6A,0x5A,0x7A,0xF2,0x42,0x42,0xFF,0x00,0x00,
  0x00,0x7F,0x7F,0x40,0x45,0x55,0x4D,0x45,0x5F,0x45,0x5D,0x45,0x41,0x7F,0x40,0x00,/*"圈",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  nuan[32] = {
  0x00,0x83,0xA2,0xBE,0xAC,0xAC,0xF2,0xB3,0x98,0x84,0xF8,0xF8,0x10,0x10,0xF8,0x00,
  0x00,0x02,0x5A,0x4E,0x42,0x5A,0x33,0x2A,0x3A,0x20,0x3F,0x3F,0x21,0x21,0x3F,0x00,/*"暖",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  feng[32] = {
  0x00,0x0E,0x06,0xFC,0x00,0x08,0x38,0xE0,0xE0,0x30,0x18,0x08,0xC0,0x7C,0x07,0x00,
  0x00,0x00,0x00,0x3F,0x20,0x20,0x2E,0x21,0x21,0x23,0x24,0x20,0x3F,0x00,0x00,0x00,/*"风",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  pen[32] = {
  0x00,0x02,0xF2,0x84,0x84,0xB8,0x8C,0x86,0xFA,0x02,0x02,0xF0,0x20,0x20,0xFC,0x00,
  0x00,0x24,0x24,0x2E,0x24,0x7C,0x24,0x2E,0x24,0x24,0x00,0x1F,0x10,0x10,0x1F,0x00,/*"喷",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  zui[32] = {
  0x00,0x00,0x7E,0x6E,0x6A,0xE8,0x7E,0x68,0x68,0xFC,0xC2,0x00,0xF0,0x20,0xF8,0x00,
  0x00,0x04,0x24,0x14,0x7D,0x01,0x15,0x7F,0x05,0x3C,0x02,0x00,0x1F,0x10,0x1F,0x00,/*"嘴",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  wei[32] = {
  0x00,0x04,0x04,0x84,0xFC,0x0C,0x04,0x74,0xCC,0x04,0x04,0x00,0xFF,0x00,0x80,0x00,
  0x00,0x08,0x08,0x0B,0x09,0x28,0x78,0x08,0x0B,0x08,0x00,0x60,0x3F,0x06,0x01,0x00,/*"位",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  zhi[32] = {
  0x00,0x02,0x02,0x02,0xFE,0x5A,0x5A,0x5A,0x5A,0x5A,0x5A,0xFE,0x02,0x02,0x02,0x00,
  0x00,0x00,0x3A,0x2B,0x2B,0x2B,0x3B,0x2F,0x2F,0x3B,0x2B,0x2B,0x3A,0x3A,0x00,0x00,/*"置",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  ye[32] = {
  0x00,0x02,0x02,0xC2,0x76,0x5C,0xCC,0x3C,0x62,0xC2,0x42,0x00,0xFF,0xC0,0x60,0x00,
  0x00,0x10,0x12,0x13,0x12,0x12,0x12,0x7E,0x77,0x10,0x10,0x1E,0x13,0x10,0x10,0x00,/*"夜",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  deng[32] = {
  0x00,0x00,0x00,0x00,0xFE,0x03,0x02,0x00,0x08,0x18,0x30,0xE0,0x78,0x0E,0x82,0x00,
  0x00,0x20,0x20,0x20,0x3F,0x20,0x20,0x20,0x24,0x04,0x02,0x7F,0x00,0x00,0x0F,0x00,/*"灯",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  guan[32] = {
  0x00,0x40,0x46,0x46,0x4C,0x48,0x58,0xE0,0xF0,0x58,0x4C,0x44,0x42,0x42,0x42,0x00,
  0x00,0x00,0x0C,0x0C,0x0C,0x7C,0x1C,0x0F,0x0F,0x1C,0x7C,0x0C,0x0C,0x0C,0x00,0x00,/*"关",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  kai[32] = {
  0x00,0x80,0x80,0x80,0x80,0xFE,0x80,0x80,0x80,0xC0,0xF8,0x8C,0x86,0x82,0x82,0x00,
  0x00,0x01,0x21,0x21,0x21,0x3F,0x21,0x21,0x21,0x3F,0x3F,0x21,0x21,0x21,0x01,0x00,/*"开",0*/
  /* (16 X 16 , 黑体 )*/
};
u8  du[32] = {
  0x00,0x03,0x02,0x22,0xB2,0xAA,0xA4,0xAC,0xBA,0xB2,0x22,0x21,0x00,0xFC,0x06,0x00,
  0x00,0x00,0x12,0x12,0x1F,0x12,0x12,0x72,0x12,0x1F,0x12,0x12,0x10,0x1F,0x00,0x00,/*"度",0*/
  /* (16 X 16 , 黑体 )*/
};

u8  level1_table[16] = {
  0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"未命名文件",0*/
  /* (8 X 16 )*/
};
u8  level2_table[16] = {
  0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"未命名文件",0*/
  /* (8 X 16 )*/
};
u8  level3_table[16] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,/*"未命名文件",0*/
  /* (8 X 16 )*/
};
u8  level4_table[16] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F,/*"未命名文件",0*/
  /* (8 X 16 )*/
};
u8  level5_table[16] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,0x7F,/*"未命名文件",0*/
  /* (8 X 16 )*/
};
u8 const kuang[] = {
  0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//0-15
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//16-31
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//32-47
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//48-63
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//64-79
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//80-95
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,//96-111
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0xFF,//112-127
  0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//128-143
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//144-159
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//160-175
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//176-191
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//192-207
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//208-223
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,//224-239
  0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xFF,//240-255
  /* (128 X 16 )*/
};

u8  clear_level[16] = {0};
u8  clear_kai_guan[32] = {0};
u8 *all_level[5] = {level1_table, level2_table, level3_table, level4_table, level5_table};

_attribute_data_retention_ static u32 uid;
_attribute_data_retention_ static u32 new_id;

u8 const function_code[MAX_FUNCTION]= {
  0,//停止
  1,//臀洗
  2,//智能温控
  3,//妇洗
  4,//烘干
  5,//移动按摩
  6,//强弱按摩
  7,//妇洗全自动
  8,//臀洗移动
  9,//喷嘴清洗（自动保养）
  10,//强洗
  11,//妇洗移动
  12,//全自动
  13,//童洗
  14,//冲刷排水全排
  15,//冲刷排水半排
  16,//翻盖全关
  17,//翻盖半开
  18,//除臭
  19,//翻盖全开
  20,//智能夜灯
  21,//智能节电
  22,//手动保养
  23,
  24,//冲洗调节（位置和水压）
  25,//参数调节（水温/座温/风温/夜灯开关/省电模式）
  26,//参数更新（蜂鸣器不响）   
  27,//强洗移动
  28,//查询与水相关的数据命令             服务用//柔洗
  29,//读取软件版本、烘干和座圈相关数据   服务用//柔洗移动
  30,//状态查询
  31,//系统初始化/学习全开/学习全关等命令
};

typedef enum{
	TINGZHI_TUNXI_ZHUBIAN_MODE = 1,
	FUXI_MODE,
}now_mode_t;

_attribute_data_retention_ now_mode_t now_current_mode;

typedef struct {
  u8 shuiya;
  u8 penzui;
  u8 fengwen;
  u8 shuiwen;
  u8 zuowen;
  u8 guangang;
  u8 zidongfangai;
  u8 shoushikaiguan;
  u8 zidongchongshua;
  u8 shendian;
  u8 yedeng;
}user_value_t __attribute__ ((aligned (4)));

_attribute_data_retention_ user_value_t *p_cur_user_value;
_attribute_data_retention_ user_value_t user_value_tingzhi_tunxi_zhubian;
_attribute_data_retention_ user_value_t user_value_fuxi;

typedef enum{
  SHUIYA_INDEX = 1,
  PENZUI_INDEX,
  FENGWEN_INDEX,
  SHUIWEN_INDEX,
  ZUOWEN_INDEX,
  GUANGANG_INDEX,
  ZIDONG_FANGAI_INDEX,
  SHOUSHIKAIGUAN_INDEX,
  ZIDONGCHONGSHUA_INDEX,
  SHENDIAN_INDEX,
  YEDENG_INDEX
}user_en_t;

void set_user_value(user_value_t* user_value, user_en_t u_val, u8 val)
{
  switch(u_val){
    case SHUIYA_INDEX:   user_value->shuiya   = val;break;
    case PENZUI_INDEX:   user_value->penzui   = val;break;
    case FENGWEN_INDEX:  user_value->fengwen  = val;break;
    case SHUIWEN_INDEX:  user_value->shuiwen  = val;break;
    case ZUOWEN_INDEX:   user_value->zuowen   = val;break;
    case GUANGANG_INDEX: user_value->guangang = val;break;
    case ZIDONG_FANGAI_INDEX:  user_value->zidongfangai = val;break;
    case SHOUSHIKAIGUAN_INDEX: user_value->shoushikaiguan = val;break;
    case ZIDONGCHONGSHUA_INDEX:user_value->zidongchongshua = val;break;
    case SHENDIAN_INDEX:       user_value->shendian = val;break;
    case YEDENG_INDEX:         user_value->yedeng = val;break;
    default:break;
  }
}

u8 get_user_value(user_value_t* user_value, user_en_t u_val)
{
  u8 r_val;
  switch(u_val){
    case SHUIYA_INDEX:    r_val = user_value->shuiya;break;
    case PENZUI_INDEX:    r_val = user_value->penzui;break;
    case FENGWEN_INDEX:   r_val = user_value->fengwen;break;
    case SHUIWEN_INDEX:   r_val = user_value->shuiwen;break;
    case ZUOWEN_INDEX:    r_val = user_value->zuowen;break;
    case GUANGANG_INDEX:  r_val = user_value->guangang;break;
    case ZIDONG_FANGAI_INDEX:   r_val = user_value->zidongfangai;break;
    case SHOUSHIKAIGUAN_INDEX:  r_val = user_value->shoushikaiguan;break;
    case ZIDONGCHONGSHUA_INDEX: r_val = user_value->zidongchongshua;break;
    case SHENDIAN_INDEX:        r_val = user_value->shendian;break;
    case YEDENG_INDEX:          r_val = user_value->yedeng;break;
    default:r_val = 0;break;
  }

  return r_val;
}

void set_default_user_value()
{
	user_value_tingzhi_tunxi_zhubian.shuiya    = DEFAULT_SHUIYA;
	user_value_tingzhi_tunxi_zhubian.penzui    = DEFAULT_PENZUI_WEIZHI;
	user_value_tingzhi_tunxi_zhubian.fengwen   = DEFAULT_FENGWEN;
	user_value_tingzhi_tunxi_zhubian.shuiwen   = DEFAULT_SHUIWEN;
	user_value_tingzhi_tunxi_zhubian.zuowen    = DEFAULT_ZUOWEN;
	user_value_tingzhi_tunxi_zhubian.guangang  = DEFAULT_GUANGANG;
	user_value_tingzhi_tunxi_zhubian.zidongfangai    = DEFAULT_ZIDONGFANGGAI;
	user_value_tingzhi_tunxi_zhubian.shoushikaiguan  = DEFAULT_SHOUSHIKAIGUAN;
	user_value_tingzhi_tunxi_zhubian.zidongchongshua = DEFAULT_ZIDONGCHONGSHUA;
	user_value_tingzhi_tunxi_zhubian.shendian  = DEFAULT_SHENDIAN;
	user_value_tingzhi_tunxi_zhubian.yedeng    = DEFAULT_YEDENG;

	user_value_fuxi.shuiya    = DEFAULT_SHUIYA;
	user_value_fuxi.penzui    = DEFAULT_PENZUI_WEIZHI;
	user_value_fuxi.fengwen   = DEFAULT_FENGWEN;
	user_value_fuxi.shuiwen   = DEFAULT_SHUIWEN;
	user_value_fuxi.zuowen    = DEFAULT_ZUOWEN;
	user_value_fuxi.guangang  = DEFAULT_GUANGANG;
	user_value_fuxi.zidongfangai    = DEFAULT_ZIDONGFANGGAI;
	user_value_fuxi.shoushikaiguan  = DEFAULT_SHOUSHIKAIGUAN;
	user_value_fuxi.zidongchongshua = DEFAULT_ZIDONGCHONGSHUA;
	user_value_fuxi.shendian  = DEFAULT_SHENDIAN;
	user_value_fuxi.yedeng    = DEFAULT_YEDENG;
}

void fix_pack_with_user_value(rf_package_t *rf_pack, function_t fn)
{
  u8 fun = fn;

  rf_pack->rf_len1     = 0;
  rf_pack->vid         = 0x5453;


  rf_pack->pid           = uid;
  rf_pack->control_key   = 0;

  rf_pack->rf_seq_no     = 0;

  rf_pack->start_code    = 0x4c;

  rf_pack->byte1         = function_code[fun];
  rf_pack->byte2         = SHUIWEN_POSITION_BYTE2_BIT4(p_cur_user_value->shuiwen) |\
                           ZIDONGFANGAI_POSITION_BYTE2_BIT3(p_cur_user_value->zidongfangai) |\
                           SHOUSHIKAIGUAN_POSITION_BYTE2_BIT2(p_cur_user_value->shoushikaiguan) |\
                           ZIDONGCHONGSHUA_POSITION_BYTE2_BIT1(p_cur_user_value->zidongchongshua) |\
                           GUANGANG_POSITION_BYTE2_BIT1(p_cur_user_value->guangang);

  rf_pack->byte3         = SHENDIANMOSHI_POSITION_BYTE3_BIT7(p_cur_user_value->shendian) |\
                           PENZUIWEIZHI_POSITION_BYTE3_BIT4(p_cur_user_value->penzui) |\
                           ZUOWEN_POSITION_BYTE3_BIT0(p_cur_user_value->zuowen);

  rf_pack->byte4 = YEDENG_POSITION_BYTE4_BIT7(p_cur_user_value->yedeng) |\
                   SHUIYA_POSITION_BYTE4_BIT4(p_cur_user_value->shuiya) |\
                   FENGWEN_POSITION_BYTE4_BIT0(p_cur_user_value->fengwen);


  rf_pack->byte5 = rf_pack->byte1+\
                   rf_pack->byte2+\
                   rf_pack->byte3+\
                   rf_pack->byte4;

  rf_pack->unused0 = 0;
  rf_pack->unused1 = 0;
  rf_pack->unused2 = 0;
}

void fix_xuexi_pack_with_user_value(rf_package_t *rf_pack, u8 byte2, u8 byte3)
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

  rf_pack->byte4 = YEDENG_POSITION_BYTE4_BIT7(p_cur_user_value->yedeng) |\
                   SHUIYA_POSITION_BYTE4_BIT4(p_cur_user_value->shuiya) |\
                   FENGWEN_POSITION_BYTE4_BIT0(p_cur_user_value->fengwen);

  rf_pack->byte5 = rf_pack->byte1+\
                   rf_pack->byte2+\
                   rf_pack->byte3+\
                   rf_pack->byte4;

  rf_pack->unused0 = 0;
  rf_pack->unused1 = 0;
  rf_pack->unused2 = 0;
}

#define UP   1
#define DOWN 2

typedef enum {
  WENSHUI_WENDU = 1,
  WENSHUI_YALI,
  ZUOQUAN_WENDU,
  NUANFENG_WENDU,
  PENZUI_WEIZHI,
  YEDENG_KAIGUAN
}mode_t;

typedef struct {
  mode_t mode;
  u8 count_mode;
  u8 level;
  u8 min_level;
  u8 max_level;
}jiumu_mode_t;

#define FIRST_PAGE  3
#define SECOND_PAGE 1

#define START_POS 111
#define HIGH  16
#define WIDTH (16 + 1)
#define PAGE 2

typedef struct {
  jiumu_mode_t * current;
  u8 is_clear;
  u8 is_first;
  u8 page;
}display_t;

int display(display_t *ptr_mode);
void or_kuang(u8 *dp, u8 col);
void clear_kuang(u8 *dp, u8 col);
void or_level(u8 *dp, u8 col);
void or_level_with_kuang(u8 *dp, u8 col);
void clear_level_with_kuang(u8 *dp, u8 col);

_attribute_data_retention_ static jiumu_mode_t set_mode[6] = {
  {WENSHUI_WENDU, UP, DEFAULT_SHUIWEN,      SHUIWEN_MIN, SHUIWEN_MAX},
  {WENSHUI_YALI,  UP, DEFAULT_SHUIYA,       SHUIYA_MIN,  SHUIYA_MAX},
  {ZUOQUAN_WENDU, UP, DEFAULT_ZUOWEN,       ZUOWEN_MIN,  ZUOWEN_MAX},
  {NUANFENG_WENDU,UP, DEFAULT_FENGWEN,      FENGWEN_MIN, FENGWEN_MAX},
  {PENZUI_WEIZHI, UP, DEFAULT_PENZUI_WEIZHI,PENZUIWEIZHI_MIN, PENZUIWEIZHI_MAX},
  {YEDENG_KAIGUAN,0,  DEFAULT_YEDENG,       YEDENG_MIN, YEDENG_MAX},
};
_attribute_data_retention_ static jiumu_mode_t set_mode_fuxi[6] = {
  {WENSHUI_WENDU, UP, DEFAULT_SHUIWEN,      SHUIWEN_MIN, SHUIWEN_MAX},
  {WENSHUI_YALI,  UP, DEFAULT_SHUIYA,       SHUIYA_MIN,  SHUIYA_MAX},
  {ZUOQUAN_WENDU, UP, DEFAULT_ZUOWEN,       ZUOWEN_MIN,  ZUOWEN_MAX},
  {NUANFENG_WENDU,UP, DEFAULT_FENGWEN,      FENGWEN_MIN, FENGWEN_MAX},
  {PENZUI_WEIZHI, UP, DEFAULT_PENZUI_WEIZHI,PENZUIWEIZHI_MIN, PENZUIWEIZHI_MAX},
  {YEDENG_KAIGUAN,0,  DEFAULT_YEDENG,       YEDENG_MIN, YEDENG_MAX},
};

_attribute_data_retention_ jiumu_mode_t *current_mode;
_attribute_data_retention_ jiumu_mode_t *next_mode;

_attribute_data_retention_ jiumu_mode_t *current_mode_tingzhi_tunxi_zhubian = &set_mode[0];
_attribute_data_retention_ jiumu_mode_t *next_mode_tingzhi_tunxi_zhubian = &set_mode[1];

_attribute_data_retention_ jiumu_mode_t *current_mode_fuxi = &set_mode_fuxi[0];
_attribute_data_retention_ jiumu_mode_t *next_mode_fuxi = &set_mode_fuxi[1];

void display_all(bool is_cur_clr, bool is_next_clr)
{
  u32 level1;
  u32 level2;

  level1 = current_mode_tingzhi_tunxi_zhubian->level;
  if(current_mode_tingzhi_tunxi_zhubian->mode == NUANFENG_WENDU){
	if(current_mode_tingzhi_tunxi_zhubian->level == 2){
		current_mode_tingzhi_tunxi_zhubian->level = 3;
	}else if(current_mode_tingzhi_tunxi_zhubian->level == 3)
		current_mode_tingzhi_tunxi_zhubian->level = 5;
  }

  level2 = next_mode_tingzhi_tunxi_zhubian->level;
  if(next_mode_tingzhi_tunxi_zhubian->mode == NUANFENG_WENDU){
	if(next_mode_tingzhi_tunxi_zhubian->level == 2){
		next_mode_tingzhi_tunxi_zhubian->level = 3;
	}else if(next_mode_tingzhi_tunxi_zhubian->level == 3)
		next_mode_tingzhi_tunxi_zhubian->level = 5;
  }

  u32 level3;
  u32 level4;

  level3 = current_mode_fuxi->level;
  if(current_mode_fuxi->mode == NUANFENG_WENDU){
	if(current_mode_fuxi->level == 2){
		current_mode_fuxi->level = 3;
	}else if(current_mode_fuxi->level == 3)
		current_mode_fuxi->level = 5;
  }

  level4 = next_mode_fuxi->level;
  if(next_mode_fuxi->mode == NUANFENG_WENDU){
	if(next_mode_fuxi->level == 2){
		next_mode_fuxi->level = 3;
	}else if(next_mode_fuxi->level == 3)
		next_mode_fuxi->level = 5;
  }

  display_t first_display  = {current_mode, 1, 1, FIRST_PAGE};
  first_display.is_clear = is_cur_clr;
  display(&first_display);

  display_t second_display = {next_mode, 1, 0, SECOND_PAGE};
  second_display.is_clear = is_next_clr;
  display(&second_display);

  current_mode_tingzhi_tunxi_zhubian->level = level1;
  next_mode_tingzhi_tunxi_zhubian->level = level2;
  current_mode_fuxi->level = level3;
  next_mode_fuxi->level = level4;
}

void is_clear(bool *cur, bool *next)
{
  if(current_mode->mode == PENZUI_WEIZHI &&\
	  (current_mode_tingzhi_tunxi_zhubian->level != current_mode_fuxi->level)){
	  *cur = 1;
  }

  if(current_mode->mode == WENSHUI_YALI &&\
	(current_mode_tingzhi_tunxi_zhubian->level != current_mode_fuxi->level)){
	  *cur = 1;
  }

  if(next_mode->mode == PENZUI_WEIZHI &&\
	(current_mode_tingzhi_tunxi_zhubian->level != current_mode_fuxi->level)){
	  *next = 1;
  }
  if(next_mode->mode == WENSHUI_YALI &&\
	(current_mode_tingzhi_tunxi_zhubian->level != current_mode_fuxi->level)){
	  *next = 1;
  }
}

//"key action" "which key" "cmd-code"
void short_key_tingzhi()
{
  rf_package_t rf_pack;
  bool is_clr_cur = 0;
  bool is_clr_next = 0;

  is_clear(&is_clr_cur, &is_clr_next);

  now_current_mode = TINGZHI_TUNXI_ZHUBIAN_MODE;
  p_cur_user_value = &user_value_tingzhi_tunxi_zhubian;
  current_mode = current_mode_tingzhi_tunxi_zhubian;
  next_mode = next_mode_tingzhi_tunxi_zhubian;

  fix_pack_with_user_value(&rf_pack, TINGZHI);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

  if(is_led_on)
    display_all(is_clr_cur, is_clr_next);
}

void long_key_tunxi_zidongqingjie()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value(&rf_pack, PENZUIQINGJIE);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void short_key_tunxi()
{
  rf_package_t rf_pack;
  bool is_clr_cur = 0;
  bool is_clr_next = 0;

  is_clear(&is_clr_cur, &is_clr_next);

  now_current_mode = TINGZHI_TUNXI_ZHUBIAN_MODE;
  p_cur_user_value = &user_value_tingzhi_tunxi_zhubian;
  current_mode = current_mode_tingzhi_tunxi_zhubian;
  next_mode = next_mode_tingzhi_tunxi_zhubian;

  fix_pack_with_user_value(&rf_pack, TUNXIYIDONG);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

  if(is_led_on)
    display_all(is_clr_cur, is_clr_next);
}

void short_key_zhubian()
{
  rf_package_t rf_pack;
  bool is_clr_cur;
  bool is_clr_next;

  is_clear(&is_clr_cur, &is_clr_next);

  now_current_mode = TINGZHI_TUNXI_ZHUBIAN_MODE;

  p_cur_user_value = &user_value_tingzhi_tunxi_zhubian;
  current_mode = current_mode_tingzhi_tunxi_zhubian;
  next_mode = next_mode_tingzhi_tunxi_zhubian;

  fix_pack_with_user_value(&rf_pack, QIANGXIYIDONG);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

  if(is_led_on)
    display_all(is_clr_cur, is_clr_next);
}

void short_key_fuxi()
{
  rf_package_t rf_pack;
  bool is_clr_cur;
  bool is_clr_next;

  is_clear(&is_clr_cur, &is_clr_next);

  now_current_mode = FUXI_MODE;

  p_cur_user_value = &user_value_fuxi;
  current_mode = current_mode_fuxi;
  next_mode = next_mode_fuxi;

  fix_pack_with_user_value(&rf_pack, FUXIYIDONG);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

  if(is_led_on)
    display_all(is_clr_cur, is_clr_next);
}

void short_key_nuanfeng()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value(&rf_pack, HONGGAN);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void long_key_xiaochong_jiaogan()
{
  rf_package_t rf_pack;
  static u8 kai_guan = 1;

  //	BYTE3:bit1：脚感功能关 1：开启0：无效 10 关  0x02
  //	BYTE3:bit0：脚感功能开 1：开启0：无效 01 开  0x01
  //10 关
  //01 开

  if(kai_guan){
    fix_xuexi_pack_with_user_value(&rf_pack, 0x24, 0x02);//guan
  }else{
    fix_xuexi_pack_with_user_value(&rf_pack, 0x24, 0x01);//kai
  }

  kai_guan ^= 1;

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void short_key_xiaochong()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value(&rf_pack, BANPAI);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

static bool chongshua_val  = 1;

void long_key_dachong_zidongchongshua_kai()
{
  rf_package_t rf_pack;

  if(!chongshua_val){//open
	  chongshua_val = 1;

	  set_user_value(p_cur_user_value, ZIDONGCHONGSHUA_INDEX, chongshua_val);
	  fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
	  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
  }
}

void long_key_dachong_zidongchongshua_guan()
{
  rf_package_t rf_pack;

  if(chongshua_val){//open
	  chongshua_val = 0;

	set_user_value(p_cur_user_value, ZIDONGCHONGSHUA_INDEX, chongshua_val);
	fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
	send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
  }
}

void short_key_dachong()
{
  rf_package_t rf_pack;

  fix_pack_with_user_value(&rf_pack, QUANPAI);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

static bool zhinengjiedian_val = 1;

void long_key_fangai_zhinengjiedian_kai()
{
  rf_package_t rf_pack;

  if(!zhinengjiedian_val){//open
	  zhinengjiedian_val = 1;
	set_user_value(p_cur_user_value, SHENDIAN_INDEX, zhinengjiedian_val);
	fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
	send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
  }
}

void long_key_fangai_zhinengjiedian_guan()
{
  rf_package_t rf_pack;

  if(zhinengjiedian_val){//open
	  zhinengjiedian_val = 0;
	set_user_value(p_cur_user_value, SHENDIAN_INDEX, zhinengjiedian_val);
	fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
	send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
  }
}


void short_key_fanquang()
{
  rf_package_t rf_pack;

  fix_xuexi_pack_with_user_value(&rf_pack, 0x5e, 0x24);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void long_key_fanquang_zidongfangai()
{
  rf_package_t rf_pack;

  static u8 fangai = 1;

  fangai ^= 1;

  set_user_value(p_cur_user_value, ZIDONG_FANGAI_INDEX, fangai);

  fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void short_key_fangai()
{
  rf_package_t rf_pack;

  fix_xuexi_pack_with_user_value(&rf_pack, 0x5e, 0x23);

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
}

void short_key_set()
{
//  jiumu_mode_t *ptr;
//  jiumu_mode_t *other_ptr;
//  jiumu_mode_t *other_next_ptr;
//
//  jiumu_mode_t **ptr1;
//  jiumu_mode_t **ptr2;
//  jiumu_mode_t **ptr3;
//  jiumu_mode_t **ptr4;


//  ptr = &set_mode[0];

//  if(current_mode == (ptr + 5)){
//    current_mode = ptr;
//    *ptr1 = ptr;
//  }
//  else{
//    current_mode += 1;
//    *ptr1 = current_mode;
//  }
//
//
//  if(next_mode == (ptr + 5)){
//    next_mode = ptr;
//    *ptr2 = ptr;
//  }
//  else{
//    next_mode += 1;
//    *ptr2 = next_mode;
//  }

  if(!is_wakeup_from_sleep()){
  if(current_mode_tingzhi_tunxi_zhubian == &set_mode[5]){
	  current_mode_tingzhi_tunxi_zhubian = &set_mode[0];
  }else{
	  current_mode_tingzhi_tunxi_zhubian++;
  }

  if(next_mode_tingzhi_tunxi_zhubian == &set_mode[5]){
	  next_mode_tingzhi_tunxi_zhubian = &set_mode[0];
  }else{
	  next_mode_tingzhi_tunxi_zhubian++;
  }

  if(current_mode_fuxi == &set_mode_fuxi[5]){
	  current_mode_fuxi = &set_mode_fuxi[0];
  }else{
	  current_mode_fuxi++;
  }

  if(next_mode_fuxi == &set_mode_fuxi[5]){
	  next_mode_fuxi = &set_mode_fuxi[0];
  }else{
	  next_mode_fuxi++;
  }

  if(now_current_mode == TINGZHI_TUNXI_ZHUBIAN_MODE){
	  current_mode  = current_mode_tingzhi_tunxi_zhubian;
	  next_mode     = next_mode_tingzhi_tunxi_zhubian;
  }else{
	  current_mode = current_mode_fuxi;
	  next_mode = next_mode_fuxi;
  }
  }

  if(!is_led_on){
    is_led_on = 1;
	display_kuang();
  }
  display_all(1, 1);
}

u8 next_pos(u8 col)
{
  col += WIDTH;
  return col;
}

u8 rever_next_pos(u8 col)
{
  col -= WIDTH;
  return col;
}

//#define STRING_START_POS 12
//#define STRING_HIGH 12
//#define STRING_WIDTH  12
#define LEVEL_LENGTH (8 + 1)

void clear_level_indi(u8 page, u8 col)
{
  for(u8 i = 0; i < 5; i++){
    display_graphic_8x16(page, col, (u8*)clear_level);
    col -= LEVEL_LENGTH;
  }
}

#define KAI_GUAN_POS(col) (col - 20)

int display(display_t *ptr_mode)
{
  u8 first_pos;
  u8 second_pos;
  u8 third_pos;
  u8 fourth_pos;
  jiumu_mode_t * current_disply;
  u8 page;
  u8 is_clear;
  u8 is_first;

  u8 *first  = NULL;
  u8 *second = NULL;
  u8 *third  = NULL;
  u8 *fourth = NULL;

  current_disply = ptr_mode->current;
  page           = ptr_mode->page;
  is_clear       = ptr_mode->is_clear;
  is_first       = ptr_mode->is_first;

  switch(current_disply->mode){
    case WENSHUI_WENDU:
      first  = wen;
      second = shui;
      third  = wen;
      fourth = du;break;
    case WENSHUI_YALI:
      first  = wen;
      second = shui;
      third  = ya;
      fourth = li;break;
    case ZUOQUAN_WENDU:
      first  = zuo;
      second = quan;
      third  = wen;
      fourth = du;break;
    case NUANFENG_WENDU:
      first  = nuan;
      second = feng;
      third  = wen;
      fourth = du;break;
    case PENZUI_WEIZHI:
      first  = pen;
      second = zui;
      third  = wei;
      fourth = zhi;break;
    case YEDENG_KAIGUAN:
      first  = ye;
      second = deng;
      third  = kai;
      fourth = guan;
    default:break;
  }

  first_pos  = START_POS;
  second_pos = rever_next_pos(first_pos);
  third_pos  = rever_next_pos(second_pos);
  fourth_pos = rever_next_pos(third_pos);

  if(is_first){
    or_kuang(first, first_pos);
    or_kuang(second,second_pos);
    or_kuang(third, third_pos);
    or_kuang(fourth,fourth_pos);
    or_kuang(clear_kai_guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    or_kuang(kai, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    or_kuang(guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    u8 k = rever_next_pos(fourth_pos);
    for(u8 i = 0; i < 5; i++){
      or_level_with_kuang(clear_level, k);
      k -= LEVEL_LENGTH;
    }
  }else{
    clear_kuang(first,  first_pos);
    clear_kuang(second, second_pos);
    clear_kuang(third,  third_pos);
    clear_kuang(fourth, fourth_pos);
    clear_kuang(clear_kai_guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    clear_kuang(guan, KAI_GUAN_POS(rever_next_pos(fourth_pos)));
    clear_kuang(kai, KAI_GUAN_POS(rever_next_pos(fourth_pos)));

    u8 k = rever_next_pos(fourth_pos);
    for(u8 i = 0; i < 5; i++){
      clear_level_with_kuang(clear_level, k);
      k -= LEVEL_LENGTH;
    }
  }

  display_graphic_16x16_2(1, page, first_pos,  (u8*)first);

  display_graphic_16x16_2(1, page, second_pos, (u8*)second);

  display_graphic_16x16_2(1, page, third_pos,  (u8*)third);

  display_graphic_16x16_2(1, page, fourth_pos, (u8*)fourth);

  if(is_clear){//clear adjust area
	clear_level_indi(page, rever_next_pos(fourth_pos));//clear level
	display_graphic_16x16_2(1, page, KAI_GUAN_POS(rever_next_pos(fourth_pos)), (u8*)clear_kai_guan);//clear kai or guan
  }

  if(!current_disply->level){
    display_graphic_16x16_2(1, page, KAI_GUAN_POS(rever_next_pos(fourth_pos)), (u8*)guan);
  }else {
    if(current_disply->mode == YEDENG_KAIGUAN){
      display_graphic_16x16_2(1, page, KAI_GUAN_POS(rever_next_pos(fourth_pos)), (u8*)kai);
    }else{
      u8 k = rever_next_pos(fourth_pos);
      for(u8 i = 0; i < current_disply->level; i++){
        if(is_first)
          or_level_with_kuang(all_level[i], k);
        //add kuang
        else
          //clear kuang
          clear_level_with_kuang(all_level[i], k);

        display_graphic_8x16(page, k, (u8*)all_level[i]);
        k -= LEVEL_LENGTH;
      }
    }
  }
  return 0;
}

void short_key_plus()
{
  rf_package_t rf_pack;
  if(!is_wakeup_from_sleep()){

  if((current_mode->mode != WENSHUI_YALI) && (current_mode->mode != PENZUI_WEIZHI)){
	  if(current_mode_tingzhi_tunxi_zhubian->level == current_mode_tingzhi_tunxi_zhubian->max_level)
		  current_mode_tingzhi_tunxi_zhubian->count_mode = DOWN;
	  else if(current_mode_tingzhi_tunxi_zhubian->level == current_mode_tingzhi_tunxi_zhubian->min_level)
		  current_mode_tingzhi_tunxi_zhubian->count_mode = UP;

	  if(current_mode_tingzhi_tunxi_zhubian->count_mode == UP)
		  current_mode_tingzhi_tunxi_zhubian->level++;
	  else
		  current_mode_tingzhi_tunxi_zhubian->level--;

	  if(current_mode_fuxi->level == current_mode_fuxi->max_level)
		  current_mode_fuxi->count_mode = DOWN;
	  else if(current_mode_fuxi->level == current_mode_fuxi->min_level)
		  current_mode_fuxi->count_mode = UP;

	  if(current_mode_fuxi->count_mode == UP)
		  current_mode_fuxi->level++;
	  else
		  current_mode_fuxi->level--;
  }else{
	  if(current_mode->level == current_mode->max_level)
		current_mode->count_mode = DOWN;
	  else if(current_mode->level == current_mode->min_level)
		current_mode->count_mode = UP;

	  if(current_mode->count_mode == UP)
		current_mode->level++;
	  else
		current_mode->level--;
  }

  switch(current_mode->mode){
    case WENSHUI_WENDU:
        set_user_value(&user_value_tingzhi_tunxi_zhubian, SHUIWEN_INDEX, current_mode_tingzhi_tunxi_zhubian->level);
        set_user_value(&user_value_fuxi, SHUIWEN_INDEX, current_mode_fuxi->level);
//      set_user_value(p_cur_user_value, SHUIWEN_INDEX, current_mode->level);
      fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
      break;
    case WENSHUI_YALI://0x18
      set_user_value(p_cur_user_value, SHUIYA_INDEX,  current_mode->level);
      fix_pack_with_user_value(&rf_pack, CHONGXITIAOJIE);
      break;
    case ZUOQUAN_WENDU:
        set_user_value(&user_value_tingzhi_tunxi_zhubian, ZUOWEN_INDEX,  current_mode_tingzhi_tunxi_zhubian->level);
        set_user_value(&user_value_fuxi, ZUOWEN_INDEX,  current_mode_fuxi->level);

//      set_user_value(p_cur_user_value, ZUOWEN_INDEX,  current_mode->level);
      fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
      break;
    case NUANFENG_WENDU:
        set_user_value(&user_value_tingzhi_tunxi_zhubian, FENGWEN_INDEX, current_mode_tingzhi_tunxi_zhubian->level);
        set_user_value(&user_value_fuxi, FENGWEN_INDEX, current_mode_fuxi->level);

//      set_user_value(p_cur_user_value, FENGWEN_INDEX, current_mode->level);
      fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
      break;
    case PENZUI_WEIZHI://0x18
      set_user_value(p_cur_user_value, PENZUI_INDEX,  current_mode->level);
      fix_pack_with_user_value(&rf_pack, CHONGXITIAOJIE);
      break;
    case YEDENG_KAIGUAN:
        set_user_value(&user_value_tingzhi_tunxi_zhubian, YEDENG_INDEX,  current_mode_tingzhi_tunxi_zhubian->level);
        set_user_value(&user_value_fuxi, YEDENG_INDEX,  current_mode_fuxi->level);

//      set_user_value(p_cur_user_value, YEDENG_INDEX,  current_mode->level);
      fix_pack_with_user_value(&rf_pack, CANSHUTIAOJIE);
      break;
    default:break;
  }

  send_rf_data_kemu(&rf_pack, sizeof(rf_pack));

//  u32 level;
//  level = current_mode->level;
//  if(current_mode->mode == NUANFENG_WENDU){
//    if(current_mode->level == 2){
//      current_mode->level = 3;
//    }else if(current_mode->level == 3)
//      current_mode->level = 5;
//  }

//  u32 level1;
//  u32 level2;
//  level1 = current_mode_tingzhi_tunxi_zhubian->level;
//  if(current_mode_tingzhi_tunxi_zhubian->mode == NUANFENG_WENDU){
//    if(current_mode_tingzhi_tunxi_zhubian->level == 2){
//    	current_mode_tingzhi_tunxi_zhubian->level = 3;
//    }else if(current_mode_tingzhi_tunxi_zhubian->level == 3)
//    	current_mode_tingzhi_tunxi_zhubian->level = 5;
//  }
//
//  level2 = current_mode_fuxi->level;
//  if(current_mode_fuxi->mode == NUANFENG_WENDU){
//    if(current_mode_fuxi->level == 2){
//    	current_mode_fuxi->level = 3;
//    }else if(current_mode_fuxi->level == 3)
//    	current_mode_fuxi->level = 5;
//  }
  }
  if(!is_led_on){
    display_kuang();
    is_led_on = 1;
  }
  display_all(1, 0);

//  current_mode_tingzhi_tunxi_zhubian->level = level1;
//  current_mode_fuxi->level = level2;

}

void peidui(rf_package_t *rf_pack)
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

void display_kuang()
{
  display_graphic_128x16(3, 1, (u8*)kuang);
}

void long_key_tingzhi_peidui()//3s
{
  rf_package_t rf_pack;
  u32 send_period;
  u32 start_time;
  u32 flash_period;
  u8 rx_buf[32] = {0};
  static bool toggle = 1;

  peidui(&rf_pack);

  send_period  = clock_time();
  start_time   = clock_time();
  flash_period = clock_time();

  while(1){
	if(n_clock_time_exceed(start_time, 30000000)){//30s
	  rf_8359_set_tx();
      display_kuang();
      display_all(0, 0);
	  clr_app_read_key_flag();
	  reload_sys_time();
	  return;
	}

	if(n_clock_time_exceed(flash_period, 500000)){//500ms
      flash_period = clock_time();

	  toggle ^= 1;

	  if(toggle){
	    display_kuang();
	    display_all(0, 0);
	  }else
	    clear_screen();
	}

    if(n_clock_time_exceed(send_period, 1000000)){//1S
      decrease_rf_power_tx();
      send_rf_data_kemu(&rf_pack, sizeof(rf_pack));
      rf_8359_set_rx();
      send_period = clock_time();
    }

    if(receive_rf_data(rx_buf)){
      if(rx_buf[7] == 0x88){//received peer code
        rf_8359_set_tx();
        display_kuang();
        display_all(0, 0);
        write_id(&new_id, 4);
        uid = new_id;
        clr_app_read_key_flag();
        reload_sys_time();
        return;
      }
    }

    if(app_read_single_key(KEY_TINGZHI)){
      rf_8359_set_tx();
      display_kuang();
      display_all(0, 0);
      reload_sys_time();
      return;
    }
  }
}

void or_level_with_kuang(u8 *dp, u8 col)
{
  for(u8 i = 0; i < 8; i++)
    dp[i] |= kuang[col + i];

  for(u8 i = 0; i < 8; i++)
    dp[i + 8] |= kuang[128 + col + i];
}

void clear_level_with_kuang(u8 *dp, u8 col)
{
  for(u8 i = 0; i < 8; i++)
    dp[i] &= ~kuang[col + i];

  for(u8 i = 0; i < 8; i++)
    dp[i + 8] &= ~kuang[128 + col + i];
}

void or_kuang(u8 *dp, u8 col)
{
  for(u8 i = 0; i < 16; i++)
    dp[i] |= kuang[col + i];

  for(u8 i = 0; i < 16; i++)
    dp[i + 16] |= kuang[128 + col + i];
}

void clear_kuang(u8 *dp, u8 col)
{
  for(u8 i = 0; i < 16; i++)
    dp[i] &= ~ kuang[i + col];

  for(u8 i = 0; i < 16; i++)
    dp[i+16] &= ~ kuang[i + 128 + col];
}

void app_init()
{
  if(!is_wakeup_from_sleep()){
    set_default_user_value();
    read_id(&uid, 4);
//    now_current_mode = TINGZHI_TUNXI_ZHUBIAN_MODE;
//    p_cur_user_value = &user_value_tingzhi_tunxi_zhubian;
//    current_mode = current_mode_tingzhi_tunxi_zhubian;
//    next_mode = next_mode_tingzhi_tunxi_zhubian;
//	display_kuang();
//	display_all(0, 0);
  }

  now_current_mode = TINGZHI_TUNXI_ZHUBIAN_MODE;
  p_cur_user_value = &user_value_tingzhi_tunxi_zhubian;
  current_mode = current_mode_tingzhi_tunxi_zhubian;
  next_mode = next_mode_tingzhi_tunxi_zhubian;

//  if(cur_key == KEY_SET || cur_key == KEY_PLUS){
//    display_kuang();
//    display_all(0, 0);
//  }

  register_key_event(KEY_TINGZHI,  0, MS2TICK(2000), 0, SHORT_KEY, short_key_tingzhi);
  register_key_event(KEY_TUNXI,    0, MS2TICK(2000), 0, SHORT_KEY, short_key_tunxi);
  register_key_event(KEY_TUNXI,    0, MS2TICK(3000), 0, LONG_KEY, long_key_tunxi_zidongqingjie);

  register_key_event(KEY_ZHUBIAN,  0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_zhubian);
  register_key_event(KEY_FUXI,     0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_fuxi);
  register_key_event(KEY_NUANFENG, 0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_nuanfeng);
  register_key_event(KEY_DACHONG,  0, MS2TICK(2000), 0, SHORT_KEY, short_key_dachong);
  register_key_event(KEY_DACHONG,  0, MS2TICK(3000), 0, LONG_KEY, long_key_dachong_zidongchongshua_kai);
  register_key_event(KEY_DACHONG,  0, 0, 0, LONG_KEY_IN_8S, long_key_dachong_zidongchongshua_guan);

  register_key_event(KEY_XIAOCHONG,0, MS2TICK(3000), 0, SHORT_KEY, short_key_xiaochong);
  register_key_event(KEY_XIAOCHONG,0, MS2TICK(3000), 0, LONG_KEY, long_key_xiaochong_jiaogan);

  register_key_event(KEY_FANQUAN,  0, MS2TICK(3000), 0, SHORT_KEY, short_key_fanquang);
  register_key_event(KEY_FANQUAN,  0, MS2TICK(3000), 0, LONG_KEY, long_key_fanquang_zidongfangai);

  register_key_event(KEY_FANGAI,   0, MS2TICK(3000), 0, SHORT_KEY, short_key_fangai);
  register_key_event(KEY_FANGAI,   0, MS2TICK(3000), 0, LONG_KEY,  long_key_fangai_zhinengjiedian_guan);
  register_key_event(KEY_FANGAI,   0, 0, 0, LONG_KEY_IN_8S,  long_key_fangai_zhinengjiedian_kai);

  register_key_event(KEY_SET,      0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_set);
  register_key_event(KEY_PLUS,     0, 0, 0, SHORT_KEY_IMMEDIATELY, short_key_plus);
  register_key_event(KEY_TINGZHI,  0, MS2TICK(3000), 0, LONG_KEY, long_key_tingzhi_peidui);
}
