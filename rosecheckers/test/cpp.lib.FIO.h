#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <fstream>
#include <syslog.h>

#define BUFFERSIZE 10

int verify_file(char *const filename) {
  /* Get /etc/passwd entry for current user */
  struct passwd *pwd = getpwuid(getuid());
  if (pwd == NULL) {
    /* Handle error */
    return 0;
  }

  const unsigned int len = strlen( pwd->pw_dir);
  if (strncmp( filename, pwd->pw_dir, len) != 0) {
    return 0;
  }
  /* Make sure there is only one '/', immediately after homedir */
  if (strrchr( filename, '/') == filename + len) {
    return 1;
  }
  return 0;
}
