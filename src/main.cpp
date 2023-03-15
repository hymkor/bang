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
		printf( "コンフィギュレーションファイル:BANG.CFGがロードできません.\n" ); 
		return 1;
	}
	
	int map_cell_load_flag=0 , opening_map_load_flag = 0;
	for(;;){
		char buffer[256],*token;
		do{
			if( file.gets( buffer , sizeof( buffer ) )==NULL ){
				if( !feof(file) ){
					printf( "コンフィギュレーションファイル:BANG.CFGがロードできません.\n" ); 
					return 1;
				}
				if( map_cell_load_flag == 0 ){
					printf( "マップセルがロードされていません.\n" );
					return 1;
				}
				if( opening_map_load_flag == 0 ){
					printf( "オープニング背景がロードされていません.\n" );
					return 1;
				}
				return 0;
			}
			
			token = strtok( buffer , DEM );
		}while(token == NULL  ||  token[0]=='\0'  ||  token[0]=='#' );
		
		int ch=token[0];

		token = strtok( NULL , DEM );
		
		switch(ch){
			
		/* シナリオファイルの指定 */
		case 'S':case 's':
			if( token == NULL ){
				printf( "シナリオファイル名がありません.(%d)\n"
						"BANG.CFG : S ファイル名 ステージ名<CR>\n" , file.Lnum() );
				break;
			}
			int err=story[nstage].load( token );
			if( err== 0 ){
				printf( "%2d面 : \"%s\"\n" , nstage , token );
				if( (token=strtok(NULL," \n\t")) != NULL ){
					strcpy( story[nstage].stage_name , token );
				}else{
					sprintf( story[nstage].stage_name , "Stage-%d" , nstage+1 );
				}
				++nstage;
			}else{
				printf( "シナリオ : \"%s\"におけるエラー(%d行目)\n\t%s\n",token,file.Lnum(),Stage::errmsg[err] );
			}
			break;
		
		/* マップセルの指定 */
		case 'C':case 'c':
			if( token == NULL ){
				printf( "マップセルファイル名がありません.(%d)\n"
						"BANG.CFG : C ファイル名 <CR>\n" , file.Lnum());
				break;
			}
			if( map_cell_load_flag==0 ){
				if( cell_load(token)==0 ){
					printf( "マップセル : \"%s\"\n" , token );
					map_cell_load_flag = 1;
				}else{
					printf( "マップセル : \"%s\"が,ロードできません.(%d行目)\n",token,file.Lnum());
					return 1;
				}
			}else{
				printf( "ゲーム全体を通じて使用できるマップセルは一つだけです.\n"
						"マップセル\"%s\"は,無視します.\n" );
			}
			break;
		
		/* オープニング画面背景の指定 */
		case 'O':case 'o':
			if( token == NULL ){
				printf( "オープニング用マップファイル名がありません.(%d)\n"
						"BANG.CFG : O ファイル名 <CR>\n" , file.Lnum() );
				break;
			}
			if( opening_map.load(token) ){
				printf( "オープニング背景ファイル : \"%s\"がありません.(%d)\n" , token , file.Lnum() );
				return 1;
			}
			opening_map_load_flag = 1;
			break;
		
		/* ジョイスティックの指定 */
		case 'J':case 'j':
			if( token == NULL ){
				printf( "ジョイスティックタイプが記述されていません.(%d)\n"
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
			
		/* サウンド(ビープ音) */
		case 'B':case 'b':
			printf( "サウンドＯＮ\n" );
			BangHoki::sound_on();
			Enemy::sound_on();
			break;
		
		case 'H':
			printf( "ゲームの難度を HARD にします.\n"
					"ホーキは、トリガを離しても帰って来ません.\n" );
			hard_flag = 1;
			break;
		default:
			printf( "無効なコマンド名があります.(%d)\n",file.Lnum());
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
	
	/* 「ばんっ」と表示 */
	Sprite::draw( 32 , 50 , logo_pattern[0] );
	Sprite::draw( 64 , 50 , logo_pattern[1] );
	Sprite xtsu;
	xtsu.putat( 96 , 50 , logo_pattern[3] );
	
	sleep(2);
	for(i=0 ; i < 20 ; i++){
		/* 「っ」を右に徐々に移動 */
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
				
			}/**** 1面内でのループ ****/
		
		stage_clear:
			beep_off();
			BangHoki::sound_stop();
			
		if( ++stage >= nstage ) break;
			
			symbol__shadow_center(80,1,"STAGE   CLEAR",2,2,15,8);
			symbol__shadow_center(72,1,&("1\0" "2\0" "3\0" "4\0" "5\0" "6\0" "7\0" "8\0" "9")[(stage<<1)-2],3,3,12,4);
			banghoki.end();
			Enemy::kill_all();
			play( "L16O3CDEFGABO4C4" );
		
		}/*** 面毎のループ ****/

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
		printf( "背景の保存のためのヒープ領域が確保できません.\n" );
		return 1;
	}
	const char *inloadable_file = SpritePatternLoad();
	
	if( inloadable_file != NULL ){
		printf( "パターンファイル\"%s\"が読めません.\n" , inloadable_file );
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