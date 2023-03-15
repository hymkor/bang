#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hayos.h"
#include "sprite.h"
#include "enemy.h"
#include "joy.h"
#include "map.h"
#include "game.h"
#include "bang.h"
#include "file.h"
#include "pattern.h"
#include "music.h"

Stage story[16];
Map opening_map;
int nstage=0;

stickfunc_t stdstick = keystick;

#define DEM " \t\n\r"

int configuration( void )
{
	File file("BANG.CFG","r");
	if( !file ){
		printf( "�R���t�B�M�����[�V�����t�@�C��:BANG.CFG�����[�h�ł��܂���.\n" ); 
		return 1;
	}
	
	int map_cell_load_flag=0 , opening_map_load_flag = 0;
	for(;;){
		char buffer[256],*token;
		do{
			if( file.gets( buffer , sizeof( buffer ) )==NULL ){
				if( !feof(file) ){
					printf( "�R���t�B�M�����[�V�����t�@�C��:BANG.CFG�����[�h�ł��܂���.\n" ); 
					return 1;
				}
				if( map_cell_load_flag == 0 ){
					printf( "�}�b�v�Z�������[�h����Ă��܂���.\n" );
					return 1;
				}
				if( opening_map_load_flag == 0 ){
					printf( "�I�[�v�j���O�w�i�����[�h����Ă��܂���.\n" );
					return 1;
				}
				return 0;
			}
			
			token = strtok( buffer , DEM );
		}while(token == NULL  ||  token[0]=='\0'  ||  token[0]=='#' );
		
		int ch=token[0];

		token = strtok( NULL , DEM );
		
		switch(ch){
			
		/* �V�i���I�t�@�C���̎w�� */
		case 'S':case 's':
			if( token == NULL ){
				printf( "�V�i���I�t�@�C����������܂���.(%d)\n"
						"BANG.CFG : S �t�@�C���� �X�e�[�W��<CR>\n" , file.Lnum() );
				break;
			}
			int err=story[nstage].load( token );
			if( err== 0 ){
				printf( "%2d�� : \"%s\"\n" , nstage , token );
				if( (token=strtok(NULL," \n\t")) != NULL ){
					strcpy( story[nstage].stage_name , token );
				}else{
					sprintf( story[nstage].stage_name , "Stage-%d" , nstage+1 );
				}
				++nstage;
			}else{
				printf( "�V�i���I : \"%s\"�ɂ�����G���[(%d�s��)\n\t%s\n",token,file.Lnum(),Stage::errmsg[err] );
			}
			break;
		
		/* �}�b�v�Z���̎w�� */
		case 'C':case 'c':
			if( token == NULL ){
				printf( "�}�b�v�Z���t�@�C����������܂���.(%d)\n"
						"BANG.CFG : C �t�@�C���� <CR>\n" , file.Lnum());
				break;
			}
			if( map_cell_load_flag==0 ){
				if( cell_load(token)==0 ){
					printf( "�}�b�v�Z�� : \"%s\"\n" , token );
					map_cell_load_flag = 1;
				}else{
					printf( "�}�b�v�Z�� : \"%s\"��,���[�h�ł��܂���.(%d�s��)\n",token,file.Lnum());
					return 1;
				}
			}else{
				printf( "�Q�[���S�̂�ʂ��Ďg�p�ł���}�b�v�Z���͈�����ł�.\n"
						"�}�b�v�Z��\"%s\"��,�������܂�.\n" );
			}
			break;
		
		/* �I�[�v�j���O��ʔw�i�̎w�� */
		case 'O':case 'o':
			if( token == NULL ){
				printf( "�I�[�v�j���O�p�}�b�v�t�@�C����������܂���.(%d)\n"
						"BANG.CFG : O �t�@�C���� <CR>\n" , file.Lnum() );
				break;
			}
			if( opening_map.load(token) ){
				printf( "�I�[�v�j���O�w�i�t�@�C�� : \"%s\"������܂���.(%d)\n" , token , file.Lnum() );
				return 1;
			}
			opening_map_load_flag = 1;
			break;
		
		/* �W���C�X�e�B�b�N�̎w�� */
		case 'J':case 'j':
			if( token == NULL ){
				printf( "�W���C�X�e�B�b�N�^�C�v���L�q����Ă��܂���.(%d)\n"
						"BANG.CFG : J  SEGA|GAMEPORT<CR>\n" , file.Lnum() );
				break;
			}
			if( strcmp(token,"sega")==0  ||  strcmp(token,"SEGA")==0 ){
				sega_joy_init();
				banghoki = stdstick = sega_joystick0;
			}else if( strcmp(token,"gameport")==0  ||  strcmp(token,"GAMEPORT")==0  ){
				banghoki = stdstick = joystick0;
			}
			break;
			
		/* �T�E���h(�r�[�v��) */
		case 'B':case 'b':
			printf( "�T�E���h�n�m\n" );
			BangHoki::sound_on();
			Enemy::sound_on();
			break;
		
		case 'H':
			printf( "�Q�[���̓�x�� HARD �ɂ��܂�.\n"
					"�z�[�L�́A�g���K�𗣂��Ă��A���ė��܂���.\n" );
			hard_flag = 1;
			break;
		default:
			printf( "�����ȃR�}���h��������܂�.(%d)\n",file.Lnum());
			break;
		}
	}/* end of for(;;) */
}

