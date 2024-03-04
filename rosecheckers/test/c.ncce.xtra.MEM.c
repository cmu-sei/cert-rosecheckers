/*
// <legal>
// CERT Rosecheckers
// Copyright 2022 Carnegie Mellon University.
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.
// CERT(R) is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.
// DM21-0505
// </legal>
 */

#include "util.h"
#include <sys/resource.h>


void MEM31(void) {
  typedef struct ll {
    struct ll *next;
  } ll_t;

  ll_t *head = (ll_t*) malloc(sizeof(ll_t)); // initialize
  ll_t *temp;

  //free elements of linked list
  while(head != NULL) {
    temp=head->next;
    free(head);
    head=temp;
  }

  //error deleting temporary variable
  free(temp);
}


enum { LEN = 1000 };

void MEM34(int argc, char const *argv[]) {
  char *str = NULL;
  if (argc == 2) {
    str = (char *)malloc(LEN);
  }
  else if(argc == 3){
    str = "usage: $>a.exe [string]";
  }
  else{
    str = (char *)malloc(LEN / 2);
  }

  if(str == NULL){
    /* Handle error*/
  }
  free(str);
}
