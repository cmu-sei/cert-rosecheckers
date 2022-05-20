/*
// <legal>CERT Rosecheckers// 
// Copyright 2022 Carnegie Mellon University.// 
// NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING INSTITUTE MATERIAL IS FURNISHED ON AN 'AS-IS' BASIS. CARNEGIE MELLON UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT, TRADEMARK, OR COPYRIGHT INFRINGEMENT.// 
// Released under a BSD (SEI)-style license, please see license.txt or contact permission@sei.cmu.edu for full terms.// 
// [DISTRIBUTION STATEMENT A] This material has been approved for public release and unlimited distribution.  Please see Copyright notice for non-US Government use and distribution.// 
// CERT® is registered in the U.S. Patent and Trademark Office by Carnegie Mellon University.// 
// This Software includes and/or makes use of Additional Software Dependencies, each subject to its own license.// 
// DM21-0505</legal>// 
 */

#include "util.h"

void DCL01_1(void) {
  int tmp=1;
  while(1) {
    int tmp=0;
    /* use tmp */
    printf("tmp=%d\n", tmp);
  }
}

void DCL02_1(void) {
  const int notel = 6;
  const int note1 = 4;
  if (notel == note1) {
    printf("We found notel to be wrong");
  }
}

void DCL02_2(void) {
  const int home = 6;
  const int nome = 4;
  if (home == nome) {
    printf("We found home to be wrong");
  }
}
