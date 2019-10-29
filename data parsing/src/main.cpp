#include <Arduino.h>

//this can currently only send positive integers max size of 14 bits
//an offset can be used and set along with the data to enlarge the data size for sending

const int numbofvariables = 4/*can be set to any number of variables*/, datasize = (numbofvariables * 2)+2;  //set number of bytes needed for all the data
short datapack[datasize]; //int an array to put all the data you want to send

void setup() 
{

}

void communicate(void){   //data conversion and transmition over serial
  byte data[datasize];//create byte array size of data
  //convert number to bytes with 'sync bits' as 8th bit
  
  //create first packet of data with a 1 in the 8th position
  data[0] = datapack[0]>>7;
  data[0] = data[0]|0x80;//8th bit to 1

  //create the rest of the data with a 8th bit as zero
  data[1] = datapack[0]&0xFF;
  data[1] = data[1]&0x7F;//8thbit to 0

  //loop so this can be used for multiple data sizes
  for(int i = 1; i <= numbofvariables; i++){
    int n = 2*i;
    data[n] = datapack[i]>>7;
    data[n] = data[n]&0x7F;//8thbit to 0
    data[n+1] = datapack[i]&0x7F;
    data[n+1] = data[n+1]&0x7F;//8thbit to 0
  }

  //create checksum but summing all the data
  unsigned short checksum = 0;
  for( int i = 0; i <= 1+((numbofvariables-1)*2); i++){
    checksum = checksum + data[i];
  }

  //fill end of data array with checksum
  data[datasize-2] = checksum>>7;
  data[datasize-2] = data[datasize-2]&0x7F;//8thbit to 0
  data[datasize-1] = checksum&0x7F;
  data[datasize-1] = data[datasize-1]&0x7F;//8thbit to 0
  Serial.write(data, datasize);//send array with a buffer size of datasize
}

void loop(){

}