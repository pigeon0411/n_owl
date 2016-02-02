#include <board.h>
#include <rtthread.h>

#ifdef  RT_USING_COMPONENTS_INIT
#include <components.h>
#endif  /* RT_USING_COMPONENTS_INIT */

#include "includes.h"
#include "osd_menu.h"
#include "key_ctl.h"




#define	KEY_PLAYER_PIN	GPIO_Pin_13
#define	KEY_PAUSE_PIN	GPIO_Pin_2
#define	KEY_NEXT_PIN	GPIO_Pin_11
#define	KEY_PREV_PIN	GPIO_Pin_10
#define	KEY_RECORD_PIN	GPIO_Pin_12

#define	KEY_ON_PIN		GPIO_Pin_1



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


void rt_state_key_thread_entry(void* parameter);



#define	RECORDER_KEY_STATE_RECORD_PIN	GPIO_Pin_8
#define	RECORDER_KEY_STATE_PLAYER_PIN	GPIO_Pin_7
#define	RECORDER_KEY_STATE_NEXT_PIN	GPIO_Pin_6
#define	RECORDER_KEY_STATE_PREV_PIN	GPIO_Pin_9
#define	RECORDER_KEY_STATE_PAUSE_PIN	GPIO_Pin_8


#if 1
void recorder_key_state_pin_init(void)
{

	GPIO_InitTypeDef GPIOD_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIOD_InitStructure.GPIO_Pin = RECORDER_KEY_STATE_PLAYER_PIN|RECORDER_KEY_STATE_NEXT_PIN|RECORDER_KEY_STATE_PREV_PIN|RECORDER_KEY_STATE_PAUSE_PIN;
	GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIOD_InitStructure); 

	GPIOD_InitStructure.GPIO_Pin = RECORDER_KEY_STATE_RECORD_PIN;
	GPIO_Init(GPIOA, &GPIOD_InitStructure); 

	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIOD_InitStructure); 


}
#else

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

#endif

enum KEY_STATE_TYPE recorder_work_state;

u8 recorder_key_state_check(void)
{
	u16 tmp;
	
	tmp = (GPIO_ReadInputData(GPIOC)>>6)&0x000f;


	u8 i;

	for(i=0;i<4;i++)
	{
		if(( (tmp>>i)&0x01) == 0)
		{
			recorder_work_state = i+1;
			break;
		}


	}
	

	if(i==4)
	{

		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0)
		{
			i = 0xff;
			recorder_work_state = KEY_STATE_RECORD;
		}
		else
			recorder_work_state = KEY_STATE_NONE;
		
	}

	
	
}


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
	//rt_thread_delay(RT_TICK_PER_SECOND*2);	
	//GPIO_WriteBit(GPIOB,KEY_ON_PIN,Bit_RESET);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIOD_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIOD_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIOD_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOD_InitStructure); 
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);

	
}


void vider_sd_hd_switch(u8 mode)
{
	mode = mode;
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_RESET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_RESET);

	rt_thread_delay(RT_TICK_PER_SECOND/2);	

	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_5,Bit_SET);
	GPIO_WriteBit(GPIOA,GPIO_Pin_6,Bit_SET);


}


u16 player_stop_key_buf[]={KEY_RECORD_PIN,KEY_PAUSE_PIN,KEY_PREV_PIN,KEY_NEXT_PIN,KEY_PLAYER_PIN};//up,down,left,right,center

enum KEY_INPUT_TYPE{
KEY_INPUT_NONE,
KEY_PREV,//KEY_RECORD, //up
KEY_NEXT,//KEY_PAUSE, //down
KEY_RECORD, //left
KEY_PLAYER,//KEY_PAUSE, //right
KEY_PAUSE,//set
};

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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
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


	recorder_key_state_pin_init();

	player_stop_pin_init();
}



u16 key_pre = 0;

#define	KEY_UP_PIN	GPIO_Pin_2
#define	KEY_DOWN_PIN	GPIO_Pin_15
#define	KEY_LEFT_PIN	GPIO_Pin_12
#define	KEY_RIGHT_PIN	GPIO_Pin_3
#define	KEY_SET_PIN	GPIO_Pin_4


u32 key_merge(void)
{
	u32 data = 0,data2 = 0;

	
	//PA,PB,PC
	data2 = GPIO_ReadInputDataBit(GPIOD,KEY_UP_PIN);

	data = data2;
	data2 = GPIO_ReadInputDataBit(GPIOA,KEY_DOWN_PIN);	

	data |= data2<<1;
	
	data2 = GPIO_ReadInputDataBit(GPIOC,KEY_LEFT_PIN);	
	data |= data2<<2;
	data2 = GPIO_ReadInputDataBit(GPIOB,KEY_RIGHT_PIN);	
	data |= data2<<3;
	data2 = GPIO_ReadInputDataBit(GPIOB,KEY_SET_PIN);	
	data |= data2<<4;
		


	return data;
}


