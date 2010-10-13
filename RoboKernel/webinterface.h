#ifndef GDDGWEBINTERFACE_H_INCLUDED
#define GDDGWEBINTERFACE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

int OpenWebInterface();
int CloseWebInterface();
int WebIntNeedsNewSnapshot();
int TakeCareOfNetworkInterface(unsigned int clock_time);
int UpdateNetworkInterface(char * pica,char * picb,char * picc,char * picd,unsigned int xsize,unsigned int ysize);

int WriteConsoleOutput(char * outstr);
int EraseConsoleOutput();

#endif // WEBINTERFACE_H_INCLUDED