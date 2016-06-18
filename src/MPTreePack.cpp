/**CFile****************************************************************

  FileName    [MPTreePack.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [functions under works]

  Author      [TBD]
  
  Affiliation [NTU]

  Date        [Jun 1, 2016]

***********************************************************************/
#include "MPTreeMgr.h"
#include "Element.h"
#include "utility.h"
#include <cassert>

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [ pack MPTree ]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
bool
MPTreeMgr::packMPTree()
{
   _chipWidth  = _initChipWidth;
   _chipHeight = _initChipHeight; 
   double factor = 0.9;
   bool isfirst = true;
   do {
      if(!packMPTree_int()) {
         if(isfirst) return false;
         else        {
            _chipWidth  = (int)(_chipWidth/factor) + 1;
            _chipHeight = (int)(_chipHeight/factor) + 1;
            assert( packMPTree_int() );
            return true;
         }
      }
      isfirst     = false;
      _chipWidth  = (int)(_chipWidth*factor);
      _chipHeight = (int)(_chipHeight*factor);
   } while(1);
   
}
bool
MPTreeMgr::packMPTree_int() 
{
   List* bcontour = new List(_chipWidth);
   List* tcontour = new List(_chipWidth);
   Node* root = 0;
   bool isbottom,isfromleft;
   _deadArea = 0;
   /*for( size_t i = 0, lsize = _allNode.size(); i < lsize; ++i) {
      _allNode[i]->_curCord._x = -1;
      _allNode[i]->_curCord._y = -1;
   }*/

   // BL-tree
   isbottom = isfromleft = true;
   root = _treeRoot[0]->_curPtr._left;
   if(!packcorner(root,bcontour,isbottom,isfromleft)) {delete bcontour; delete tcontour; return false;}
   //_BLArea = bcontour->area();
   // BR-tree
   isbottom = true; isfromleft = false;
   root = _treeRoot[1]->_curPtr._left;
   if(!packcorner(root,bcontour,isbottom,isfromleft)) {delete bcontour; delete tcontour; return false;}
   //_BRArea = bcontour->area() - _BLArea;
   bcontour->compute_balance_area(_BLArea,_BRArea,_chipWidth);
   // TL-tree
   isbottom = false; isfromleft = true;
   root = _treeRoot[2]->_curPtr._left;
   if(!packcorner(root,tcontour,isbottom,isfromleft)) {delete bcontour; delete tcontour; return false;}
   //_TLArea = tcontour->area();
   // TR-tree
   isbottom = isfromleft = false;
   root = _treeRoot[2]->_curPtr._right;
   if(!packcorner(root,tcontour,isbottom,isfromleft)) {delete bcontour; delete tcontour; return false;}
   //_TRArea = tcontour->area() - _TLArea;
   tcontour->compute_balance_area(_TLArea,_TRArea,_chipWidth);
   // check contour overlaps
   ListNode* it1 = bcontour->_begin->_next;
   ListNode* it2 = tcontour->_begin->_next;
   while( it1 != bcontour->_end && it2 != tcontour->_end ) {
      if( it1->_y1 + it2->_y1 > _chipHeight ) return false;
      if( it1->_x1 == it2->_x1 ) { it1 = it1->_next; it2 = it2->_next; }
      else if( it1->_x1 > it2->_x1 ) it2 = it2->_next;
      else it1 = it1->_next;
   }
   assert(it1 == bcontour->_end && it2 == tcontour->_end);
   double sum = 0;
   it1 = bcontour->_begin->_next;
   while( it1 != bcontour->_end ) {
      sum += (it1->_x1 - it1->_x0) * it1->_y1;
      it1 = it1->_next;
   }
   it1 = tcontour->_begin->_next;
   while( it1 != tcontour->_end ) {
      sum += (it1->_x1 - it1->_x0) * it1->_y1;
      it1 = it1->_next;
   }
   _cntrArea = sum;
   delete bcontour;
   delete tcontour;
   return true;
}
bool
MPTreeMgr::packcorner( Node* const & root, List*& contour, const bool& isbottom, const bool& isfromleft)
{
   if(root == 0) return true; 
   int& x0 = root->_curCord._x;
   int& y0 = root->_curCord._y;
   if(isfromleft) x0 = 0;
   else           x0 = _chipWidth - root->width();
   
   int y = contour->update(x0,x0+root->width(),root->height(),isfromleft,_deadArea);

   if(isbottom)   y0 = y;
   else           y0 = _chipHeight - y - root->height();

   if(!packMPTree_rec(root->_curPtr._left,contour,isbottom,isfromleft))  return false;
   if(!packMPTree_rec(root->_curPtr._right,contour,isbottom,isfromleft)) return false; 
   return true;
}
bool
MPTreeMgr::packMPTree_rec( Node* const &  n, List*& contour, const bool& isbottom, const bool& isfromleft)
{
   // left-first inorder traversal
   if(n == 0) return true;
   const Node* const & p = n->_curPtr._p;
   const int& px0 = p->_curCord._x;
   const int  px1 = px0 + p->width();
   int& nx0 = n->_curCord._x;
   int& ny0 = n->_curCord._y;   

   if(isbottom) {
      if(isfromleft) {
         if(p->_curPtr._left == n) nx0 = px1;
         else                      nx0 = px0;
         if( nx0 + n->width() > _chipWidth )   return false;
         ny0 = contour->update(nx0,nx0+n->width(),n->height(),isfromleft,_deadArea);
         if( ny0 + n->height() > _chipHeight ) return false;
      }
      else {
         if(p->_curPtr._left == n) nx0 = px0 - n->width();
         else                      nx0 = px1 - n->width();
         if ( nx0 < 0 ) return false;
         ny0 = contour->update(nx0,nx0+n->width(),n->height(),isfromleft,_deadArea);
         if( ny0 + n->height() > _chipHeight ) return false;
      }
   }
   else {
      if(isfromleft) {
         if(p->_curPtr._left == n) nx0 = px1;
         else                      nx0 = px0;
         if( nx0 + n->width() > _chipWidth ) return false;
         ny0 = _chipHeight - contour->update(nx0,nx0+n->width(),n->height(),isfromleft,_deadArea) - n->height();
         if( ny0 < 0 ) return false;
      } 
      else {
         if(p->_curPtr._left == n) nx0 = px0 - n->width();
         else                      nx0 = px1 - n->width();
         if( nx0 < 0 ) return false;
         ny0 = _chipHeight - contour->update(nx0,nx0+n->width(),n->height(),isfromleft,_deadArea) - n->height();
         if( ny0 < 0 ) return false;
      }
   }
   if(!packMPTree_rec(n->_curPtr._left,contour,isbottom,isfromleft))  return false;
   if(!packMPTree_rec(n->_curPtr._right,contour,isbottom,isfromleft)) return false;
   return true;
}

