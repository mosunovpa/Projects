/**
 * \file crc32.h
 *
 * \since 2006/02/20
 * \author pavel
 */

#ifndef crc32_h__
#define crc32_h__

class Crc32
{
public:
	static int crc32_get_crc (unsigned char* buffer, long len);

private:
	Crc32() {};
	virtual ~Crc32() {};
	static void crc32_init_table();
	static unsigned long crc32_reflect (unsigned ref, char ch);

	static unsigned int crc32_table[256];	
	static char initialized;


};

#endif // crc32_h__