/**CFile****************************************************************

  FileName    [MPTreeOp.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [functions under works]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [June 10, 2016]

***********************************************************************/

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

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
