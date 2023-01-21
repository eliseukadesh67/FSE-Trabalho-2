/*
 * crc16.h
 *
 *  Created on: 18/03/2014
 *      Author: Renato Coral Sampaio
 */

#ifndef CRC16_H_
#define CRC16_H_

#include <stdio.h>
#include <stdlib.h>

short CRC16(short crc, char data);
short calcula_CRC(unsigned char *commands, int size);
int validate_CRC(u_char *recieved_data, size_t numbytes, short recieved_crc);

#endif /* CRC16_H_ */