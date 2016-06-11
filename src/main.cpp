/**CFile****************************************************************

  FileName    [main.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    []

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016.]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <ctime>
#include <cstdlib>
#include "MPTreeMgr.h"
#include "utility.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [main function]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

int
main( int argc , char ** argv )
{
	MPTreeMgr pd16;
	if ( !pd16.readInput( argv[1] , argv[2] , argv[3] ) ) {
      cout << " >  reading input has failed\n";
		return 1;
	}
	srand( time(0) );
	pd16.test();
	pd16.simAnneal();
	pd16.writeOutput( argv[4] );
	return 0;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
