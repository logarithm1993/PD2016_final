/**CFile****************************************************************

  FileName    [Element.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [member functions of Node/Pin/Net/Term.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 29, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "MPTreeMgr.h"
#include "Element.h"
#include "utility.h"
#include <cassert>
#include <cfloat>
#include <cmath>

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Node Function*******************************************************
 
  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

int
Node::centerX() const
{
    // done 4U
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _curCord._x + _width/2;
    else
        return _curCord._x + _height/2;
    return -1;
}

int
Node::centerY() const
{
    // done 4U
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _curCord._y + _height/2;
    else
        return _curCord._y + _width/2;
    return -1;
}

int
Node::right() const
{
    // done NZ
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _curCord._x + _width;
    else
        return _curCord._x + _height;
    return -1;
}

int
Node::top() const
{
    // done NZ
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _curCord._y + _height;
    else
        return _curCord._y + _width;
    return -1;
}

int
Node::width() const
{
    // done NZ
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _width;
    else
        return _height;
    return -1;
}

int
Node::height() const
{
    // done NZ
    if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
        return _height;
    else
        return _width;
    return -1;
}

int
Node::displacement() const
{
   // done OAO
   int dx = _curCord._x - _initCord._x;
   int dy = _curCord._y - _initCord._y;
   
   return abs(dx) + abs(dy);
}

void
Node::updateOpt()
{
   // done OAO
   _optCord  = _curCord;
   _optOrt   = _curOrt;
   _optPtr   = _curPtr;
}

void
Node::updateCur()
{
   // done OAO
   _curCord = _optCord;
   _curOrt  = _optOrt;
   _curPtr  = _optPtr;
}

bool
Node::pLRChild() const
{
   if ( ((_curPtr._p)->_curPtr)._left  == this ) return 0;
   if ( ((_curPtr._p)->_curPtr)._right == this ) return 1;
	cout << "[Error] fail to find child!\n";
	assert(0);
	return 1;
}

bool
Node::checkNode() const
{
   Node * pParent , * pLeft , * pRight;
   
   pParent = _curPtr._p;
   pLeft   = _curPtr._left;
   pRight  = _curPtr._right;

   //cout << "check node " << _name << endl;
   // check parent
   if ( !pParent ) {
      if ( _name != "root-0" ) {
         cout << "[Error] checkNode() : dangling node (name = " << _name << ")\n";
         return false;
      }
   }
   else {
      if ( pParent == this ) {
         cout << "[Error] checkNode() : self parent node (name = " << _name << ")\n";
         return false;
      }
      if ( this != (pLRChild() ? pParent->_curPtr._right : pParent->_curPtr._left) ) {
         cout << "[Error] checkNode() : wrong parent child node (name = " << _name << ")\n";
         return false;
      }
   }
   // check left
   if ( pLeft ) {
      if ( pLeft == this ) {
         cout << "[Error] checkNode() : self left node (name = " << _name << ")\n";
         return false;
      }
      if ( pLeft->_curPtr._p != this ) {
         cout << "[Error] checkNode() : wrong parent left child node (name = " << _name << ")\n";
         return false;
      }
   }
   // check right
   if ( pRight ) {
      if ( pRight == this ) {
         cout << "[Error] checkNode() : self right node (name = " << _name << ")\n";
         return false;
      }
      if ( pRight->_curPtr._p != this ) {
         cout << "[Error] checkNode() : wrong parent right child node (name = " << _name << ")\n";
         return false;
      }
   }
   return true;
}

/**Pin Function*********************************************************
 
  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

double
Pin::x() const
{
    // done 4U
    double x  = (double)_node->_curCord._x;
    double w  = (double)_node->_width;
    double w2 = (double)(_node->_width/2);
    double h  = (double)_node->_height;
    double h2 = (double)(_node->_height/2);
    switch(_node->_curOrt) {
        case N :  return x + w2 + _offsetX; 
        case W :  return x + h - h2 - _offsetY;
        case S :  return x + w - w2 - _offsetX;
        case E :  return x + h2 + _offsetY;
        case FN:  return x + w - w2 - _offsetX;
        case FW:  return x + h - h2 - _offsetY;
        case FS:  return x + w2 + _offsetX;
        case FE:  return x + h2 + _offsetY;
    }
}

double
Pin::y() const
{
    // done 4U
    double y  = (double)_node->_curCord._y;  
    double w  = (double)_node->_width;
    double w2 = (double)(_node->_width/2);
    double h  = (double)_node->_height;
    double h2 = (double)(_node->_height/2);
    switch(_node->_curOrt) {
        case N :  return y + h2 + _offsetX; 
        case W :  return y + w2 + _offsetX;
        case S :  return y + h - h2 - _offsetY;
        case E :  return y + w - w2 - _offsetX;
        case FN:  return y + h2 + _offsetY;
        case FW:  return y + w - w2 - _offsetX;
        case FS:  return y + h - h2 - _offsetY;
        case FE:  return y + w2 + _offsetX;
    }
}

/**Net Function*********************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

double
Net::HPWL() const
{
    // 4U done
    double xmax = DBL_MIN;
    double xmin = DBL_MAX;
    double ymax = DBL_MIN;
    double ymin = DBL_MAX;
    for(size_t i = 0, psize = _pinList.size(); i < psize; ++i) {
        Pin* p = _pinList[i];
        if(p->x() > xmax) xmax = p->x();
        if(p->x() < xmin) xmin = p->x();
        if(p->y() > ymax) ymax = p->y();
        if(p->y() < ymin) ymin = p->y();
    }
    for(size_t i = 0, tsize = _termList.size(); i < tsize; ++i) {
        Term* t = _termList[i];
        if(t->x() > xmax) xmax = t->x();
        if(t->x() < xmin) xmin = t->x();
        if(t->y() > ymax) ymax = t->y();
        if(t->y() < ymin) ymin = t->y();
    }
    return xmax - xmin + ymax - ymin;
}

/**Term Function********************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/



////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
