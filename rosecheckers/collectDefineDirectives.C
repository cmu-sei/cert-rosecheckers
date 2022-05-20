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

/**
 * \file collectDefineDirectives.C
 */

#include "rose.h"

using namespace std;

/**
 * \todo We need to figure out what this is all about and either finish or
 * delete it
 */

// Build a synthesized attribute for the tree traversal
class SynthesizedAttribute
   {
     public:
       // List of #define directives (save the PreprocessingInfo objects
       // so that we have all the source code position information).
          list<PreprocessingInfo*> accumulatedList;

          void display() const;
   };

void
SynthesizedAttribute::display() const
   {
     list<PreprocessingInfo*>::const_iterator i = accumulatedList.begin();
     while (i != accumulatedList.end())
        {
          printf ("CPP define directive = %s \n",(*i)->getString().c_str());
          i++;
        }
   }

class visitorTraversal : public AstBottomUpProcessing<SynthesizedAttribute>
   {
     public:
       // virtual function must be defined
          virtual SynthesizedAttribute evaluateSynthesizedAttribute (
                       SgNode* n, SynthesizedAttributesList childAttributes );
   };

SynthesizedAttribute
visitorTraversal::evaluateSynthesizedAttribute ( SgNode* n, SynthesizedAttributesList childAttributes )
   {
     SynthesizedAttribute localResult;

  // printf ("In evaluateSynthesizedAttribute(n = %p = %s) \n",n,n->class_name().c_str());

  // Build the list from children (in reverse order to preserve the final ordering)
     for (SynthesizedAttributesList::reverse_iterator child = childAttributes.rbegin(); child != childAttributes.rend(); child++)
        {
          localResult.accumulatedList.splice(localResult.accumulatedList.begin(),child->accumulatedList);
        }

  // Add in the information from the current node
     SgLocatedNode* locatedNode = isSgLocatedNode(n);
     if (locatedNode != NULL)
        {
          AttachedPreprocessingInfoType* commentsAndDirectives = locatedNode->getAttachedPreprocessingInfo();

          if (commentsAndDirectives != NULL)
             {
            // printf ("Found attached comments (to IR node at %p of type: %s): \n",locatedNode,locatedNode->class_name().c_str());
            // int counter = 0;

            // Use a reverse iterator so that we preserve the order when using push_front to add each directive to the accumulatedList
               AttachedPreprocessingInfoType::reverse_iterator i;
               for (i = commentsAndDirectives->rbegin(); i != commentsAndDirectives->rend(); i++)
                  {
                 // The different classifications of comments and directives are in ROSE/src/frontend/SageIII/rose_attributes_list.h
                    if ((*i)->getTypeOfDirective() == PreprocessingInfo::CpreprocessorDefineDeclaration)
                       {
#if 0
                         printf ("          Attached Comment #%d in file %s (relativePosition=%s): classification %s :\n%s\n",
                            counter++,(*i)->get_file_info()->get_filenameString().c_str(),
                            ((*i)->getRelativePosition() == PreprocessingInfo::before) ? "before" : "after",
                            PreprocessingInfo::directiveTypeName((*i)->getTypeOfDirective()).c_str(),
                            (*i)->getString().c_str());
#endif
                      // use push_front() to end up with source ordering of final list of directives
                         localResult.accumulatedList.push_front(*i);
                       }
                  }
             }
        }

  // printf ("localResult after adding current node info \n");
  // localResult.display();

     return localResult;
   }

int
main ( int argc, char* argv[] )
   {
     SgProject* project = frontend(argc,argv);

  // Build the traversal object and call "traverse" member function
     visitorTraversal exampleTraversal;
     SynthesizedAttribute results = exampleTraversal.traverse(project);

  // Output the results
     results.display();

     return 0;
   }
