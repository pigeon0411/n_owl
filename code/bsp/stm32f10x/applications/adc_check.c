#include <board.h>
#include <rtthread.h>

#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif  /* RT_USING_COMPONENTS_INIT */

#include "includes.h"




static vu16 Photoreg_ADC1_ConvertedValue[16];
static volatile u8 ADC_Ok=RT_FALSE;

static void Photoreg_ADC2_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //ADC1-light
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

static DMA_InitTypeDef DMA_InitStructure;

//DMA������
static void Photoreg_DMA_Configuration(void)
{
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
	
	/* open the DMA1 intterrupt service */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;//ENABLE;
	NVIC_Init(&NVIC_InitStructure); 

	/* ���� DMA1 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	/* DMAͨ��1*/
	DMA_DeInit(DMA1_Channel1);
	//ָ��DMA�������ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)( &(ADC1->DR));		//ADC1���ݼĴ���
	//�趨DMA�ڴ����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)Photoreg_ADC1_ConvertedValue;					//��ȡADC������
	//������Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;					//Ƭ��������Դͷ
	//ָ��DMAͨ����DMA�����С
	DMA_InitStructure.DMA_BufferSize = 16;								//ÿ��DMA16������
	//�����ַ�����������䣩
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ������
	//�ڴ��ַ�����������䣩
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;				//�ڴ��ַ����
	//�趨�������ݿ��Ϊ16λ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//����
	//�趨DMA�Ĺ���ģʽ��ͨģʽ������һ����ѭ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;								//��ͨģʽ
	//�趨DMAͨ����������ȼ�
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;							//�����ȼ�
	//ʹ��DMA�ڴ浽�ڴ�Ĵ��䣬�˴�û���ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;								//���ڴ浽�ڴ�
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	//DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);								//DMAͨ��1��������ж�
	
	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

static void Photoreg_ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	/* ����ADC */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	/* ADC1 */
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;							//��ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//����ɨ��
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 	//�������ת��
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1; 								//1��ͨ��
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ����ͨ��1�Ĳ����ٶ�,*/ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);


	/* ����ADC1��DMAģʽ */
	ADC_DMACmd(ADC1, ENABLE);

	/* ����ADC1*/
	ADC_Cmd(ADC1, ENABLE);

	/*����У׼�Ĵ��� */   
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));

	/*��ʼУ׼״̬*/
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	   
	/* �˹���ADCת��.*/ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);


}

static void DMAReConfig(void)
{
	DMA_DeInit(DMA1_Channel1);
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}



static void ADC1_Init(void)
{

	Photoreg_ADC2_GPIO_INIT();
	Photoreg_DMA_Configuration();
	Photoreg_ADC_Configuration();

   
}

extern void delayms(int n);


//static u16 ADC_2_Filter(void)
//{
//	u32 result=0;
//	u8 i;
//	for(i=16;i>0;i--)
//	{
//	       delayms(1);
//	       result += (u32)Photoreg_ADC1_ConvertedValue;
//	}
//	return (u16)(result/16);
//}


/*��������ֵ��������*/
#define	ADC_SAMPLE_NUM	16	//ADCֵ������	
#define	ADC_MIDDLE_START_NUM	(ADC_SAMPLE_NUM/5)
#define	ADC_MIDDLE_END_NUM		(ADC_SAMPLE_NUM - ADC_MIDDLE_START_NUM)
#define	ADC_SAMPLE_VALID_SIZE	(ADC_SAMPLE_NUM - ADC_MIDDLE_START_NUM * 2)	




/**************************************************************
** ������:DigitFilter
** ����:����˲�
** ע������:ȡNO ��2/5 ��Ϊͷβ����ֵ,ע��N Ҫ����5,���򲻻�ȥͷβ
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
	//���򣬽�buf[0]��buf[no-1]�Ӵ�С����
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


	//ƽ��
	tmp=0;
	//for(i=cut_no;i<no-cut_no;i++) //ֻȡ�м�n-2*cut_no ����ƽ��
	for(i=ADC_MIDDLE_START_NUM;i<ADC_MIDDLE_END_NUM;i++) //ֻȡ�м�n-2*cut_no ����ƽ��
		tmp+=Pravite_ADC_buf[i];
	return(tmp/ADC_SAMPLE_VALID_SIZE);
}

/*
 * Function:       
 * Description:    ��ȡ��������ĵ�ѹֵ������������ǰ��������ǿ��
 * Calls:          
 * Called By:      
 * Table Accessed: 
 * Table Updated:  
 * Input:          
 * Output:         
 * Return:         ���ص�ѹֵ����Ϊmv
 * Others:         
 */
 
static rt_uint32_t  Get_agc_Value(void)
{
	rt_uint32_t i;
	i = DigitFilter((u16 *)Photoreg_ADC1_ConvertedValue,ADC_SAMPLE_NUM)*3300/0xFFF;
DMAReConfig();
	return i;
}



static void rt_hw_adc_init(void)
{
ADC1_Init();
	
	rt_kprintf("ACD init.....OK\n\r");
}

void adc_io_set(u16 val)
{
	switch(val)
	{
	case 1:
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);
		break;
	case 2:
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		break;
	case 3:
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		break;
	case 4:
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		break;
	case 5:
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
		break;
	default:break;
	
	}

}

//adc>1470 pc0-h, <1470 pc0-l
// >1570 pc1 h,< pc1 l
// >1670 pc2 h,< pc2 l
// >1770 pc3 h,< pc3 l

#define	PC3_LED_VAL		1770
static void adc_check(void)
{
	rt_uint32_t adc_this;
  rt_kprintf("hello here is a ADC test\n\r");

	adc_this = Get_agc_Value();

	if(adc_this >= PC3_LED_VAL)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);
	}
	else if(adc_this < PC3_LED_VAL)
		GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);
	
	if(adc_this >= 1670)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);
	}
	else 
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);
	}
	
	if(adc_this >= 1570)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);
	}
	else 
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);
	}
	
	if(adc_this >= 1470)
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);
	}
	else 
	{
		GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);
	}
	
}


static void adc_io_pin_init(void)
{
	GPIO_InitTypeDef GPIOD_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_Init(GPIOC, &GPIOD_InitStructure);	
    
	GPIO_SetBits(GPIOC,GPIO_Pin_0);	
	GPIO_SetBits(GPIOC,GPIO_Pin_1);		
	GPIO_SetBits(GPIOC,GPIO_Pin_2);	
	GPIO_SetBits(GPIOC,GPIO_Pin_3);	
 

}

void rt_adc_thread_entry(void* parameter)
{
    rt_thread_t init_thread;
	u16 k;

	rt_hw_adc_init();
	adc_io_pin_init();
	
    while(1)
	{
		adc_check();
		rt_thread_delay(RT_TICK_PER_SECOND/2);
    }
}




int rt_adc_ctl_init(void)
{

//	
//    rt_thread_t init_thread;

//    init_thread = rt_thread_create("adc",
//                                   rt_adc_thread_entry, RT_NULL,
//                                   4089, 10, 30);
//    if (init_thread != RT_NULL)
//        rt_thread_startup(init_thread);

    return 0;
}


