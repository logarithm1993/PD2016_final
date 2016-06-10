/**CFile****************************************************************

  FileName    [MPTreeOp.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [functions under works]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [June 10, 2016]

***********************************************************************/

#include <cstdlib>
#include <cassert>
#include "MPTreeMgr.h"
#include "Element.h"
#include "utility.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [perturb/undo MP tree : SA interface]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::perturbMPTree( Node ** pNd1 , Node ** pNd2 , 
                          int * arg1   , int * arg2   , 
                          int type )
{
   switch ( type )
   {
      case 0:
         rotateNode( pNd1 , arg1 );
         break;
      case 1:
         delInsNode( pNd1 , arg1 , pNd2 , arg2 );
         break;
      case 2:
         swapNode( pNd1 , pNd2 );
         break;
      case 3:
         swapSubTree( arg1 , arg2 );
         break;
   }
}

void
MPTreeMgr::undoMPTree( Node ** pNd1 , Node ** pNd2 , 
                       int * arg1   , int * arg2   , 
                       int type )
{
   switch ( type )
   {
      case 0:
         rotateNode( pNd1 , arg1 );
         break;
      case 1:
         delInsNode( pNd2 , arg2 , pNd1 , arg1 );
         break;
      case 2:
         swapNode( pNd1 , pNd2 );
         break;
      case 3:
         swapSubTree( arg1 , arg2 );
         break;
   }
}

/**Function*************************************************************

  Synopsis    [Operation 1 : rotate a node]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::rotateNode( Node ** pNode , int * ort )
{
   // TODO
   int newOrt;
   if ( *pNode ) { // undo
      if ( *ort < 0 || *ort > 7 ) { 
         cout << "[Error] rotateNode() : invalid ort!\n";
         assert(0);
      }
      (*pNode)->_curOrt = Nz_Int2Orient( *ort );
   }
   else { // rotate
      *pNode = _allNode[ rand() % _allNode.size() ];
      if ( *ort != -1 ) cout << "[Warning] rotateNode() : ort not clear!\n";
      *ort   = Nz_Orient2Int( (*pNode)->_curOrt );
      newOrt = rand() % 8;
      while ( newOrt == *ort ) newOrt = rand() % 8;
      (*pNode)->_curOrt = Nz_Int2Orient( newOrt );
   }
}

/**Function*************************************************************

  Synopsis    [Operation 2 : delete and insert a node]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::delInsNode( Node ** pNd1 , int * right1 , 
                       Node ** pNd2 , int * right2 )
{
   // TODO
}

/**Function*************************************************************

  Synopsis    [Operation 3 : swap two nodes]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::swapNode( Node ** pNd1 , Node ** pNd2 )
{
   // TODO
}

/**Function*************************************************************

  Synopsis    [Operation 4 : swap two subtrees]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::swapSubTree( int * sub1 , int * sub2 )
{
   // TODO
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
