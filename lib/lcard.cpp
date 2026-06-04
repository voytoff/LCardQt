#include "lcard.h"
#pragma hdrstop
#include <stdio.h>
#include <conio.h>
#include "ltr/include/ltrapi.h"
#pragma argsused

void print_oem(const char *s)
{
  char str[1024];
  AnsiToOem(s, str);
  printf(">> %s\n", str);
}

LCard::LCard() {}

void LCard::createList()
{
  INT res;
  TLTR ltr;
  //
  res=LTR_Init(&ltr);
  if(res==LTR_OK) {
    ltr.cc=CC_CONTROL;
    res=LTR_Open(&ltr);
    if(res==LTR_OK) {
      WORD mid[MODULE_MAX];
      res=LTR_GetCrateModules(&ltr, mid);
      if(res==LTR_OK) {
        print_oem("список установленных модулей");
        for(int i=0; i<MODULE_MAX; i++)
          printf(">> slot%0.2d  mid=%d\n", i+1, mid[i]&0xFF);
      }
    }
  }
  //
  print_oem(LTR_GetErrorString(res));
  print_oem("press any key...");
  while(!kbhit());
  getch();


}
