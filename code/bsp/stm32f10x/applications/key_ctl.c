#include <board.h>
#include <rtthread.h>

#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif  /* RT_USING_COMPONENTS_INIT */

#include "includes.h"
#include "osd_menu.h"
#include "key_ctl.h"


#define	KEY_PORT1		GPIOA
#define	KEY_PORT2		GPIOB
#define	KEY_PORT3		GPIOC
#define	KEY_PORT4		GPIOD

enum key_type
{
	KEY_NONE,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,

	KEY_ESC,
	KEY_CALL,
	
	KEY_PRESET,
	
	KEY_IRIS,//14

	KEY_MODE,//15
	KEY_FILTER,//
	
	KEY_F1,
	KEY_SETUP,//18
	
	KEY_F2,
	KEY_CAM,
	
	KEY_NO_CHANGE,

};



u8 cam_para_mode=0;


u32 int_nu = 0;
u8 flag =0;

u8 ec11_power;      //火力0-100
s32 ec11_power_m=0;    //旋转编码器增量
u16 ec11_time=0;      //时间，分最大180分（3小时）
u16 ec11_time_m=100;          //旋转编码器增量




void EXTI9_5_int(u8 mode)
{
	NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;

  if(mode)
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
else
	
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;

  NVIC_Init(&NVIC_InitStructure);

}

//u16 pp7l=0,pp7h=0;


#if 1
void ec11_key_interrupt(void)
{}


#else
void ec11_key_interrupt(void)
{  
//   u8 ss_m;
//按键中断**********************************************************
	static rt_tick_t ec11cnt = 0;
		rt_tick_t ec11cnt_cru=0;

		
	static rt_uint8_t pulse_state_bak = 0;
	
	
	if((EXTI_GetITStatus(EXTI_Line7) != RESET))
	{

		if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0))   //第一次中断，并且A相是下降沿
		{

			pp7l++;
		}
		else if((GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7))) 
		{
	

			pp7h++;
		}
		

	/* Clear the  EXTI line 8 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line7);
	}

}
#endif



void exti9_5_int_enable(FunctionalState cmd)
{
	NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  NVIC_InitStructure.NVIC_IRQChannelCmd = cmd;

  NVIC_Init(&NVIC_InitStructure);

}

/**
  * @brief  Configure PB.09 or PG.08 in interrupt mode
  * @param  None
  * @retval None
  */
void EXTI9_5_Config(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure PB.09 pin as input floating */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Enable AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  /* Connect EXTI9 Line to PB.09 pin */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);

  /* Configure EXTI9 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
 
 EXTI_InitStructure.EXTI_Line = EXTI_Line8;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI9_5 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//DISABLE;//ENABLE;

  NVIC_Init(&NVIC_InitStructure);

}



//旋钮 1
void key_2_pin_init(void)
{

	GPIO_InitTypeDef GPIOD_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIOD_InitStructure);	

	EXTI9_5_Config();
}

u8 key_pb78_state = 0;


u8 key2_check_dir(void)
{
	
	if(key_pb78_state==1)
		return 1;// cw
	else if(key_pb78_state==2)
		return 2;//ccw
	else
		return 0;//no
}


//0,press; 1,no press
u8 key2_press_check(void)
{
	if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == 0)
	{
		rt_thread_delay(20);

			if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2) == 0)
				return 0;
	}

	return 1;
}


//0,press; 1,no press
u8 key_sw22_check(void)
{
	static u8 key_sw22_pre=0;
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
	{
		rt_thread_delay(10);

			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
			{
				key_sw22_pre = 1;
				return key_sw22_pre;

			}
	}
	else
	{
		if(key_sw22_pre == 1)
		{
			key_sw22_pre = 0x10;
			return key_sw22_pre;
		}
		else
			{
			key_sw22_pre = 0;

		}
	}
	

	
	return 0;
}

#define ADC_CHN_M 4 //为2个通道 0,1
#define ADC_CHN_N (ADC_CHN_M*16) //每通道采16次

static vu16 Photoreg_ADC1_ConvertedValue[ADC_CHN_N];
static volatile u8 ADC_Ok=0;

static void Photoreg_ADC2_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_0|GPIO_Pin_2|GPIO_Pin_3; //ADC1-light
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

}

static DMA_InitTypeDef DMA_InitStructure;

//DMA的配置
static void Photoreg_DMA_Configuration(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/* open the DMA1 intterrupt service */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;//ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	/* 允许 DMA1 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMA通道1*/
	DMA_DeInit(DMA1_Channel1);
	//指定DMA外设基地址
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)( &(ADC1->DR));		//ADC1数据寄存器
	//设定DMA内存基地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Photoreg_ADC1_ConvertedValue;					//获取ADC的数组
	//外设作为数据传输的来源
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					//片内外设作源头
	//指定DMA通道的DMA缓存大小
	DMA_InitStructure.DMA_BufferSize = ADC_CHN_N;								//每次DMA16个数据
	//外设地址不递增（不变）
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址不增加
	//内存地址不递增（不变）
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//内存地址增加
	//设定外设数据宽度为16位
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//半字
	//设定DMA的工作模式普通模式，还有一种是循环模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;								//普通模式
	//设定DMA通道的软件优先级
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//高优先级
	//使能DMA内存到内存的传输，此处没有内存到内存的传输
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//非内存到内存
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);								//DMA通道1传输完成中断
	
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

