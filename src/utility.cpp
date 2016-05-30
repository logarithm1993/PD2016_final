/**CFile****************************************************************

  FileName    [utility.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [utility functions]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 29, 2016.]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "utility.h"
#include "Element.h"
using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [parse tokens]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
Nz_ParseToken( const string & str , Token & token )
{
	string temp;
	size_t head , tail;
   
	head = tail = 0;
	while ( true ) {
	   head = str.find_first_not_of( ' ' , tail );
      if ( head == string::npos ) break;
		tail = str.find_first_of( ' ' , head );
		temp = str.substr( head , tail-head );
		token.push_back( temp );
	}
}

void
Nz_PrintToken( const Token & token )
{
	int i , n;

	cout << " > print token : " << endl;
	for ( i = 0 , n = token.size() ; i < n ; ++i )
		cout << token[i] << endl;
}

/**Function*************************************************************

  Synopsis    [string v.s. Orient conversion]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

Orient
Nz_Str2Orient( const string & str )
{
	if ( str == "N"  ) return N;
	if ( str == "W"  ) return W;
	if ( str == "S"  ) return S;
	if ( str == "E"  ) return E;
	if ( str == "FN" ) return FN;
	if ( str == "FW" ) return FW;
	if ( str == "FS" ) return FS;
	if ( str == "FE" ) return FE;
}

string
Nz_Orient2Str( const Orient & ort )
{
	if ( ort == N  )  return "N";
	if ( ort == W  )  return "W";
	if ( ort == S  )  return "S";
	if ( ort == E  )  return "E";
	if ( ort == FN )  return "FN";
	if ( ort == FW )  return "FW";
	if ( ort == FS )  return "FS";
	if ( ort == FE )  return "FE";
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
