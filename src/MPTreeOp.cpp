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
	int index1 , index2;
	if ( *pNd1 && *pNd2 ) swapNode_int( *pNd1 , *pNd2 ); // undo
	else { // swap two nodes
	   getTwoRandNum( _allNode.size() , &index1 , &index2 );
		*pNd1 = _allNode[index1];
		*pNd2 = _allNode[index2];
		swapNode_int( *pNd1 , *pNd2 );
	}

}

void
MPTreeMgr::swapNode_int( Node * pNd1 , Node * pNd2 )
{
	//cout << " >  swapping " << pNd1->_name << " and " << pNd2->_name << endl;
   if ( checkParentChild( pNd1 , pNd2 ) )  swapParentChild( pNd1 , pNd2 );
   else if ( checkSibling( pNd1 , pNd2 ) ) swapSibling( pNd1 , pNd2 );
   else {
		changeNbrPtr( pNd1 , pNd2 );
		changeNbrPtr( pNd2 , pNd1 );
	   (pNd1->_curPtr).swapTriPtr( pNd2->_curPtr );
	}
}

void
MPTreeMgr::changeNbrPtr( Node * pNd1 , Node * pNd2 )
{
	// point pNd1`s _p , _left , _right to pNd2
   Node * pParent , * pLeft , * pRight;
   
	pParent = pNd1->_curPtr._p;
   pLeft   = pNd1->_curPtr._left;
   pRight  = pNd1->_curPtr._right;

	if ( pParent ) pParent->setChild( pNd2 , pLRChild( pNd1 ) );
	if ( pLeft   ) pLeft ->setParent( pNd2 );
	if ( pRight  ) pRight->setParent( pNd2 );
}

bool
MPTreeMgr::pLRChild( const Node * pNd ) const
{
   if ( (((pNd->_curPtr)._p)->_curPtr)._left  == pNd ) return 0;
   if ( (((pNd->_curPtr)._p)->_curPtr)._right == pNd ) return 1;
	cout << "[Error] fail to find child!\n";
	assert(0);
	return 1;
}

bool
MPTreeMgr::checkParentChild( const Node * pNd1 , const Node * pNd2 ) const
{
	if ( (pNd1->_curPtr)._p == pNd2 ) return true;
	if ( (pNd2->_curPtr)._p == pNd1 ) return true;
	return false;
}

void
MPTreeMgr::swapParentChild( Node * pNd1 , Node * pNd2 )
{
	Node * pParent , * pChild , * pGrandParent;

	if ( pNd1->_curPtr._p == pNd2 ) {
	   pParent = pNd2;
		pChild  = pNd1;
	}
	else {
	   pParent = pNd1;
		pChild  = pNd2;
	}
	pGrandParent  = pParent->_curPtr._p;

	changeNbrPtr( pChild  , pParent );
	changeNbrPtr( pParent , pChild  );
	pParent->_curPtr.swapTriPtr( pChild->_curPtr );
	pParent->setParent( pChild );
	pChild ->setParent( pGrandParent );
}

bool
MPTreeMgr::checkSibling( const Node * pNd1 , const Node * pNd2 ) const
{
	return ( (pNd1->_curPtr)._p == (pNd2->_curPtr)._p );
}

void
MPTreeMgr::swapSibling( Node * pNd1 , Node * pNd2 )
{
   Node * pParent;
   bool right;

   pParent = pNd1->_curPtr._p;
   right   = pLRChild( pNd1 );

   changeNbrPtr( pNd1 , pNd2 );
	changeNbrPtr( pNd2 , pNd1 );
	if ( right ) {
      pParent->_curPtr._left  = pNd1;
      pParent->_curPtr._right = pNd2;
   }
   else {
      pParent->_curPtr._left  = pNd2;
      pParent->_curPtr._right = pNd1;
   }
   (pNd1->_curPtr).swapTriPtr( pNd2->_curPtr );
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
