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

#include <iostream>
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
   //cout << " >  testing MPTree : \n";
   //Node * pNd1 , * pNd2;
   //int arg1 , arg2;

   //pNd1 = pNd2 = NULL;
   //arg1 = arg2 = -1;
   /*cout << "rotate a node : \n";
   perturbMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , 0 );
   cout << "node name = " << pNd1->_name 
        << " old Ort = "   << arg1 
        << " new Ort =  "  << Nz_Orient2Int( pNd1->_curOrt ) << endl;
   cout << "undo: \n" << endl;
   undoMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , 0 );
   cout << "node name = " << pNd1->_name << " ort =  " << Nz_Orient2Int( pNd1->_curOrt ) << endl;*/
   
   /*printMPTree();
   cout << "swap two subtrees : \n";
   perturbMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , 3 );
   cout << "swap " << arg1 << " and " << arg2 << endl;
   printMPTree();
   undoMPTree( &pNd1 , &pNd2 , &arg1 , &arg2 , 3 );
   cout << "undo : \n";
   printMPTree();*/
	//writeAllNode( cout );
	//writeAllTerm( cout );
	//writeAllNet ( cout );
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
