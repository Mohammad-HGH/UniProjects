/*
  Prototypes for the Dallas Semiconductor
  DS1621 I2C bus thermometer/thermostat functions
  for ATxmega devices

  CodeVisionAVR C Compiler
  (C) 1998-2010 Pavel Haiduc, HP InfoTech S.R.L.
*/

#ifndef _DS1621X_INCLUDED_
#define _DS1621X_INCLUDED_

#include "twix.h"

bool ds1621_init(TWI_MASTER_INFO_t *ptwim,
unsigned char chip,signed char tlow,signed char thigh,unsigned char pol);
unsigned char ds1621_get_status(unsigned char chip);
bool ds1621_set_status(unsigned char chip,unsigned char status);
bool ds1621_start(unsigned char chip);
bool ds1621_stop(unsigned char chip);
float ds1621_temperature(unsigned char chip);

#endif

