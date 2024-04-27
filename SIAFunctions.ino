#include "SIAFunctions.h"

char * genDC09String(int type, int *evento, int sequence, char * account){
  static char salida[48];
  char salidaSinStamp[40];

  //if(*sequence > 9999) *sequence = 0;
  
  switch(type){
    case 0:
        snprintf(salidaSinStamp, 40, "\"NULL\"%04dR0L0#%s[]", sequence, account);
      break;
    case 1:
        snprintf(salidaSinStamp, 40, "\"ADM-CID\"%04dR0L0#%X%X%X%X[#%X%X%X%X|%X%X%X%X%X%X%X%X%X]", sequence, evento[0], evento[1], evento[2], evento[3],
        evento[0], evento[1], evento[2], evento[3], evento[6], evento[7], evento[8], evento[9], evento[10], evento[11], evento[12], evento[13], evento[14]);
      break;
    case 2:
        snprintf(salidaSinStamp, 40, "\"ADM-42E\"%04dR0L0#%X%X%X%X[#%X%X%X%X|%X%X]", sequence, evento[0], evento[1], evento[2], evento[3],
        evento[0], evento[1], evento[2], evento[3], evento[6], evento[7]);
      break;
  }
  snprintf(salida, 48, "%s%s", genCRCStamp(salidaSinStamp), salidaSinStamp);
  
  return salida;
}

bool checkDC09CRC(char * message) {
  char entrada[21], salida[50], token[9];
  strcpy(salida, message);
  strcpy(entrada, strtok(&salida[8], "\r"));
  strcpy(token, strtok(salida, "\""));

  if (strcmp(token, genCRCStamp(entrada)) == 0) {
    return true;
  }
  return false;
}

char * genCRCStamp(char * entradaString)
{
 static char stamp[9];
 unsigned int CRC; /* 16 BIT CRC RESULT */
 int count, ch;
 
 CRC = 0;
 count = 0;

 while (ch = *entradaString++)
 {
  CRC = calcCRC(CRC, ch); /* CALL CRC FUNCTION BELOW */
  ++count;

 } 
 
  sprintf(stamp,"%04X%04X", CRC, count);

  return stamp;
}

unsigned int calcCRC(unsigned CRC, int ch)
{
 int i;
 unsigned char temp;
 temp = (unsigned char)ch; /* TREAT LOCALLY AS UNSIGNED */
 for (i = 0; i < 8; i++) /* DO 8 BITS */
 {
 temp ^= CRC & 1; /* PROCESS LSB */
 CRC >>= 1; /* SHIFT RIGHT */
 if (temp & 1)
 CRC ^= 0xA001; /* IF LSB SET,ADD FEEDBACK */
 temp >>= 1; /* GO TO NEXT BIT */
 }
 return CRC;
}
