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
#include <cfloat>

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
	//TODO
	if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
	  return _curCord._x + _width/2;
	else
	  return _curCord._x + _height/2;
	return 0;
}

int
Node::centerY() const
{
	//TODO
	if(_curOrt == N || _curOrt == S || _curOrt == FN || _curOrt == FS)
	  return _curCord._y + _height/2;
	else
	  return _curCord._y + _width/2;
	return 0;
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
	//TODO
	double x = (double)_node->_curCord._x;
    double w = (double)_node->_width;
	double h = (double)_node->_height;
	switch(_node->_curOrt) {
        case N :  return x + _offsetX; 
        case W :  return x + h - _offsetY;
        case S :  return x + w - _offsetX;
        case E :  return x + _offsetY;
        case FN:  return x + w - _offsetX;
        case FW:  return x + h - _offsetY;
        case FS:  return x + _offsetX;
        case FE:  return x + _offsetY;
	}
}

double
Pin::y() const
{
	//TODO	
    double y = (double)_node->_curCord._y;  
    double w = (double)_node->_width;
	double h = (double)_node->_height;
  switch(_node->_curOrt) {
        case N :  return y + _offsetX; 
		case W :  return y + _offsetX;
		case S :  return y + h - _offsetY;
		case E :  return y + w - _offsetX;
		case FN:  return y +  _offsetY;
		case FW:  return y + w - _offsetX;
		case FS:  return y + h - _offsetY;
		case FE:  return y + _offsetX;
	}
}

/**Net Function*********************************************************

  Synopsis    []

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

double
Net::computeHPWL() const
{
	//TODO
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
