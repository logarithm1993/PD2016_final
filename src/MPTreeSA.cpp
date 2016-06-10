/**CFile****************************************************************

  FileName    [MPTreeSA.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [Simulated annealing methods.]

  Author      [OAO]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "MPTreeMgr.h"
#include <cmath>

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Simulated annealing (interface)]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::simAnneal()
{
   // compute initial MPTree;
   // compute initial cost func parameters
   
}

/**Function*************************************************************

  Synopsis    [cost computing function (main)]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
double
MPTreeMgr::computeCost()
{
   // TODO: adjust vlaue of alpha, beta, gamma
   double c1 = computeArea();
   double c2 = computeWL();
   double c3 = computeDisp();

   return c1 + c2 + c3;
}

/**Function*************************************************************

  Synopsis    [cost computing function (inner)]

  Description [
                 Area: chip outline area
                 WL  : HPWL
                 Disp: Node displacement
              ]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
double
MPTreeMgr::computeArea()
{
   return 0.0;
}

double
MPTreeMgr::computeWL() const
{
   double sum = 0.0;
   for(unsigned i = 0, n = _allNet.size(); i < n; ++i){
   }
   return sum / _initWL;
}

double
MPTreeMgr::computeDisp() const
{
   return 0.0;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
