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

static void getTwoRandNum( int , int * , int * );

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
   if ( *sub1 != -1 && *sub2 != -1 ) // undo
      swapSubTree_int( *sub1 , *sub2 );
   else { // swap two subtrees
      //*sub1 = rand() % 4;
      //*sub2 = rand() % 4;
      //while ( *sub1 == *sub2 ) *sub2 = rand() % 4;
      getTwoRandNum( 4 , sub1 , sub2 ); // choose from 4 subtrees
      swapSubTree_int( *sub1 , *sub2 );
   }
}

void
MPTreeMgr::swapSubTree_int( int sub1 , int sub2 )
{
   if ( sub1 == sub2 ) {
      cout << "[Warning] swapSubTree() : same subtree index!\n";
      return;
   }
   if ( sub1 < 0 || sub1 > 3 ) {
      cout << "[Error] swapSubTree() : wrong subtree index (sub1)!\n";
      assert(0);
   }
   if ( sub2 < 0 || sub2 > 3 ) {
      cout << "[Error] swapSubTree() : wrong subtree index (sub2)!\n";
      assert(0);
   }
   Node * pNd1 , * pNd2 , * pTemp;
   
   pNd1 = getSubTreeRoot( sub1 );
   pNd2 = getSubTreeRoot( sub2 );
   
   // swap children of treeRoot
   if ( sub1 == 3 ) {
      pTemp                          = _treeRoot[2]->_curPtr._right;
      _treeRoot[2]->_curPtr._right   = _treeRoot[sub2]->_curPtr._left;
      _treeRoot[sub2]->_curPtr._left = pTemp;
   }
   else if ( sub2 == 3 ) {
      pTemp                          = _treeRoot[2]->_curPtr._right;
      _treeRoot[2]->_curPtr._right   = _treeRoot[sub1]->_curPtr._left;
      _treeRoot[sub1]->_curPtr._left = pTemp;
   }
   else {
      pTemp                          = _treeRoot[sub1]->_curPtr._left;
      _treeRoot[sub1]->_curPtr._left = _treeRoot[sub2]->_curPtr._left;
      _treeRoot[sub2]->_curPtr._left = pTemp;
   }
   
   // swap parents of pNd
   pTemp            = pNd1->_curPtr._p;
   pNd1->_curPtr._p = pNd2->_curPtr._p;
   pNd2->_curPtr._p = pTemp;
}

Node*
MPTreeMgr::getSubTreeRoot( int sub )
{
   if ( sub == 0 ) return _treeRoot[0]->_curPtr._left;
   if ( sub == 1 ) return _treeRoot[1]->_curPtr._left;
   if ( sub == 2 ) return _treeRoot[2]->_curPtr._left;
   if ( sub == 3 ) return _treeRoot[2]->_curPtr._right;
}

/**Function*************************************************************

  Synopsis    [Local functions for this file]

  Description [get 2 different rand numbers]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
getTwoRandNum( int range , int * num1 , int * num2 )
{
   *num1 = rand() % range;
   *num2 = rand() % range;
   while ( *num1 == *num2 ) *num2 = rand() % range;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
