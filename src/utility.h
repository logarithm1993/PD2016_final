/**HFile****************************************************************

  FileName    [utility.h]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [Utilities declaration : coordinate(Coord) , contour , ... .]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

#ifndef __UTIL_H__
#define __UTIL_H__

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <string>
#include <iostream>
#include "MPTreeMgr.h"
#include "Element.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

typedef vector<string> Token;

////////////////////////////////////////////////////////////////////////
///                         DECLARATION                              ///
////////////////////////////////////////////////////////////////////////

extern void    Nz_ParseToken     ( const string & , Token & );
extern void    Nz_PrintToken     ( const Token & );
extern Orient  Nz_Str2Orient     ( const string & );
extern string  Nz_Orient2Str     ( const Orient & );
extern Orient  Nz_Int2Orient     ( const int & );
extern int     Nz_Orient2Int     ( const Orient & );

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////
class ListNode{
friend class List;
friend class MPTreeMgr;
public:
   ListNode(){}
   ListNode(const int& x0,const int& x1,const int& y1) : _x0(x0), _x1(x1), _y1(y1), _next(0), _prev(0) {}

   friend ostream& operator << (ostream& ,const ListNode&);
   friend ostream& operator << (ostream& ,const List&);
private:
   int       _x0,_x1;  // x range
   int       _y1;      // height
   ListNode* _next;
   ListNode* _prev;
};

class List{
friend class MPTreeMgr;
public:
   List(const int& outwidth);
   ~List();
   
   int  update(const int&,const int&,const int&,const bool&);
   void concate(ListNode* const &,ListNode* const &);
   friend ostream& operator << (ostream& ,const List&);
   
private:
   ListNode* _begin;
   ListNode* _end;

};
/*
class Contour
{
	friend class BstMgr;
   public:
	   Contour();
	   Contour( int , int , int );
	private:
		// data members
		int        _left;
		int        _right;
		int        _height;
		Contour *  _prev;
		Contour *  _next;
};
*/

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
