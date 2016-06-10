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

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

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
