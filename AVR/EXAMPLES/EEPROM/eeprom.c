// EEPROM access example

// CodeVisionAVR C Compiler
// (C) 2000-2009 HP InfoTech S.R.L.
// www.hpinfotech.ro

// Chip: ATmega8515

flash char f[]="This is a test";
#pragma warn-
eeprom char e[16];
#pragma warn+
char r[16];

void main (void)
{
char flash *ptr_to_flash;
char eeprom *ptr_to_eeprom;
char *ptr_to_ram;

// copy the string f from FLASH to
// the string e in EEPROM
ptr_to_flash=f;
ptr_to_eeprom=e;
while (*ptr_to_flash)
      *ptr_to_eeprom++=*ptr_to_flash++;
// NULL terminate the string in EEPROM
*ptr_to_eeprom=0;

// copy the string e from EEPROM to
// the string r in RAM
ptr_to_eeprom=e;
ptr_to_ram=r;
while (*ptr_to_eeprom)
      *ptr_to_ram++=*ptr_to_eeprom++;
// NULL terminate the string in RAM
*ptr_to_ram=0;

// stop here
while (1);
}
