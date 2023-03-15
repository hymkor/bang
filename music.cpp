#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "music.h"

unsigned long get_interval_timer();

static void irq_wait( unsigned long dncount )
{
	unsigned long timer = get_interval_timer() + dncount ;
	
	while( get_interval_timer() < timer )
		;
}

int play( const char *MMLSTR )
{
	const static onpu_t  onpu_table[3][ 'G'-'A'+1 ]={
		{ A , B    ,  C   , D ,  E   ,  F   , G },
		{ As,NoOnpu,  Cs  , Ds,NoOnpu,  Fs  , Gs},	/* # :シャープ */
		{ Af, Bf   ,NoOnpu, Df,  Ef  ,NoOnpu, Gf},	/* b :フラット */
	};
	unsigned	O=4 ,	/* オクターブ */
				L=4 ,	/* 音符 */
				T=100 ;	/* テンポ(一分間に四分音符をいくつならすか) */
	const char *MML = MMLSTR;
	
	for(;;){
		/* wait量 = 1093 [回/分] ÷ <テンポ>[四分音符数/分] ÷ <音符番号> × 4 [全音符の長さ/四分音符の長さ] 
		 *
		 *        = 4*1093 / Tf
		 */
		
		const unsigned  long numer = 4*1093;
		
		switch( toupper(*MML) ){
		case '\0':
			return 0;
		
		case 'O':
			O=(unsigned) strtoul( MML+1 , (char**)&MML , 10 );
			break;
	
		case 'L':
			L=(unsigned) strtoul( MML+1 , (char**)&MML , 10 );
			break;
		
		case 'R':{
			int length;
			if( isdigit(MML[1]) ){
				length = strtoul( MML+1 , (char**)&MML , 10 );
			}else{
				length = L;
			}
			irq_wait( numer / (T*length) );
			break;
		}
		default:
			if( 'A' <= *MML  &&  *MML <= 'G' ){
				int n;
				onpu_t   onpu;
				unsigned length;
				
				n = *MML++ -'A';
				if( *MML=='+'  ||  *MML=='#' ){
					onpu = onpu_table[1][n];
					MML++;
				}else if( *MML=='-' ){
					onpu = onpu_table[2][n];
					MML++;
				}else{
					onpu = onpu_table[0][n];
				}
				
				if( onpu == NoOnpu )
					return MMLSTR - MML;
				
				if( isdigit(*MML) ){
					length = (unsigned) strtoul( MML , (char**)&MML , 10 );
				}else{
					length = L;
				}
				
				piano( O , onpu );
				/*  printf("T=%d L=%d wait=%ld " , T , length , numer / (T*length) );  */
				irq_wait( numer / (T*length) );
				beep_off();
			}
			break;
		}/* switch */
	}/* for(;;) */
}/* music */

#if 0
int main(int argc,char **argv)
{
	int i;
	for( i=1 ; i<argc ; i++){
		play( argv[i] );
	}
}

#endif
