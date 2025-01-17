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
#include <string>
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
   char input5[1024] = "adaptec0.fp";
   
   string mFileName = argv[4]; mFileName += ".matlab";
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
	pd16.simAnneal();
   pd16.writeMatlabOutput( mFileName.c_str() );
	pd16.writeOutput( argv[4] );
   cout << " >  MP trees macro packing finished." << endl;
	return 0;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
