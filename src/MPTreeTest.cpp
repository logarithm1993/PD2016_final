/**CFile****************************************************************

  FileName    [MPTreeTest.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [test interface.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <iostream>
#include <cassert>
#include "MPTreeMgr.h"
#include "utility.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [test interface]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::test()
{
#if 1
   cout << " >  testing MPTree operations: \n";
   Node * pNd1 , * pNd2 , * pNd3;
   int move , arg1 , arg2 , i , n;

   pNd1 = pNd2 = NULL;
   arg1 = arg2 = -1;

   printMPTree();
   for ( i = 0 , n = 100000 ; i < n ; ++i ) {
      move = rand() % 4;
      perturbMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , move );
      undoMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , move );
      pNd1 = pNd2 = NULL;
      arg1 = arg2 = -1;
   }
   printMPTree();
#endif
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
