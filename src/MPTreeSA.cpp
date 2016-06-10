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
                 Area: contour area(like Riemann sum)
                 WL  : HPWL
                 Disp: Node displacement
              ]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
double
MPTreeMgr::computeArea()
{
   double sum = 0.0;
   // TODO :  Riemann sum
   return sum / _initArea;
}

double
MPTreeMgr::computeWL() const
{
   double sum = 0.0;
   for(unsigned i = 0, n = _allNet.size(); i < n; ++i){
      sum += _allNet[i]->computeHPWL();
   }
   return sum / _initWL;
}

double
MPTreeMgr::computeDisp() const
{
   double sum = 0.0;
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i){
      sum += _allNode[i]->displacement();
   }
   return sum / _initDisp;
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