#define	KEY_INPUT_NUM_MAX		5

//返回0为无按键，返回非0值，则为对应的按键号
static u32 key_ctl_check(void)
{
	u16 i;
	u32 key_tmp;
	static u32 long_press_cnt = 0;// 50ms
	
	key_tmp = key_merge();
	for(i=0;i<KEY_INPUT_NUM_MAX;i++)
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


	if((key_pre && key_pre!=(i+1))||(key_pre && i==KEY_INPUT_NUM_MAX))
	{
		i = key_pre|0x8000;
		key_pre = 0;
		return i;

	}
	return 0;
}


extern rt_sem_t	uart1_sem;




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

u8 cursor_hor_pos =0;
u8 cursor_ver_pos = 0;

void exit_menu(void)
{
	cursor_hor_pos =0;
	cursor_ver_pos = 0;	
	menu_normal_flag = 0;

	osd_line_1to4_all_disp();
}

void key_analyze(u16 val)
{

		
		switch(val)
			{
			case key_to_long(KEY_RECORD):
				filter_mode_value = 2;
				
				//osd_line3_disp(0);
				pelcod_call_pre_packet_send(filter_mode_value+201);

				osd_line_1to4_all_disp();
				break;
			case key_to_long(KEY_PAUSE):
				
				if(video_sd_hd_mode)
					video_sd_hd_mode = 0;
				else
					video_sd_hd_mode = 1;

				//osd_sd_hd_mode_disp(video_sd_hd_mode,OSD_LINE2_X_16_POS+88,OSD_LINE2_Y_16_POS);
				vider_sd_hd_switch(0);
				osd_line_1to4_all_disp();
				break;
		
			case key_to_long(KEY_PLAYER):
		
				filter_mode_value = 3;
					
					//osd_line3_disp(0);

					pelcod_call_pre_packet_send(filter_mode_value+201);
					osd_line_1to4_all_disp();

				
				break;
			case key_to_long(KEY_NEXT):

				filter_mode_value = 1;
				
				//osd_line3_disp(0);
				pelcod_call_pre_packet_send(filter_mode_value+201);

				osd_line_1to4_all_disp();
				break;
		
			case key_to_long(KEY_PREV):
		
					filter_mode_value = 0;
					
					//filter_mode_disp(0,OSD_LINE3_Y_POS,filter_mode_value);
					//osd_line3_disp(0);
					pelcod_call_pre_packet_send(filter_mode_value+201);
					osd_line_1to4_all_disp();

				break;
					
		
			case key_to_release(KEY_PLAYER):
				//exit_menu();

				
				break;
				
			default:

				if(val > 0x8000)
					{

					player_stop_set((u8)val&0xff);
						osd_line_1to4_all_disp();
				}
				break;
			}



	


}


u8 menu_normal_flag = 0;

void key_handle(u16 val)
{
	u16 tmp;


	key_analyze(val);

	


	
}


u16 key_detect(void)
{
	u32 tmp;

	tmp = key_ctl_check();

	return(tmp);

}

u16 key_from_wait = 0;



void rt_key_thread_entry(void* parameter)
{
    rt_thread_t init_thread;

	u16 k;
	u16 cnttmp = 0;
	
	key_pin_init();
	
	rt_thread_delay(200);

    init_thread = rt_thread_create("state",
                                   rt_state_key_thread_entry, RT_NULL,
                                   512, 10, 5);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);
	
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


		//osd_battery_mode_disp(battery_power,OSD_LINE2_X_16_POS,OSD_LINE2_Y_16_POS);
		//osd_line4_disp(1);
		
		rt_thread_delay(20);
    }
}



void rt_blink_thread_entry(void* parameter)
{
	static  u8 state_pre;
	
	while(1)
	{
		if(menu_normal_flag)
		{
			if(cursor_ver_pos == 0)
			{
				osd_sd_hd_mode_disp(video_sd_hd_mode,OSD_LINE2_X_16_POS+88,OSD_LINE2_Y_16_POS);
				rt_thread_delay(600);
				osd_sd_hd_mode_disp_clear(video_sd_hd_mode,OSD_LINE2_X_16_POS+88,OSD_LINE2_Y_16_POS);
			}
			else if(cursor_ver_pos == 1)
			{
				osd_line3_disp(1);
				rt_thread_delay(600);
				
				filter_mode_disp_clear(0,OSD_LINE3_Y_POS,filter_mode_value);
			}

		}
		rt_thread_delay(600);
	}

}

void rt_state_key_thread_entry(void* parameter)
{
	while(1)
	{

		recorder_key_state_check();

		rt_thread_delay(100);

	}


}

int rt_key_ctl_init(void)
{

	
    rt_thread_t init_thread;


    init_thread = rt_thread_create("key",
                                   rt_key_thread_entry, RT_NULL,
                                   512, 10, 5);
    if (init_thread != RT_NULL)
        rt_thread_startup(init_thread);




	
    return 0;
}

