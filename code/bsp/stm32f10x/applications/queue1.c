
#ifndef _QUEUE1_C_
#define _QUEUE1_C_



#include    <locale.h>
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <stdarg.h>

#include "malloc.h"
#include "queue.h"


#define	printf_s	printf
#define	MAX_QUEUE_LIST	15

#ifndef u8
typedef unsigned char u8;
#endif
	

signed char QueueLenght = 0;

queue_link front = NULL;      //��ʼ��ͷָ��
queue_link rear  = NULL;      //��ʼ��βָ��

//�����������
//�����Ѿ�������2���ڴ治������0���������շ���1
u8 addqueue(u8 dataLen,u8 *datapointer)
{
	u8 i;
	
	queue_link new_node;

	new_node = (queue_link) mymalloc(sizeof(queue_list));

		
	if(new_node != NULL)
	{
		if(QueueLenght < MAX_QUEUE_LIST)
		{
			for(i=0;i<dataLen;i++)
				new_node->data[i] = datapointer[i];    //�����ݴ������
				
			new_node->next = NULL;    //���ó�ֵ
			
			if(rear == NULL || front == NULL) //��Ϊ���еĵ�һ������ 
				front = rear = new_node;    //��frontָ���½ڵ�
			else
				rear = rear->next = new_node;   //rear��ָ�Ľڵ�ָ���½ڵ�
			
			QueueLenght++;
			return 1;
		}
		else
		{
			return 2;
		}
		
	}
	else
		return 0;
}


/*------------------------------------------------------------
����Ŀ�ģ� �õ����еĳ���
��ʼ������ ����Q�Ѵ���
��������� ����Q������Ԫ�صĸ���
����������
   LinkQueue Q   ����Q
����ֵ��
   int     ����Ԫ�صĸ���
------------------------------------------------------------*/
int QueueLength(queue_link Q)
{
	int count = 0;

    queue_link p;
    p = front; //pָ��ͷ���
    while(p->next != NULL)
    {
        count++;
        p = p->next;
    }
    return count;
}


//�����������
//�����ݷ���1�������ݷ���0
//type:0,��ʾ����ʽ���ݳ�ȡ��ÿ�ζ�ȡ��������ʱ���ᱣ����ǰ��ȡ�������ڻ�����
//1,��ʾΪ����ʽ��ȡ���ݣ�ÿ�ζ�ȡ���ݺ󣬻�������ͬʱ��ձ���ȡ������
u8 delqueue_extend(u8 type,u8 dataLen,u8 *dst)
{
	queue_link top;
	u8 i;
	
	if(front != NULL)
	{   

		//�������Ƿ�Ϊ��
		top   = front;     //��topָ��front

		if(type)
			front = front->next;   //

		for(i=0;i<dataLen;i++)
			dst[i]  = top->data[i];    //�ݴ������������

		if(type)
		{
			myfree(top);      //�ͷ�����ڵ������
			QueueLenght--;
			if(QueueLenght < 0)
				QueueLenght = 0;
		}
		return 1;

	}
	else
		return 0;
}


//�����������
//�����ݷ���1�������ݷ���0
u8 delqueue(u8 dataLen,u8 *dst)
{
	queue_link top;
	u8 i;
	
	if(front != NULL)
	{ 
		//�������Ƿ�Ϊ��
		top   = front;     //��topָ��front
		front = front->next;   //

		for(i=0;i<dataLen;i++)
			dst[i]  = top->data[i];    //�ݴ������������
		myfree(top);      //�ͷ�����ڵ������

		QueueLenght--;
		if(QueueLenght < 0)
			QueueLenght = 0;
		return 1;

	}
	else
		rear = NULL;
	return 0;
}


void queue_init(void)
{
	my_mem_init();
	front = NULL;
	rear = NULL;
	QueueLenght = 0;
}

#if 0
//���ж���������룬����ӡ����������
int debug_main(void)
{
 int select;        //����ѡ�����
 int  temp;
 queue_link point;
 
 printf_s("(1)Input a data\n");
 printf_s("(2)Output a data\n");
 printf_s("(3)Exit\n");
 printf_s("Please select:");
 scanf("%d", &select);
 printf_s("\n");
 if(select != 3){
  do{
   switch(select){
    case 1: printf_s("Please input the input value == >");
     scanf("%d", &temp);

	 addqueue(temp);      //�����ݲ��������

	if(Keyboard_linkqueue->QueueLenght > MAX_QUEUE_LIST)
	 	printf("The queue is full.\n");

	printf_s("The content of queue:"); //�����������

	point = Keyboard_linkqueue->front;
	while(point != NULL){
	printf_s("[%d]", point->data);
	point = point->next;
     }
     break;
    case 2: if((temp = delqueue()) == -1) //�ж϶����Ƿ�Ϊ��
       printf_s("The queue is empty!!!\n");
      else{
       printf_s("The output value is [%d]\n", temp);
       printf_s("The content of queue:"); //�����������
       point = Keyboard_linkqueue->front;
       while(point != NULL){
        printf_s("[%d]", point->data);
        point = point->next;
       }
      }
      break;
   }
   printf_s("\n(1)Input a data\n");
   printf_s("\n(2)Output a data\n");
   printf_s("\n(3)Exit\n");
   printf_s("Please select:");
   scanf("%d", &select);
   printf_s("\n");
  }while(select != 3);
 }
 return 0;
}

#endif

#endif  /* _QUEUE1_C_ */

