/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Third-Party Software subject to its own license.// 
// DM21-0505</legal>// 
 */

#include "util.h"
#include "cpp.util.h"

void SIG00();
void SIG01();
void SIG30();

void SIG() {
  SIG00();
  SIG01();
  SIG30();
}

volatile sig_atomic_t SIG00_sig1 = 0;
volatile sig_atomic_t SIG00_sig2 = 0;

void SIG00_handler(int signum) {
  if (signum == SIGUSR1) {
    SIG00_sig1 = 1;
  }
  else if (SIG00_sig1) {
    SIG00_sig2 = 1;
  }
}

void SIG00(void) {
  struct sigaction act;
  act.sa_handler = &SIG00_handler;
  act.sa_flags = 0;
  if (sigemptyset(&act.sa_mask) != 0) {
    return;
  }
  if (sigaddset(&act.sa_mask, SIGUSR1)) {
    return;
  }
  if (sigaddset(&act.sa_mask, SIGUSR2)) {
    return;
  }

  if (sigaction(SIGUSR1, &act, NULL) != 0) {
    return;
  }
  if (sigaction(SIGUSR2, &act, NULL) != 0) {
    return;
  }

  while (SIG00_sig2 == 0) {
    /* Do nothing or give up CPU for a while */
  }
}

void SIG01_handler(int signum) {
#ifndef WINDOWS
  if (signal(signum, SIG_DFL) == SIG_ERR) {
    /* Handler error */
  }
#endif
  /* Handle signal */
}

void SIG01(void) {
  struct sigaction act;
  act.sa_handler = &SIG01_handler;
  act.sa_flags = SA_RESETHAND;
  if (sigemptyset(&act.sa_mask) != 0) {
    return;
  }
  if (sigaction(SIGUSR1, &act, NULL) != 0) {
    return;
  }
}

volatile sig_atomic_t SIG30_eflag = 0;
char *SIG30_info = NULL;

void SIG30_log_message(void) {
  fprintf(stderr, SIG30_info);
}

void SIG30_handler(int signum) {
  SIG30_eflag = 1;
}

/* This is the same cce code for SIG31, SIG33, and SIG34 */
void SIG30(void) {
  if (signal(SIGINT, SIG30_handler) == SIG_ERR) {
    return;
  }
  SIG30_info = (char*)malloc(12);
  if (SIG30_info == NULL) {
    return;
  }

  while (!SIG30_eflag) {
    /* Main loop program code */

    SIG30_log_message();

    /* More program code */
  }

  SIG30_log_message();
  free(SIG30_info);
  SIG30_info = NULL;
}
