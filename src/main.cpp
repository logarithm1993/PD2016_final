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
   char input1[1024] = "benchmark/adaptec0_MMS/adaptec0.nodes";
   char input2[1024] = "benchmark/adaptec0_MMS/adaptec0.ntup.pl";
   char input3[1024] = "benchmark/adaptec0_MMS/adaptec0.nets";
   char input4[1024] = "adaptec2.fp.matlab";
   char input5[1024] = "adaptec0.fp";

	MPTreeMgr pd16;
#if 0
	if ( !pd16.readInput( input1 , input2 , input3 ) ) {
      cout << " >  reading input has failed\n";
		return 1;
	}
#else
	if ( !pd16.readInput( argv[1] , argv[2] , argv[3] ) ) {
      cout << " >  reading input has failed\n";
		return 1;
	}
#endif
	srand( time(0) );
	//pd16.test();
	pd16.simAnneal();
   //cout << " >  End SA" << endl;
   //cout.flush();
	//pd16.test();
   pd16.writeMatlabOutput( input4 );
	pd16.writeOutput( argv[4] );
	//pd16.writeOutput( input5 );
   cout << " >  MP trees macro packing finished." << endl;
	return 0;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
