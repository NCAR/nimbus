/* UnixTask.h
   Spawns and manages a child process.

   Original Author: Jerry V. Pelk
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#ifndef UNIXTASK_H
#define UNIXTASK_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dsmctl.h>

class UnixTask {
public:
  UnixTask(char*);				// constructor
  void killTask();				// kill the task

private:
  char task_name[20];
  int pid;
};

#endif