static void Photoreg_ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* 允许ADC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	/* ADC1 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;							//单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//连续扫描
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	//软件启动转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 4; 								//1个通道
	ADC_Init(ADC1, &ADC_InitStructure);

	/* 配置通道1的采样速度,*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_239Cycles5);
	/* 配置通道0的采样速度,*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 2, ADC_SampleTime_239Cycles5);
	/* 配置通道0的采样速度,*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 3, ADC_SampleTime_239Cycles5);
	/* 配置通道0的采样速度,*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 4, ADC_SampleTime_239Cycles5);


	/* 允许ADC1的DMA模式 */
	ADC_DMACmd(ADC1, ENABLE);

	/* 允许ADC1*/
	ADC_Cmd(ADC1, ENABLE);

	/*重置校准寄存器 */   
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	/*开始校准状态*/
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	   
	/* 人工打开ADC转换.*/ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);


}

static void DMAReConfig(void)
{
	DMA_DeInit(DMA1_Channel1);
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}



static void joystick_ADC_Init(void)
{

	Photoreg_ADC2_GPIO_INIT();
	Photoreg_DMA_Configuration();
	Photoreg_ADC_Configuration();

   
}

#define	ADC_SAMPLE_NUM	16	//ADC值抽样数	
#define	ADC_MIDDLE_START_NUM	(ADC_SAMPLE_NUM/5)
#define	ADC_MIDDLE_END_NUM		(ADC_SAMPLE_NUM - ADC_MIDDLE_START_NUM)
#define	ADC_SAMPLE_VALID_SIZE	(ADC_SAMPLE_NUM - ADC_MIDDLE_START_NUM * 2)	

/**************************************************************
** 函数名:DigitFilter
** 功能:软件滤波
** 注意事项:取NO 的2/5 作为头尾忽略值,注意N 要大于5,否则不会去头尾
***************************************************************/

static u16 DigitFilter(u16* buf,u8 no)
{
	u8 i,j;
	u32 tmp;
	u16 Pravite_ADC_buf[16];
	
	for(i=0;i<no;i++)
	{
		Pravite_ADC_buf[i] = buf[i];
	 	buf[i] = 0;
	}	
	//排序，将buf[0]到buf[no-1]从大到小排列
	for(i=0;i<no;i++)
	{
		for(j=0;j<no-i-1;j++)
		{
			if(Pravite_ADC_buf[j]>Pravite_ADC_buf[j+1])
			{
				tmp=Pravite_ADC_buf[j];
				Pravite_ADC_buf[j]=Pravite_ADC_buf[j+1];
				Pravite_ADC_buf[j+1]=tmp;
			}
		}
	}


	//平均
	tmp=0;
	//for(i=cut_no;i<no-cut_no;i++) //只取中间n-2*cut_no 个求平均
	for(i=ADC_MIDDLE_START_NUM;i<ADC_MIDDLE_END_NUM;i++) //只取中间n-2*cut_no 个求平均
		tmp+=Pravite_ADC_buf[i];
	return(tmp/ADC_SAMPLE_VALID_SIZE);
}


#define	RECORDER_KEY_STATE_RECORD_PIN	GPIO_Pin_8
#define	RECORDER_KEY_STATE_PLAYER_PIN	GPIO_Pin_7
#define	RECORDER_KEY_STATE_NEXT_PIN	GPIO_Pin_6
#define	RECORDER_KEY_STATE_PREV_PIN	GPIO_Pin_9
#define	RECORDER_KEY_STATE_PAUSE_PIN	GPIO_Pin_8


