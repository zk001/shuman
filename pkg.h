#ifndef __PKG_H__
#define __PKG_H__

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

typedef enum {
  TINGZHI = 0,    //ֹͣ
  TUNXI,	      //��ϴ
  ZHINENGWENKONG, //�����¿�
  FUXI,	          //��ϴ
  HONGGAN,	      //���
  YIDONGANMO,	  //�ƶ���Ħ
  QIANGRUOANMO,	  //ǿ����Ħ
  FUXIQUANZIDONG, //��ϴȫ�Զ�
  TUNXIYIDONG,	  //��ϴ�ƶ�
  PENZUIQINGJIE,  //������ϴ���Զ�������
  QIANGXI,        //ǿϴ
  FUXIYIDONG,	  //��ϴ�ƶ�
  QUANZIDONG,	  //ȫ�Զ�
  TONGXI,	      //ͯϴ
  QUANPAI,	      //��ˢ��ˮȫ��
  BANPAI,	      //��ˢ��ˮ����
  FANGAIQUANGUAN, //����ȫ��
  FANGAIBANKAI,	  //���ǰ뿪
  CHUCHOU,	      //����
  FANGAIQUANKAI,	//����ȫ��
  ZHINENGYEDENG,	//����ҹ��
  ZHINENGJIEDIAN,	//���ܽڵ�
  SHOUDONGBAOYANG,	//�ֶ�����
  NOTHING,	        //NOTHING
  CHONGXITIAOJIE,	//��ϴ���ڣ�λ�ú�ˮѹ��
  CANSHUTIAOJIE,	//�������ڣ�ˮ��/����/����/ҹ�ƿ���/ʡ��ģʽ��
  CANSHUGENXIN,	    //�������£����������죩
  QIANGXIYIDONG,    //ǿϴ�ƶ�
  CHAXUNSHUI,	    //��ѯ��ˮ��ص���������             ������//��ϴ
  BANBEN,	        //��ȡ����汾����ɺ���Ȧ�������   ������//��ϴ�ƶ�
  ZHUANGTAICHAXUN,	//״̬��ѯ
  XUEXI	            //ϵͳ��ʼ��/ѧϰȫ��/ѧϰȫ�ص�����
}function_t;

extern void fix_pack_with_user_value(rf_package_t *rf_pack, function_t fn);
extern void fix_xuexi_pack_with_user_value(rf_package_t *rf_pack, u8 byte2, u8 byte3);
extern void peidui(rf_package_t *rf_pack);

#endif
