#ifndef HAYOS_H
#define HAYOS_H




/*********** ���ʃ}�N�� **************/

#include <stddef.h>

typedef unsigned char (far *gram_t)[320];
#define numof(A) ( sizeof(A) / sizeof((A)[0]) )
#define GRAM	 ((gram_t)0xA0000000L)
#define FpOff(P) (* ( unsigned short *)&(P)   )  
#define FpSeg(P) (*(( unsigned short *)&(P)+1))


/********** module "VIDEO" ***********/

	extern unsigned short vram_offset;
	void		set_video_mode(unsigned mode);
	unsigned	get_video_mode( void );
	void 		set_crtc_register(unsigned short address,unsigned short data);
	int  		get_crtc_register(unsigned short address);
	void		set_start_address(unsigned short address);
	void		set_horizontal_planning( unsigned short npixel );
	void		add_vram_offset_13h( signed int delta  );

	class VgaSave {
		char *buffer;
		int flag,video_mode;
		size_t size;
	public:
		 VgaSave(int flag=7);
		~VgaSave( void );
		operator const void* (void) const { return buffer==0 ? 0 : this; }
		int operator! (void) const { return buffer==0 ? 1 : 0; }
	};


/******** module "GRAPH" **********/

	void     box_fill( int x , int y , int width , int height , int color );
	void xor_box     ( int x , int y , int width , int height , int color );
	void xor_box_fill( int x , int y , int width , int height , int color );


/******** module "SYMBOL" *********/

	void symbol(int x,int y,const char *string,int xtimes=1,int ytimes=1,int color=15);


/********** module "WRITES" *********/

	void writes(int cx,int cy,const char *string,int color=15,int page=0);
#if 0
	size_t line_input(int cx,int cy,char *buffer,size_t size);
	size_t line_input(int cx,int cy,const char *prompt,char *buffer,size_t size);
#endif
	int input(int x,int y,const char *prompt,char *buffer,size_t size);
	int input(int x,int y,                   char *buffer,size_t size);

	/* input()�ł�,buffer�ɂ̓f�t�H���g�̕����񂪓����Ă��Ȃ���΁C�����Ȃ��D
		�A��l == -1 �ŃG�X�P�[�v���͂ɂ�钆�f�D�O�ȏ�ŕ�����̒����D
	*/


/*********** module "KEYBOARD" ****************/

	enum{
		RightShift	=0x1,		LeftShift	=0x2,
		BothShift	=0x3,		CtrlShift	=0x4,
		AltShift	=0x8,		ScrollLock	=0x10,
		NumLock		=0x20,		CapsLock	=0x40,
		InsertLock	=0x80,
	};
	
	unsigned Inputch();
	unsigned Kbhit();
	unsigned ShiftScan();
	inline int Getch(){ return Inputch()&0xff; }


/********** module "SCNKEY" *************/

	extern volatile scantable[128];
#if __cplusplus
	extern "C"
#endif
	void keyscan_init( void );

/********** module "BEEP" ************/

	void beep_on(unsigned n=400);
	void beep_off();
	void beep(int sw);

	/*	beep_on , beep_off ��,���������Ȃ�Ƃ��ł������I�Ƀr�[�vON/OFF���s��.
		beep(1) , beep(0) ��,beep(1)�����s����������,beep(0)�����s���Ȃ��Ɖ��͏����Ȃ�.	*/


/********** module "SLEEP" ***********/

	unsigned long get_interval_timer( void );

	void	vsync_sleep( void );					/* 1/60   �b */
	void	irq0_sleep(  unsigned int useconds  );	/* 1/18.2 �b */
	void	sleep(  unsigned int seconds  );		/*   �P   �b */
	int		atsleep(  void (*func)(void)  ,  int  flag = 0);
	/* �t���O��	���Ȃ��,sleep�̊J�n����1�x����,
				��Ȃ��,sleep�̊�,�����,
				���Ȃ��,sleep�̏I������1�x����,�Ăяo�����.	*/


/********** module "LED" ************/

#ifdef __cplusplus
	extern "C"
#endif
	void LED(unsigned int pat);
	enum{
		ScrollLockLed	= 1,
		NumLockLed		= 2,
		CapsLockLed		= 4,
	};

#endif