void recorder_key_state_pin_init(void)
{

	GPIO_InitTypeDef GPIOD_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIOD_InitStructure.GPIO_Pin = KEY_PLAYER_PIN|KEY_PAUSE_PIN|KEY_NEXT_PIN|KEY_PREV_PIN|KEY_RECORD_PIN|KEY_ON_PIN;
	GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOD_InitStructure); 

	GPIOD_InitStructure.GPIO_Pin = RECORDER_KEY_STATE_RECORD_PIN;
	GPIO_Init(GPIOA, &GPIOD_InitStructure); 


}




#define	KEY_PLAYER_PIN	GPIO_Pin_13
#define	KEY_PAUSE_PIN	GPIO_Pin_2
#define	KEY_NEXT_PIN	GPIO_Pin_11
#define	KEY_PREV_PIN	GPIO_Pin_10
#define	KEY_RECORD_PIN	GPIO_Pin_12

#define	KEY_ON_PIN		GPIO_Pin_1

void player_stop_pin_init(void)
{

	GPIO_InitTypeDef GPIOD_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIOD_InitStructure.GPIO_Pin = KEY_PLAYER_PIN|KEY_PAUSE_PIN|KEY_NEXT_PIN|KEY_PREV_PIN|KEY_RECORD_PIN|KEY_ON_PIN;
	GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOD_InitStructure); 


	GPIO_WriteBit(GPIOB,KEY_PLAYER_PIN,Bit_RESET);
	GPIO_WriteBit(GPIOB,KEY_PAUSE_PIN,Bit_RESET);
	GPIO_WriteBit(GPIOB,KEY_NEXT_PIN,Bit_RESET);
	GPIO_WriteBit(GPIOB,KEY_PREV_PIN,Bit_RESET);
	GPIO_WriteBit(GPIOB,KEY_RECORD_PIN,Bit_RESET);
	
	GPIO_WriteBit(GPIOB,KEY_ON_PIN,Bit_RESET);
	rt_thread_delay(RT_TICK_PER_SECOND/20);
	GPIO_WriteBit(GPIOB,KEY_ON_PIN,Bit_SET);
	rt_thread_delay(RT_TICK_PER_SECOND/2);	
	GPIO_WriteBit(GPIOB,KEY_ON_PIN,Bit_RESET);

}


u16 player_stop_key_buf[]={KEY_RECORD_PIN,KEY_PAUSE_PIN,KEY_PREV_PIN,KEY_NEXT_PIN,KEY_PLAYER_PIN};//up,down,left,right,center

void player_stop_set(u8 mode)
{

	if(mode<5)
		{
		GPIO_WriteBit(GPIOB,player_stop_key_buf[mode],Bit_SET);
		rt_thread_delay(RT_TICK_PER_SECOND/2);	
		GPIO_WriteBit(GPIOB,player_stop_key_buf[mode],Bit_RESET);

	}

}


void key_pin_init(void)
{

	GPIO_InitTypeDef GPIOD_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(KEY_PORT1, &GPIOD_InitStructure);	

	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(KEY_PORT2, &GPIOD_InitStructure);	

	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(KEY_PORT3, &GPIOD_InitStructure);	
	
	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(KEY_PORT4, &GPIOD_InitStructure);	

	key_2_pin_init();


	recorder_key_state_pin_init();

	player_stop_pin_init();
}

u16 key_pre = 0;


