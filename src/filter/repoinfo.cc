#include <cstdio>
#include <unistd.h>

#include <nimbus.h>
#include <gui.h>

/*

#!/bin/bash

echo Tag and commit #....we have no tag/version at this point.
git describe --match [vV][0-9]*

echo Repo URL
git config --get remote.origin.url

echo "git log"
git log --pretty=format:"%cd,%H" -1

echo Dirty bits
git status --porcelain

*/


void executePopenCommand(const char cmd[], char result[], int n)
{
  FILE *pp;
  int rc;

  if ((pp = popen(cmd, "r")) == 0)
  {
    fprintf(stderr, "validateProjectDirectory: popen of [%s] failed\n", cmd);
    Quit(NULL, NULL, NULL);
  }

  rc = fread(result, 1, n, pp);
  result[rc-1] = 0;
  pclose(pp);
}

/* -------------------------------------------------------------------- */
void validateGitProjectDirectory()
{
  char cmd[200];

  chdir(cfg.ProjectDirectory().c_str());

  // Get repo URL
  sprintf(cmd, "git config --get remote.origin.url");
  executePopenCommand(cmd, buffer, 1000);
  cfg.SetProjectRepoURL(buffer);

  // Get repo hash
  sprintf(cmd, "git status porcelain");
  executePopenCommand(cmd, buffer, 1000);
  cfg.SetProjectDirectoryStatus(buffer);

  // Get repo status
  sprintf(cmd, "git log --pretty=format:\"%H\" -1");
  executePopenCommand(cmd, buffer, 1000);
  cfg.SetProjectDirectoryRevision(buffer);
}

/* -------------------------------------------------------------------- */
void validateProjectDirectory()
{
  char cmd[200];

  /* Project directories were moved from svn to git in June of 2020.
   * Start by testing subversion.  If that fails, then test for git.
   */

  // Get repo revision number of the project directory.
  sprintf(cmd, "svnversion %s/%s", cfg.ProjectDirectory().c_str(), cfg.ProjectNumber().c_str());
  executePopenCommand(cmd, buffer, 1000);

  if (strcmp(buffer, "Unversioned directory") == 0)
  {
    // This is good, we've moved to git.
    validateGitProjectDirectory();
  }
  else
    cfg.SetProjectDirectoryRevision(buffer);

}
