#ifndef MUSIC_H
#define MUSIC_H

enum onpu {
	C , Cs , D , Ds , E , F , Fs , G , Gs , A , As , B , num_of_onpu ,
		Df=Cs ,  Ef=Ds ,      Gf=Fs ,  Af=Gs,      Bf=As,NoOnpu = -1 ,
};
typedef enum onpu onpu_t;

extern const unsigned short  beep_table[ num_of_onpu ];

#define piano(m,n)    beep_on( beep_table[n]>>m )

void beep_on(unsigned dncount);
void beep_off( void );

int play( const char *MML );

#endif /* MUSIC_H */