u32 key_merge(void)
{
	u32 data = 0,data2 = 0,data3 = 0,data4 = 0;

	u32 key_tmp;
	
	//PA,PB,PC
	data = GPIO_ReadInputDataBit(KEY_PORT1);
	data2 = GPIO_ReadInputData(KEY_PORT2);	
	data3 = GPIO_ReadInputData(KEY_PORT3);	
	data4 = GPIO_ReadInputData(KEY_PORT4);	
		
	key_tmp = (data2>>15)&0x0001;//0
	key_tmp |= (data3>>5)&0x0006;//1-2
	key_tmp |= (data2>>11)&0x0008;// 3
	key_tmp |= (data2>>9)&0x0010;// 4
	key_tmp |= (data2>>7)&0x0020;// 5
	key_tmp |= (data2>>4)&0x0040;// 6

	key_tmp |= (data2<<5)&0x0080;//7
	key_tmp |= (data2<<7)&0x0100;//8
	key_tmp |= (data<<3)&0x0200;//9

	key_tmp |= (data3<<2)&0x00000400;//10
	key_tmp |= (data3<<2)&0x00000800;//11

	key_tmp |= (data<<4)&0x1000;//12
	key_tmp |= (data<<2)&0x2000;//13
	key_tmp |= (data<<2)&0x4000;//14

	key_tmp |= (data2<<4)&0x8000;//15
	key_tmp |= (data2<<16)&0x10000;//16

	key_tmp |= (data<<13)&0x20000;//17
	key_tmp |= (data<<13)&0x40000;//18
	key_tmp |= (data<<12)&0x80000;//19

	return key_tmp;
}

//返回0为无按键，返回非0值，则为对应的按键号
static u32 key_ctl_check(void)
{
	u16 i;
	u32 key_tmp;
	static u32 long_press_cnt = 0;// 50ms
	
	key_tmp = key_merge();
	for(i=0;i<20;i++)
	{
		if(((key_tmp>>i)&0x0001)==0)
		{
			rt_thread_delay(20);

			key_tmp = key_merge();

			if(((key_tmp>>i)&0x0001)==0)
			{
				if(key_pre == i+1)
				{
					if(long_press_cnt>40)
					{

						long_press_cnt=0;
						key_pre = 0;
						return ((i+1)|0x9000);
					}

					long_press_cnt++;
				}
				key_pre = i+1;
				//return (i+1);
				break;
			}
		}
	}


	if((key_pre && key_pre!=(i+1))||(key_pre && i==20))
	{
		i = key_pre|0x8000;
		key_pre = 0;
		return i;

	}
	return 0;
}


extern rt_sem_t	uart1_sem;


u8 iris_auto_manual_state = 0;// 默认0 为自动模式
void iris_auto_manual_switch(void)
{


}


void iris_auto_manual_set(u8 mode)
{

}


u8 key_val_buffer_cnt = 0;
u16 key_val_buffer_func = 0;

u8 osd_mid_buff[30]={0};
u8 osd_mid_str_buff[10]={0};

void num_to_string(u16 data,u8 *dst)
{

	dst[0] = data/1000+0x30;
	dst[1] = data/100%10+0x30;
	dst[2] = data%100/10+0x30;
	dst[3] = data%10+0x30;
}

u16 key_num_val = 0;

u8 osd_mid_buff_pointer=0;

#define	OSD_MSG_DISP_MAX_SECOND		40

#define	key_to_long(val)	(val|0x9000)
#define	key_to_release(val)	(val|0x8000)

u8 iris_mode=0;
u8 iris_mode_setup=0;

u8 cam_filter_mode = 1;


void key_value_all_clear(void)
{
	memset(osd_mid_buff,0,sizeof(osd_mid_buff));
	memset(osd_mid_str_buff,0,sizeof(osd_mid_str_buff));

	key_num_val = 0;
	key_val_buffer_cnt = 0;
	osd_mid_buff_pointer=0;
	key_val_buffer_func = 0;
	osd_line1_val_disp_clear();
}


 enum key_type key_function_state = KEY_NONE;


extern uchar keyboard_data_buffer[20];
//return 0,failed; 1,success
u8 wait_device_reply(u8* srcdata,u8 len,u32 w_100ms)
{
	u32 cnt;
	cnt = w_100ms;
	while(cnt--)
	{
		if (command_analysis()) 
		{
			u8 kk;

			for(kk=2;kk<len-1;kk++)
			{
				if(srcdata[kk] != keyboard_data_buffer[kk])
					return 0;


			}
			return 1;
		}

		rt_thread_delay (100);
	}
}


u8 cmd_buff[7];


u8 baudrate_to_num(u16 br)
{
	switch(br)
	{
	case 1200:

		return 0;
	case 2400:

		return 1;
	case 4800:

		return 2;
	case 9600:

		return 3;
	default:break;

	}
	return 1;
}


u16 num_to_baudrate(u8 numb)
{
	switch(numb)
	{
	case 0:

		return 1200;
	case 1:

		return 2400;
	case 2:

		return 4800;
	case 3:

		return 9600;
	default:break;

	}
	return 2400;
}


