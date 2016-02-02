


extern void vider_sd_hd_switch(u8 mode);
extern int rt_key_ctl_init(void);

extern u8 iris_set_ok;
extern u8 osd_mid_str_buff[10];
extern u16 key_detect(void);
extern u8 cam_filter_set_ok;
extern u16 key_from_wait;


extern u8 menu_normal_flag;

extern u8 cursor_hor_pos;
extern u8 cursor_ver_pos;
extern u8 battery_power;
extern u8 video_sd_hd_mode;
extern u8 filter_mode_value;


#ifndef		KEY_STATE_TYPE_MACRO
#define		KEY_STATE_TYPE_MACRO
enum KEY_STATE_TYPE{
KEY_STATE_NONE,
	KEY_STATE_RECORD,
	KEY_STATE_PAUSE,
	KEY_STATE_PREV,
	KEY_STATE_NEXT,
	KEY_STATE_PLAYER,

};
#endif

