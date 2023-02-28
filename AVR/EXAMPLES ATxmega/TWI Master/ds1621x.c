/*
  Dallas Semiconductor DS1621 I2C bus
  thermometer/thermostat functions
  for ATxmega devices

  CodeVisionAVR V2.0 C Compiler
  (C) 1998-2010 Pavel Haiduc, HP InfoTech S.R.L.
*/

#include "ds1621x.h"

static TWI_MASTER_INFO_t *ptwi_master;

unsigned char ds1621_get_status(unsigned char chip)
{
unsigned char data=0xac,status;
twi_master_trans(ptwi_master,(0x90>>1)|chip,&data,1,&status,1);
return status;
}

bool ds1621_set_status(unsigned char chip,unsigned char status)
{
unsigned char data[2];
data[0]=0xac;
data[1]=status;
return twi_master_trans(ptwi_master,(0x90>>1)|chip,data,2,NULL,0);
}

bool ds1621_set_temp(unsigned char chip,unsigned char cmd,char temp)
{
unsigned char data[3];
data[0]=cmd;
data[1]=temp;
data[2]=0;
if (!twi_master_trans(ptwi_master,(0x90>>1)|chip,data,3,NULL,0)) return false;
while (ds1621_get_status(chip) & 0x10);
return true;
}

bool ds1621_start(unsigned char chip)
{
unsigned char cmd=0xee;
return twi_master_trans(ptwi_master,(0x90>>1)|chip,&cmd,1,NULL,0);
}

bool ds1621_stop(unsigned char chip)
{
unsigned char cmd=0x22;
return twi_master_trans(ptwi_master,(0x90>>1)|chip,&cmd,1,NULL,0);
}

float ds1621_temperature(unsigned char chip)
{
unsigned char tt[2];
unsigned char cmd=0xaa;
if (!twi_master_trans(ptwi_master,(0x90>>1)|chip,&cmd,1,tt,2))
   return -999; // return invalid temp. in case of communication error
return ((int) (((unsigned int) tt[0]<<8)+tt[1])>>7)*0.5;
}

bool ds1621_init(TWI_MASTER_INFO_t *ptwim,unsigned char chip,signed char tlow,signed char thigh,
unsigned char pol)
{
ptwi_master=ptwim;
//set configuration register
if (!ds1621_set_status(chip,8+(pol<<1))) return false;
//set low temperature
if (!ds1621_set_temp(chip,0xa2,tlow)) return false;
//set high temperature
if (!ds1621_set_temp(chip,0xa1,thigh)) return false;
//start temperature conversions
return ds1621_start(chip);
}