void key_analyze(u16 val)
{

	switch(val)
	{
	case key_to_release(KEY_CALL):

		if(key_val_buffer_cnt)
		{pelcod_call_pre_packet_send((u8)key_num_val);

		osd_opt_message_disp(0,OSD_MSG_DISP_MAX_SECOND);
		}
		key_value_all_clear();

		
		break;
	case key_to_release(KEY_PRESET):
		if(key_val_buffer_cnt)
		{pelcod_set_pre_packet_send((u8)key_num_val);
		osd_opt_message_disp(1,OSD_MSG_DISP_MAX_SECOND);}
		key_value_all_clear();

		
		break;

	case key_to_release(KEY_CAM):

		if((key_val_buffer_cnt>0)&&(key_val_buffer_cnt<4))
		{if(key_num_val < 256)
		{
			domeNo = key_num_val;

			//osd_opt_message_disp(0,OSD_MSG_DISP_MAX_SECOND);

			osd_line_little_4_disp(1);

			key_value_all_clear();
		}
		}
		else if(key_val_buffer_cnt==0)
		{
			if(key_val_buffer_func == 0)
			{key_val_buffer_func = key_to_release(KEY_CAM);
			strcat(osd_mid_str_buff," ID+");
			osd_line1_disp(32);
			}

		}
		break;
	case key_to_release(KEY_IRIS):
		if(key_num_val==0)
			key_value_all_clear();
		if(key_num_val > 0 && key_num_val < 5)
		{
			
			{
				iris_mode = key_num_val-1;
				if(iris_mode>3)
					iris_mode=0;

				if(iris_mode==0)
					pelcod_set_pre_packet_send(128);
				else if(iris_mode==1)
					pelcod_call_pre_packet_send(128);
				else if(iris_mode==2)
					pelcod_call_pre_packet_send(127);
				else if(iris_mode==3)
					pelcod_call_pre_packet_send(126);
				osd_line2_disp(1);
				osd_opt_message_disp(16+iris_mode,OSD_MSG_DISP_MAX_SECOND);
			key_value_all_clear();
			}

			
		}
		break;

	case key_to_release(KEY_MODE):

		if(key_num_val==0)
			key_value_all_clear();
		if(key_num_val > 0 && key_num_val < 9)
		{	
			pelcod_call_pre_packet_send((u8)key_num_val+200);
			cam_para_mode = key_num_val-1;
			osd_line2_disp(key_num_val);
			osd_opt_message_disp(3,OSD_MSG_DISP_MAX_SECOND);
			key_value_all_clear();

		}
		else if(key_val_buffer_cnt==0)
		{
			if(key_val_buffer_func == 0)
			{key_val_buffer_func = key_to_release(KEY_MODE);
			strcat(osd_mid_str_buff,"Mode+");
			osd_line1_disp(32);
			}

		}
		//osd_opt_message_disp(0,OSD_MSG_DISP_MAX_SECOND);
		break;
	case key_to_release(KEY_SETUP):
			if(key_num_val > 0 && key_num_val < 9)
			{
				if(key_val_buffer_func == key_to_release(KEY_MODE))
				{
					if(key_num_val <= 4)
					{
					pelcod_set_pre_packet_send(255);
					pelcod_set_pre_packet_send(253);
				pelcod_set_pre_packet_send((u8)key_num_val+200);
					pelcod_set_pre_packet_send(253);
					}
					else
					{
					pelcod_set_pre_packet_send(255);	
					pelcod_set_pre_packet_send((u8)key_num_val+200);
					}
				osd_line3_disp(1);
				osd_opt_message_disp(2,OSD_MSG_DISP_MAX_SECOND);
				
				if(wait_device_reply(cmd_buff,7,OSD_MSG_DISP_MAX_SECOND))
				{
					;//osd_opt_message_disp(2,OSD_MSG_DISP_MAX_SECOND);

				}
				else
					{
					
				}
				}

			}

			if(key_val_buffer_func == key_to_release(KEY_IRIS))
			{
				if(iris_mode==0)
					pelcod_set_pre_packet_send(128);
				else if(iris_mode==1)
					pelcod_call_pre_packet_send(128);
				else if(iris_mode==2)
					pelcod_call_pre_packet_send(127);
				else if(iris_mode==3)
					pelcod_call_pre_packet_send(126);
				osd_line2_disp(1);
				iris_set_ok = 1;
				//osd_opt_message_disp(4,OSD_MSG_DISP_MAX_SECOND);
			}

			if(key_val_buffer_func == key_to_release(KEY_FILTER))
			{
				pelcod_call_pre_packet_send(cam_filter_mode+80);
				osd_line2_disp(1);
				cam_filter_set_ok = 1;
			}

			if(key_val_buffer_func == key_to_release(KEY_CAM))
			{
				if(key_num_val<256)
				{
					u8 dometmp;

					dometmp = domeNo;
					domeNo = 255;
					pelcod_set_pre_packet_send(255);
					pelcod_set_pre_packet_send(255);
					pelcod_set_pre_packet_send(key_num_val);
					pelcod_set_pre_packet_send(255);
					osd_opt_message_disp(10,OSD_MSG_DISP_MAX_SECOND);

					domeNo = key_num_val;
					osd_line_little_4_disp(0);
				}
				osd_line2_disp(1);
				cam_filter_set_ok = 1;
			}

			
			key_value_all_clear();
			break;
			
	case key_to_release(KEY_ESC):
		key_value_all_clear();
		break;

	case key_to_release(KEY_FILTER):
		if(key_num_val==0)
			key_value_all_clear();
		if(key_num_val > 0 && key_num_val < 5)
		{
			cam_filter_mode = key_num_val-1;
			pelcod_call_pre_packet_send(cam_filter_mode+80);
				osd_line2_disp(1);
				cam_filter_set_ok = 1;

				osd_opt_message_disp(11+cam_filter_mode,OSD_MSG_DISP_MAX_SECOND);
			key_value_all_clear();
		}
		break;
	case key_to_long(KEY_SETUP):
		switch(key_num_val )
		{
		case 1200:
		case 2400:
		case 9600:
		case 4800:
			Baud_rate = baudrate_to_num(key_num_val);
			set_rs485_uart_baudrate();
			osd_line_little_4_disp(0);
			break;
		default:break;
		}

		key_value_all_clear();
		break;

	case key_to_long(KEY_IRIS):
		iris_mode_setup = 1;	
		iris_mode = 0x80;
		osd_line2_disp(1);

		break;
		
	default:
		break;
	}


}


