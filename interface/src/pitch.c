/*
* Pitch conversion functions.
*
* Copyright 2010-2018 Pete Jefferson <pete.jefferson@gmail.com>
*
*/

#include <stdbool.h>

#include "dn-interface.h"

/*
* Normal pitch range on the DN2000F mk II is +/-8%, with pitch bend range +/-12.7%.
* Value 0x80 means the pitch control is disabled.
* Values between 0.1% and 12.7% are 0x01 through to 0x7f.
* Values between -12.7% and -0.1% are 0x81 through to 0xff.
*
* These two tables provide fast lookup converting between the packet byte and percentage.
*
*/

const float PITCH_TABLE_PERCENT[256] = {
	/* 0x00 >= 0x7f */
	0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f,
	1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f,
	2.0f, 2.1f, 2.2f, 2.3f, 2.4f, 2.5f, 2.6f, 2.7f, 2.8f, 2.9f,
	3.0f, 3.1f, 3.2f, 3.3f, 3.4f, 3.5f, 3.6f, 3.7f, 3.8f, 3.9f,
	4.0f, 4.1f, 4.2f, 4.3f, 4.4f, 4.5f, 4.6f, 4.7f, 4.8f, 4.9f,
	5.0f, 5.1f, 5.2f, 5.3f, 5.4f, 5.5f, 5.6f, 5.7f, 5.8f, 5.9f,
	6.0f, 6.1f, 6.2f, 6.3f, 6.4f, 6.5f, 6.6f, 6.7f, 6.8f, 6.9f,
	7.0f, 7.1f, 7.2f, 7.3f, 7.4f, 7.5f, 7.6f, 7.7f, 7.8f, 7.9f,
	8.0f, 8.1f, 8.2f, 8.3f, 8.4f, 8.5f, 8.6f, 8.7f, 8.8f, 8.9f,
	9.0f, 9.1f, 9.2f, 9.3f, 9.4f, 9.5f, 9.6f, 9.7f, 9.8f, 9.9f,
	10.0f, 10.1f, 10.2f, 10.3f, 10.4f, 10.5f, 10.6f, 10.7f, 10.8f, 10.9f,
	11.0f, 11.1f, 11.2f, 11.3f, 11.4f, 11.5f, 11.6f, 11.7f, 11.8f, 11.9f,
	12.0f, 12.1f, 12.2f, 12.3f, 12.4f, 12.5f, 12.6f, 12.7f,

	/* 0x80 is pitch off */
	0.0f,

	/* 0x81 >= 0xff */
	-12.7f, -12.6f, -12.5f, -12.4f, -12.3f, -12.2f, -12.1f,
	-12.0f, -11.9f, -11.8f, -11.7f, -11.6f, -11.5f, -11.4f, -11.3f, -11.2f, -11.1f,
	-11.0f, -10.9f, -10.8f, -10.7f, -10.6f, -10.5f, -10.4f, -10.3f, -10.2f, -10.1f,
	-10.0f, -9.9f, -9.8f, -9.7f, -9.6f, -9.5f, -9.4f, -9.3f, -9.2f, -9.1f,
	-9.0f, -8.9f, -8.8f, -8.7f, -8.6f, -8.5f, -8.4f, -8.3f, -8.2f, -8.1f,
	-8.0f, -7.9f, -7.8f, -7.7f, -7.6f, -7.5f, -7.4f, -7.3f, -7.2f, -7.1f,
	-7.0f, -6.9f, -6.8f, -6.7f, -6.6f, -6.5f, -6.4f, -6.3f, -6.2f, -6.1f,
	-6.0f, -5.9f, -5.8f, -5.7f, -5.6f, -5.5f, -5.4f, -5.3f, -5.2f, -5.1f,
	-5.0f, -4.9f, -4.8f, -4.7f, -4.6f, -4.5f, -4.4f, -4.3f, -4.2f, -4.1f,
	-4.0f, -3.9f, -3.8f, -3.7f, -3.6f, -3.5f, -3.4f, -3.3f, -3.2f, -3.1f,
	-3.0f, -2.9f, -2.8f, -2.7f, -2.6f, -2.5f, -2.4f, -2.3f, -2.2f, -2.1f,
	-2.0f, -1.9f, -1.8f, -1.7f, -1.6f, -1.5f, -1.4f, -1.3f, -1.2f, -1.1f,
	-1.0f, -0.9f, -0.8f, -0.7f, -0.6f, -0.5f, -0.4f, -0.3f, -0.2f, -0.1f
};

const byte PITCH_TABLE_BYTES[256] = {
	/* 0.0% >= 12.7% */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
	0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13,
	0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
	0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
	0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31,
	0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
	0x3c, 0x3d, 0x3e, 0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45,
	0x46, 0x47, 0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60, 0x61, 0x62, 0x63,
	0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d,
	0x6e, 0x6f, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
	0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,

	/* Pitch off */
	0x80,

	/* -0.1% <= -12.7% */
	0xff, 0xfe, 0xfd, 0xfc, 0xfb, 0xfa, 0xf9, 0xf8, 0xf7, 0xf6,
	0xf5, 0xf4, 0xf3, 0xf2, 0xf1, 0xf0, 0xef, 0xee, 0xed, 0xec,
	0xeb, 0xea, 0xe9, 0xe8, 0xe7, 0xe6, 0xe5, 0xe4, 0xe3, 0xe2,
	0xe1, 0xe0, 0xdf, 0xde, 0xdd, 0xdc, 0xdb, 0xda, 0xd9, 0xd8,
	0xd7, 0xd6, 0xd5, 0xd4, 0xd3, 0xd2, 0xd1, 0xd0, 0xcf, 0xce,
	0xcd, 0xcc, 0xcb, 0xca, 0xc9, 0xc8, 0xc7, 0xc6, 0xc5, 0xc4,
	0xc3, 0xc2, 0xc1, 0xc0, 0xbf, 0xbe, 0xbd, 0xbc, 0xbb, 0xba,
	0xb9, 0xb8, 0xb7, 0xb6, 0xb5, 0xb4, 0xb3, 0xb2, 0xb1, 0xb0,
	0xaf, 0xae, 0xad, 0xac, 0xab, 0xaa, 0xa9, 0xa8, 0xa7, 0xa6,
	0xa5, 0xa4, 0xa3, 0xa2, 0xa1, 0xa0, 0x9f, 0x9e, 0x9d, 0x9c,
	0x9b, 0x9a, 0x99, 0x98, 0x97, 0x96, 0x95, 0x94, 0x93, 0x92,
	0x91, 0x90, 0x8f, 0x8e, 0x8d, 0x8c, 0x8b, 0x8a, 0x89, 0x88,
	0x87, 0x86, 0x85, 0x84, 0x83, 0x82, 0x81
};

/* Convert the packet byte to pecentage */
float PitchByteToPercent(byte Pitch)
{
	float f = PITCH_TABLE_PERCENT[Pitch];
	return f;
}

/* Convert the packet byte to pecentage */
byte PitchPercentToByte(float Pitch)
{
	int n = (int)(Pitch * 10); /* Shift the decimal point the right */

	/* Positive pitch ranges start at offset 0, negative pitch ranges start at offset 128 */
	if (Pitch < 0)
		n = 128 + (n * -1);

	return PITCH_TABLE_BYTES[n];
}
