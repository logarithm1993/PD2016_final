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
   List* bcontour = new List(_chipWidth);
   List* tcontour = new List(_chipWidth);
   Node* root = NULL;
   bool isbottom,isfromleft;
  
   // BL-tree
   isbottom = isfromleft = true;
   root = _treeRoot[0]->_curPtr._left;
   if(!packcorner(root,bcontour,isbottom,isfromleft)) return false;
   // BR-tree
   isbottom = true; isfromleft = false;
   root = _treeRoot[1]->_curPtr._left;
   if(!packcorner(root,bcontour,isbottom,isfromleft)) return false;
   // TL-tree
   isbottom = false; isfromleft = true;
   root = _treeRoot[2]->_curPtr._left;
   if(!packcorner(root,tcontour,isbottom,isfromleft)) return false;
   // TR-tree
   isbottom = isfromleft = false;
   root = _treeRoot[2]->_curPtr._right;
   if(!packcorner(root,tcontour,isbottom,isfromleft)) return false;

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

   delete bcontour;
   delete tcontour;
   return true;
}
bool
MPTreeMgr::packcorner( Node* const & root, List*& contour, const bool& isbottom, const bool& isfromleft)
{
   int& x0 = root->_curCord._x;
   int& y0 = root->_curCord._y;
   if(isfromleft) x0 = 0;
   else           x0 = _chipWidth-root->_width;
   
   int y = contour->update(x0,x0+root->_width,root->_height,isfromleft);

   if(isbottom)   y0 = y;
   else           y0 = _chipHeight - y - root->_height;

   if(!packMPTree_rec(root->_curPtr._left,contour,isbottom,isfromleft))  {delete contour; return false;}
   if(!packMPTree_rec(root->_curPtr._right,contour,isbottom,isfromleft)) {delete contour; return false;}
   return true;
}
bool
MPTreeMgr::packMPTree_rec( Node* const &  n, List*& contour, const bool& isbottom, const bool& isfromleft)
{
   // left-first inorder traversal
   if(n == NULL) return true;
   const Node* const & p = n->_curPtr._p;
   const int& px0 = p->_curCord._x;
   const int  px1 = px0 + p->_width;
   int& nx0 = n->_curCord._x;
   int& ny0 = n->_curCord._y;   

   if(isbottom) {
      if(isfromleft) {
         if(p->_curPtr._left == n) nx0 = px1;
         else                      nx0 = px0;

         ny0 = contour->update(nx0,nx0+n->_width,n->_height,isfromleft);
         if( nx0 + n->_width > _chipWidth || ny0 + n->_height > _chipHeight ) return false;
      }
      else {
         if(p->_curPtr._left == n) nx0 = px0 - n->_width;
         else                      nx0 = px1 - n->_width;

         ny0 = contour->update(nx0,nx0+n->_width,n->_height,isfromleft);
         if( nx0 < 0 || ny0 + n->_height > _chipHeight ) return false;
      }
   }
   else {
      if(isfromleft) {
         if(p->_curPtr._left == n) nx0 = px1;
         else                      nx0 = px0;
         ny0 = _chipHeight - contour->update(nx0,nx0+n->_width,n->_height,isfromleft) - n->_height;
         if( nx0 + n->_width > _chipWidth || ny0 < 0 ) return false;
      } 
      else {
         if(p->_curPtr._left == n) nx0 = px0 - n->_width;
         else                      nx0 = px1 - n->_width;

         ny0 = _chipHeight - contour->update(nx0,nx0+n->_width,n->_height,isfromleft) - n->_height;
         if( nx0 < 0 || ny0 < 0 ) return false;
      }
   }
   packMPTree_rec(n->_curPtr._left,contour,isbottom,isfromleft);
   packMPTree_rec(n->_curPtr._right,contour,isbottom,isfromleft);
   return true;
}

