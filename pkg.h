#ifndef __PKG_H__
#define __PKG_H__

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

extern void fix_pack_with_user_value(rf_package_t *rf_pack, function_t fn);
extern void fix_xuexi_pack_with_user_value(rf_package_t *rf_pack, u8 byte2, u8 byte3);
extern void peidui(rf_package_t *rf_pack);

#endif
