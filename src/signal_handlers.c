#include "signal_handlers.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void catch_sigint(int signalNo)
{
  // TODO: File this!i
  printf("\n");
  return; 
}

void catch_sigtstp(int signalNo)
{
  // TODO: File this!
 printf("\n");
 return;
}