void symbol__shadow_center(int y,int shift,const char *string,int xtimes,int ytimes,int forcol,int bakcol)
{
	size_t len=strlen(string);
	int x = (320-(len * xtimes * 8))/2;
	symbol(x+shift,y+shift,string,xtimes,ytimes,bakcol);
	symbol(x      ,y      ,string,xtimes,ytimes,forcol);
}
static int opening(void)
{
	int i;
	jmp_buf jb;
	
	for(int x=0; x < MAP_WIDTH ; x++ ){
		for(int y=0; y < MAP_HEIGHT ; y++ ){
			   cell_putat( x<<3 , y<<3 , opening_map[ x ][ y ] );
			bg_cell_putat( x<<3 , y<<3 , opening_map[ x ][ y ] );
		}
	}
	
	/* �u�΂���v�ƕ\�� */
	Sprite::draw( 32 , 50 , logo_pattern[0] );
	Sprite::draw( 64 , 50 , logo_pattern[1] );
	Sprite xtsu;
	xtsu.putat( 96 , 50 , logo_pattern[3] );
	
	sleep(2);
	for(i=0 ; i < 20 ; i++){
		/* �u���v���E�ɏ��X�Ɉړ� */
		xtsu.putat( 97+i , 50 );
		vsync_sleep();
		if( ShiftScan() & BothShift ){
			return 0;
		}else if( (*stdstick)() ){
			return 1;
		}
	}
	
	Sprite::draw( 96 , 50 , logo_pattern[2] );
	Sprite::draw( 148 , 50 , exclamation );
	
	sleep(1);
	
	Sprite ZO,BAR,KI,N2;
	
	for(i=200-32 ; i > 50 ; i-- ){
		ZO. putat( 160 , i-1 , logo_pattern[4] );
		BAR.putat( 192 , i-1 , logo_pattern[5] );
		vsync_sleep();
		
		KI.putat( 224 , i-1 , logo_pattern[6] );
		N2.putat( 256 , i-1 , logo_pattern[7] );
		
		vsync_sleep();
		if( ShiftScan() & BothShift ){
			return 0;
		}else if( (*stdstick)() ){
			return 1;
		}
	}
	
	symbol__shadow_center(35,1,"SPACE KAMIKAZE",2,1,14,6);
	
	symbol__shadow_center(152,1,"ANNIVERSARY",1,1,12,4);
//	symbol__shadow_center(105,1,"Insert Coins !",2,1,9,1);

	symbol__shadow_center(170,1,"Program & Map   K.HAYAMA" ,1,1,10,2);
	symbol__shadow_center(180,1,"  Charactor     T.OYA   " ,1,1,10,2);

	Sprite hoki;
	for(i=0 ; ; i++ ){
		const static unsigned char ledpat[]={
			ScrollLockLed ,
			CapsLockLed ,
			NumLockLed ,
			CapsLockLed ,
		};
		
		LED( ledpat[ (i>>3) % numof(ledpat)  ] );
		hoki.putat( 200 , 80 , hoki_pattern[ (i&15)>>2 ] );
		
		vsync_sleep();
		vsync_sleep();
		
		if( ShiftScan() & BothShift ){
			return 0;
		}else if( (*stdstick)() ){
			return 1;
		}
	}
}

