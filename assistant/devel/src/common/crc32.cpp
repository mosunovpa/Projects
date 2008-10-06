/**
 * \file crc32.cpp
 *
 * \since 2006/02/20
 * \author pavel
 */

#include "stdafx.h"
#include "crc32.h"

unsigned int Crc32::crc32_table[256];
char Crc32::initialized = 0;

unsigned long Crc32::crc32_reflect (unsigned ref, char ch)
{
	unsigned long value(0);

	for (int i = 1; i < (ch + 1); i++) {
		if(ref & 1) value |= 1 << (ch - i);
		ref >>= 1;
	}
	return value;
}

void Crc32::crc32_init_table()
{
	unsigned long ulPolynomial = 0x04c11db7;

	for (int i = 0; i <= 0xFF; i++) {
		crc32_table[i] = crc32_reflect (i, 8) << 24;
		for (int j = 0; j < 8; j++)
			crc32_table[i] = (crc32_table[i] << 1) ^ (crc32_table[i] & (1 << 31) ? ulPolynomial : 0);
		crc32_table[i] = crc32_reflect (crc32_table[i], 32);
	}
	initialized = 1;
}

int Crc32::crc32_get_crc (unsigned char* buffer, long len)
{
	if (!initialized) 
	{
		crc32_init_table();
	}
	unsigned long  crc(0xffffffff);
	while(len--) crc = (crc >> 8) ^ crc32_table[(crc & 0xFF) ^ *buffer++];
	return crc^0xffffffff;
}