u8 menu_normal_flag = 0;

void key_handle(u16 val)
{
	u16 tmp;

	if(menu_normal_flag)
	{
		

	}
	else
	{
		
		player_stop_set(val);

	}
	
}


u16 key_detect(void)
{
	u32 tmp;

	tmp = key_ctl_check();
	if((tmp>=key_to_release(KEY_1)) && (tmp<=key_to_release(KEY_0)))
		tmp = tmp&(~0x9000);
	return(tmp);

}

u16 key_from_wait = 0;


const u8* wifi_enter_at_mode="+++AtCmd\\r\\n";



void rt_key_thread_entry(void* parameter)
{

	u16 k;

	key_pin_init();
	joystick_ADC_Init();
	
	rt_thread_delay(200);
	
    while(1)
	{
				
		if(key_from_wait)
		{
			key_handle(key_from_wait);
			key_from_wait = 0;
		}
		
		k = key_detect();
		if(k)
		{
			key_handle(k);

		}

		
		rt_thread_delay(20);
    }
}


u8 iris_set_ok=1;
u8 cam_filter_set_ok = 1;

void rt_blink_thread_entry(void* parameter)
{
	static  u8 state_pre,cam_filter_pre;
	
	while(1)
	{
		cam_filter_pre = cam_filter_set_ok;
		state_pre = iris_set_ok;

		if(!iris_set_ok)
		{
			osd_line2_disp(1);
			rt_thread_delay(600);
			osd_line_2_disp_item_clear(2);
		}

		if(!state_pre && iris_set_ok)
			osd_line2_disp(1);
		
		if(!cam_filter_set_ok)
		{
			osd_line2_disp(1);
			rt_thread_delay(600);
			osd_line_2_disp_item_clear(3);
		}

		if(!cam_filter_pre && cam_filter_set_ok)
			osd_line2_disp(1);
		rt_thread_delay(600);
	}

}



int rt_key_ctl_init(void)
{

	
    rt_thread_t init_thread;


    init_thread = rt_thread_create("key",
                                   rt_key_thread_entry, RT_NULL,
                                   1024, 10, 5);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);


	
    return 0;
}

