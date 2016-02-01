#ifndef	__PROTOCOL_SEND_H_
#define	__PROTOCOL_SEND_H_

extern void pelcod_call_pre_packet_send(u8 val);
extern void pelcod_set_pre_packet_send(u8 val);
extern void pelcod_open_close_packet_send(u8 val);
extern void pelcod_stop_packet_send(void);
extern void pelcod_lrud_pre_packet_send(u8 lrudcmd,u8 lrspeed,u8 udspeed);
extern void pelcod_zf_packet_send(u8 cmd,u8 zfspeed);


/*****************************************************�ɶ���Э�鲿��***********************************************/
////////////////��Э��ͬ����/////////////////////////
#define PELCOD_SYNC_WORD 0xff
#define PELCOP_SYNC_WORD 0xa0
#define PELCOP_ETX_WORD 0xaf
/////////////PELCOЭ��ĸ��ֶ�������/////////////////
#define DP_Left  0x04		 //��ת
#define DP_Right 0x02		 //��ת
#define DP_Up    0x08		 //��ת
#define DP_Down  0x10		 //��ת
#define DP_ZoomWide   0x40	 //�䱶��/���/��С/��Զ
#define DP_ZoomTele   0x20	 //�䱶��/խ��/�Ŵ�/����
#define DP_Left_Wide  0x44   //����С
#define DP_Left_Tele  0x24   //��Ŵ�
#define DP_Right_Wide  0x42   //����С
#define DP_Right_Tele  0x22   //�ҷŴ�
#define DP_Up_Wide     0x48   //����С
#define DP_Up_Tele     0x28   //�ϷŴ�
#define DP_Down_Wide   0x50   //����С
#define DP_Down_Tele   0x30   //�·Ŵ�

#define DP_Left_Up    0x0c      //����
#define DP_Left_Down  0x14      //����
#define DP_Right_Up   0x0a      //����
#define DP_Right_Down 0x12      //����

#define DP_Left_Up_Wide  0x4c   //������С
#define DP_Left_Up_Tele  0x2c   //���ϷŴ�
#define DP_Right_Up_Wide  0x4a   //������С
#define DP_Right_Up_Tele  0x2a   //���ϷŴ�

#define DP_Left_Down_Wide  0x54   //������С
#define DP_Left_Down_Tele  0x34   //���·Ŵ�
#define DP_Right_Down_Wide  0x52   //������С
#define DP_Right_Down_Tele  0x32   //���·Ŵ�

#define DP_Preassign_Place_Set   0x03   //Ԥ��λ����
#define DP_Preassign_Place_Call	 0x07   //Ԥ��λ����
#define DP_Preassign_Place_Del   0x05 	//Ԥ��λɾ��

#define DP_Pattern_Start         0x1f   //��ѧϰ��ʼ
#define DP_Pattern_Stop          0x21   //��ѧϰֹͣ
#define DP_Pattern_Run           0x23   //��ѧϰ����

#define DP_Tour_Run            0x65   //Ѳ������
#define DP_Tour_Stop           0x71   //Ѳ��ֹͣ
#define DP_Tour_Del            0x6d   //ɾ��Ѳ��
#define DP_Tour_SetStart       0x67   //��ʼ����Ѳ����ͬʱ��ID
#define DP_Tour_Data           0x69  //����Ѳ������
#define DP_Tour_SetEnd         0x6b   //����Ѳ������

#define DP_SetZone_Start       0x11   //ˮƽɨ����λ��ʼ����
#define DP_SetZone_End         0x13   //ˮƽɨ����λ���ý���
#define DP_ZoneScan_On         0x1b   //��ˮƽɨ��
#define DP_ZoneScan_Off        0x1d   //��ˮƽɨ��

#define DP_AUX_On              0x09   //���ø�������
#define DP_AUX_Off             0x0B   //�����������

#define D_Auto_Scan              0x90   //�Զ�ɨ��
#define P_Auto_Scan              0x20   //�Զ�ɨ��

#define D_Camera_On              0x88   //�����
#define D_Camera_Off             0x08   //�����

#define P_Camera_On              0x50   //�����
#define P_Camera_Off             0x10   //�����




//////////PELCO-DЭ�������ж�������//////////
#define D_FocusFar   0x80    //�۽�Զ
#define D_IrZoomout  0x04	 //��Ȧ��С
#define D_IrZoomIn   0x02	 //��Ȧ����
#define D_FocusNear  0x01    //�۽���

//////////PELCO-PЭ�������ж�������//////////
#define P_IrZoomout    0x08	   //��Ȧ��С
#define P_IrZoomIn     0x04	   //��Ȧ����
#define P_FocusFar     0x01    //�۽�Զ
#define P_FocusNear    0x02    //�۽���
#define PRORO_INVILID  0x00    //��Чֵ



#endif
