
#include "stm32f10x.h"
#ifndef _ALLOC_H_
#define _ALLOC_H_
//////////////////////////////////////////////////////////////////////////////////  
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK ������
//�ڴ���� ����     
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2011/7/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************
//û�и�����Ϣ
//////////////////////////////////////////////////////////////////////////////////   


#ifndef NULL
#define NULL 0
#endif
#define MEM_BLOCK_SIZE   32          //�ڴ���СΪ32�ֽ�
#define MAX_MEM_SIZE   5*1024       //�������ڴ� 10K
#define MEM_ALLOC_TABLE_SIZE MAX_MEM_SIZE/MEM_BLOCK_SIZE //�ڴ���С

//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(void);     //��ʼ��
	u8 (*perused)(void);         //�ڴ�ʹ����
	u8  membase[MAX_MEM_SIZE];   //�ڴ��
	u16 memmap[MEM_ALLOC_TABLE_SIZE];  //�ڴ����״̬��
	u8  memrdy;        //�ڴ�����Ƿ����
};

extern struct _m_mallco_dev mallco_dev;  //��mallco.c���涨��
void mymemset(void *s,u8 c,u32 count);  //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�  
void my_mem_init(void);      //�ڴ�����ʼ������(��/�ڲ�����)
u32 my_mem_malloc(u32 size);     //�ڴ����(�ڲ�����)
u8 my_mem_free(u32 offset);     //�ڴ��ͷ�(�ڲ�����)
u8 my_mem_perused(void);      //����ڴ�ʹ����(��/�ڲ�����)

////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(void *ptr);       //�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u32 size);     //�ڴ����(�ⲿ����)
void *myrealloc(void *ptr,u32 size);  //���·����ڴ�(�ⲿ����)

#endif  /* _ALLOC_H_ */

