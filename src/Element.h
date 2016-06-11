/**HFile****************************************************************

  FileName    [Element.h]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [Node/Pin/Net/Term declaration.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

#ifndef __ELEMENT_H__
#define __ELEMENT_H__

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>
#include "MPTreeMgr.h"
#include "utility.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
///                         DECLARATION                              ///
////////////////////////////////////////////////////////////////////////

class Node;
class Pin;
class Net;
class Term;
class Coord;
class TriPtr;
class ListNode;
class List;
////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

typedef vector<Node*>    NodeList;
typedef vector<Pin*>     PinList;
typedef vector<Net*>     NetList;
typedef vector<Term*>    TermList;
typedef enum Orient
{
	N  , W  , S  , E , 
	FN , FW , FS , FE  // flip
} Orient;

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

class Coord
{
	friend class MPTreeMgr;
	friend class Node;
	friend class Pin;
	friend class Net;
	friend class Term;
	public:
	   Coord() : _x(0) , _y(0) {}
		Coord( int x , int y ) : _x(x) , _y(y) {}
		Coord& operator=( const Coord & rhs ) { 
			_x = rhs._x; 
			_y = rhs._y;
		   return *this;
		}
	private:
		int _x;
		int _y;
};

class TriPtr
{
	friend class MPTreeMgr;
	friend class Node;
	friend class Term;
	friend class Net;
	public:
	   TriPtr() { _p = _left = _right = NULL; }
		TriPtr& operator=( const TriPtr & rhs ) { 
			_p     = rhs._p; 
			_left  = rhs._left;
			_right = rhs._right;
		   return *this;
		}
		void swapTriPtr( TriPtr & swap ) {
		   TriPtr temp = swap;
			swap        = *this;
			*this       = temp;
		}
		void resetPtr() { _p = _left = _right = NULL; }
	private:
		// data members
		Node * _p;
		Node * _left;
		Node * _right;
};

class Node
{
	friend class MPTreeMgr;
	friend class Pin;
	friend class Net;
	friend class Term;
	public:
		Node() : _name("") , _width(0) , _height(0) {};
		Node( string name , int width , int height ) :  _name(name) , _width(width) , _height(height) {}
		
      // set methods
      void   setParent ( Node * pNd ) { _curPtr._p = pNd; }
      void   setChild  ( Node * pNd , bool right ) { 
         if ( right ) _curPtr._right = pNd; 
         else         _curPtr._left  = pNd;
      }
      // get methods
      int    centerX()      const;
		int    centerY()      const;
		int    right()        const;
		int    top()          const;
		int    width()        const;
		int    height()       const;
      int    displacement() const;
      void   updateOpt();
      void   updateCur();
	private:
		string      _name;
		int         _width;
		int         _height;
		PinList     _pinList;
		Coord       _initCord; // from placer
		Coord       _curCord;
		Coord       _optCord;
		Orient      _initOrt;  // from placer
		Orient      _curOrt;
		Orient      _optOrt;
      // Tree structure
      TriPtr      _curPtr;
      TriPtr      _optPtr;
};

class Pin
{
	friend class MPTreeMgr;
	friend class Node;
	friend class Net;
	friend class Term;
	public:
		Pin() : _node(NULL) , _offsetX(0.0) , _offsetY(0.0) {}
		Pin( Node * node , double offsetX , double offsetY ) : _node(node) , _offsetX(offsetX) , _offsetY(offsetY) {}
		double x() const; // get x-coordinate
		double y() const; // get y-coordinate
	private:
		Node *    _node;
		double    _offsetX;
		double    _offsetY;
};

class Net
{
	friend class MPTreeMgr;
	friend class Node;
	friend class Pin;
	friend class Term;
   public:
		Net() : _name("") {};
		double HPWL() const;
	private:
		string       _name;
		PinList      _pinList;
		TermList     _termList;
};

class Term
{
	friend class MPTreeMgr;
	friend class Node;
	friend class Pin;
	friend class Net;
   public:
		Term() : _name("") {};
		int    x() const { return _fixXY._x; } 
		int    y() const { return _fixXY._y; }
	private:
		string       _name;
		Coord        _fixXY;
};

////////////////////////////////////////////////////////////////////////
///                      MACRO DEFINITIONS                           ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                    FUNCTION DECLARATIONS                         ///
////////////////////////////////////////////////////////////////////////

#endif

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
