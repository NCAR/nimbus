/* UnixTask.cc
   Spawns and manages a child process.

   Original Author: Jerry V. Pelk 
   Copyright by the National Center for Atmospheric Research
 
   Revisions:

*/

#include <UnixTask.h>


/* -------------------------------------------------------------------- */
UnixTask::UnixTask(char *name)
{
  int temp_pid;
  char *disc_dir, bin[512];

  if (name[0] == '/' || (disc_dir = getenv("DISC")) == NULL)
    strcpy(bin, name);
  else {
    strcpy(bin, disc_dir);
    strcat(bin, "/bin/");
    strcat(bin, name);
    }

printf("UnixTask: spawning [%s]\n", bin);

// Spawn the child process.
  switch (temp_pid = vfork()) {
    case ERROR:
      perror("UnixTask: vfork");
      exit (ERROR);

    case 0:
      if (name[0] == '/')
        (void)execl(bin, bin, "-rt", NULL);
      else
        (void)execl(bin, bin, 0, NULL);

      perror("UnixTask: execl");
      break;

   default:
      pid=temp_pid;

    break;
  }
}

/* -------------------------------------------------------------------- */
void UnixTask::killTask()
{
  if (kill (pid, SIGUSR2) == ERROR) 
    perror ("UnixTask: kill SIGUSR2");
}
