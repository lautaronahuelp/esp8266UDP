#ifndef SIA_FUNCTIONS_H
#define SIA_FUNCTIONS_H

char * genDC09String(int type, int *evento, int * sequence);// // GENERATE DC09 STRING

bool checkDC09CRC(char * message);// CHECK DC09 CRC

char * genCRCStamp(char * entradaString);// CALC DC09 CRC

unsigned int calcCRC(unsigned CRC, int ch);// CALC DC09 CRC

#endif