const char *SpritePatternLoad( void );

static int game(void)
{
	while( opening() ){
		play( "L16O3CDFA" );
		while( (*stdstick)() )
			;
		int stage=0;
		banghoki.set_left(3);
		for(;;){
			box_fill(0,0,320,200,0);
			symbol__shadow_center(90,1, story[stage].stage_name ,1,1,12,4);
			sleep(1);
		
			World::init(&story[stage]);
			
			banghoki.begin(15,100);
			int scrollwait=0;
			
			for(;;){
				if( ++scrollwait == 6 ){
					scrollwait = 0;
					
					switch(World::scroll() ){
					case World::SCROLL_CONT :
						Enemy::scroll( -1 , 0 );
						break;
					
					case World::STAGE_CLEAR :
						goto stage_clear;
					}
				}
				
				vsync_sleep();
				beep_off();
				banghoki.runs();
				
				if( banghoki.is_over()  ||  ( ShiftScan() & BothShift)  ){
					beep_off();
					BangHoki::sound_stop();
					symbol__shadow_center(80,1,"GAME OVER",2,2,15,8);
					play( "O3L16F8FFF8G+GGFFEF8R4" );
					
					goto gameover;
				}
				Enemy::run();
				
				while( Kbhit() ) (void) Getch();
				
			}/**** 1�ʓ��ł̃��[�v ****/
		
		stage_clear:
			beep_off();
			BangHoki::sound_stop();
			
		if( ++stage >= nstage ) break;
			
			symbol__shadow_center(80,1,"STAGE   CLEAR",2,2,15,8);
			symbol__shadow_center(72,1,&("1\0" "2\0" "3\0" "4\0" "5\0" "6\0" "7\0" "8\0" "9")[(stage<<1)-2],3,3,12,4);
			banghoki.end();
			Enemy::kill_all();
			play( "L16O3CDEFGABO4C4" );
		
		}/*** �ʖ��̃��[�v ****/

		beep_off();
		BangHoki::sound_stop();
		symbol__shadow_center(80,1,"Congraturations !",2,3,14,6);
		play( "L16O3CDEFGABO4C4" );
		symbol__shadow_center(150,1,"Your mission is completed.",1,1,12,4);
		sleep(9);
		
	gameover:
		banghoki.end();
		Enemy::kill_all();
	}
	return 0;
}


int main(int argc,char **argv)
{
//	atsleep( keyscan );
	
	if( configuration() )
		return 1;
	
	if( background_init() ){
		printf( "�w�i�̕ۑ��̂��߂̃q�[�v�̈悪�m�ۂł��܂���.\n" );
		return 1;
	}
	const char *inloadable_file = SpritePatternLoad();
	
	if( inloadable_file != NULL ){
		printf( "�p�^�[���t�@�C��\"%s\"���ǂ߂܂���.\n" , inloadable_file );
		return 1;
	}
	if( argc >= 2  &&  ( strcmp(argv[1],"sega")==0  ||  strcmp(argv[1],"SEGA")==0 )  ){
		sega_joy_init();
		banghoki = stdstick = sega_joystick0;
	}
	int old_video_mode=get_video_mode();
	set_video_mode(0x13);
	  game();
	set_video_mode( old_video_mode );
	return 0;
}
