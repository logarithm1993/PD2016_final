/**CFile****************************************************************

  FileName    [MPTreeMgr.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [Constructor/Destructor.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

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

  Synopsis    [constructor]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

MPTreeMgr::MPTreeMgr()
{
   _allNode.reserve  ( 1024 );
   _allNet.reserve   ( 1024 );
   _allTerm.reserve  ( 1024 );
   _treeRoot.reserve ( 16 );
   
   _chipWidth  = 0;
   _chipHeight = 0;
   _cntrArea   = 0.0;
}

/**Function*************************************************************

  Synopsis    [destructor]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

MPTreeMgr::~MPTreeMgr()
{
	freeNode();
	freeNet();
	freeTerm();
}

void
MPTreeMgr::freeNode()
{
	int i , n;
	if ( !_allNode.empty() ) {
	   for ( i = 0 , n = _allNode.size(); i < n ; ++i ) {
		   if ( _allNode[i] ) {
				freeNodePin( _allNode[i] );
				delete _allNode[i];
				_allNode[i] = NULL;
			}
		}
	}
   
   for ( i = 0 , n = _treeRoot.size() ; i < n ; ++i ) {
      if ( _treeRoot[i] ) {
         delete _treeRoot[i];
         _treeRoot[i] = NULL;
      }
   }
}

void
MPTreeMgr::freeNodePin( Node * node )
{
	int i , n;
   if ( !node->_pinList.empty() ) {
	   for ( i = 0 , n = node->_pinList.size() ; i < n ; ++i ) {
		   if ( node->_pinList[i] ) {
			   delete node->_pinList[i];
				node->_pinList[i] = NULL;
			}
		}
	}
}

void
MPTreeMgr::freeNet()
{
	int i , n;
	if ( !_allNet.empty() ) {
	   for ( i = 0 , n = _allNet.size(); i < n ; ++i ) {
		   if( _allNet[i] ) {
				delete _allNet[i];
				_allNet[i] = NULL;
			}
		}
	}
}

void
MPTreeMgr::freeTerm()
{
   int i , n;
	if ( !_allTerm.empty() ) {
	   for ( i = 0 , n = _allTerm.size(); i < n ; ++i ) {
		   if( _allTerm[i] ) {
				delete _allTerm[i];
				_allTerm[i] = NULL;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
