/*
 * <legal>
 * CERT Rosecheckers
 * Copyright 2007-2021 Carnegie Mellon University.
 *
 * NO WARRANTY. THIS CARNEGIE MELLON UNIVERSITY AND SOFTWARE ENGINEERING
 * INSTITUTE MATERIAL IS FURNISHED ON AN "AS-IS" BASIS. CARNEGIE MELLON
 * UNIVERSITY MAKES NO WARRANTIES OF ANY KIND, EITHER EXPRESSED OR
 * IMPLIED, AS TO ANY MATTER INCLUDING, BUT NOT LIMITED TO, WARRANTY OF
 * FITNESS FOR PURPOSE OR MERCHANTABILITY, EXCLUSIVITY, OR RESULTS
 * OBTAINED FROM USE OF THE MATERIAL. CARNEGIE MELLON UNIVERSITY DOES NOT
 * MAKE ANY WARRANTY OF ANY KIND WITH RESPECT TO FREEDOM FROM PATENT,
 * TRADEMARK, OR COPYRIGHT INFRINGEMENT.
 *
 * Released under a BSD (SEI)-style license, please see license.txt or
 * contact permission@sei.cmu.edu for full terms.
 *
 * [DISTRIBUTION STATEMENT A] This material has been approved for public
 * release and unlimited distribution.  Please see Copyright notice for
 * non-US Government use and distribution.
 *
 * CERT® is registered in the U.S. Patent and Trademark Office by
 * Carnegie Mellon University.
 *
 * This Software includes and/or makes use of the following Third-Party
 * Software subject to its own license:
 *
 * 1. rose
 *    (https://github.com/rose-compiler/rose/blob/weekly/LicenseInformation/ROSE_BSD_License.txt)
 *    Copyright 2005 Lawrence Livermore National Laboratory.
 *
 * DM21-0505
 * </legal>
 */

// Generates a CFG dot file from a C file.

#include "rose.h"
#include "cfgToDot.h"
using namespace std;

int main(int argc, char *argv[]) {
  // Build the AST used by ROSE
  SgProject* sageProject = frontend(argc,argv);

  // Run internal consistency tests on AST
  AstTests::runAllTests(sageProject);

  // Print the CFG view of each procedure in the AST in Dot format to cout
  NodeQuerySynthesizedAttributeType functions = NodeQuery::querySubTree(sageProject, V_SgFunctionDefinition);
  for (NodeQuerySynthesizedAttributeType::const_iterator i = functions.begin(); i != functions.end(); ++i) {
    SgFunctionDefinition* proc = isSgFunctionDefinition(*i);
    if (proc) {
      // Print out the full CFG, including bookkeeping nodes
      //      VirtualCFG::cfgToDotForDebugging(cout, proc->get_declaration()->get_name(), proc->cfgForBeginning());
      // Print out only those nodes which are "interesting" for analyses
      VirtualCFG::cfgToDot(cout, proc->get_declaration()->get_name(), VirtualCFG::makeInterestingCfg(proc));
    }
  }

  return 0;
}
