#ifndef PALETTE_H
#define PALETTE_H

class Palette {
	unsigned char code;

public:
	unsigned char red, blue, green;

	 Palette( void ) {  }
	 Palette( int c ) : code(c) {  }
	~Palette( void ) {  }

	void read(int code = -1);
	void write(int code = -1);
};

#endif
