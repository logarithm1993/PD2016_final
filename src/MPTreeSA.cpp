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
#include <cfloat>
#include <cstdlib>
////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////
int    chooseMove()
       { return rand() % 4; }
double prob()
       { return (double)rand() / RAND_MAX; }
bool   isAccepted(const double &C, const double &T)
       { return exp(-C/T) > prob(); }

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**function*************************************************************

  synopsis    [simulated annealing (interface)]

  description []
               
  sideeffects []

  seealso     []

***********************************************************************/

void
MPTreeMgr::simAnneal()
{
   // compute initial MPTree;
   // compute initial cost func parameters
   // do sa
   simAnneal_int();
}

/**function*************************************************************

  synopsis    [simulated annealing (Real)]

  description []
               
  sideeffects []

  seealso     []

***********************************************************************/
void
MPTreeMgr::simAnneal_int()
{
   double cost, costPrev, deltaCost;
   double T_start, T_end;
   
   setTemp(T_start, T_end);
   cost = computeCost();

   unsigned repeat = 20;
   double   T      = T_start;
   double   r      = 0.97;
   while (T > T_end){
      for(unsigned i = 0; i < repeat; ++i){
         Node* obj1 = NULL;
         Node* obj2 = NULL;
         int   arg1 = -1;
         int   arg2 = -1;
         int   move = chooseMove();
         perturbMPTree( &obj1, &obj2, &arg1, &arg2, move );
         while (!packMPTree()){
            undoMPTree( &obj1, &obj2, &arg1, &arg2, move );
            obj1 = obj2 = NULL;
            arg1 = arg2 = -1;
            move = chooseMove(); // TBD: choose another?
            perturbMPTree( &obj1, &obj2, &arg1, &arg2, move );
         }
         costPrev = cost;
         cost = computeCost();
         deltaCost = cost - costPrev;
         if (deltaCost < 0){
            if(cost < _optCost){
               _optCost = cost;
               updateOptSol();
            }
            continue;
         }
         else if(isAccepted(deltaCost, T))
            continue;
         else
            undoMPTree( &obj1, &obj2, &arg1, &arg2, move );
      }
      T *= r;
   }
   // restore opt-info
   updateCurSol();
}

/**Function*************************************************************

  Synopsis    [initialize SA parameters]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void
MPTreeMgr::initCost()
{
  _initWL   = 0.0; 
  _initArea = 0.0;
  _initDisp = 0.0;
  _optCost  = DBL_MAX;
  // computeWL
  for(unsigned i = 0, n = _allNet.size(); i < n; ++i)
     _initWL += _allNet[i]->HPWL();
  // computeArea TODO
  
  // computeDisp
  for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
     _initDisp += _allNode[i]->displacement();
}

void
MPTreeMgr::setTemp(double & T0, double & Tx)
{
   // TODO
   T0 = 9999.999;
   Tx = 0.0001;
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
      sum += _allNet[i]->HPWL();
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

/**Function*************************************************************

  Synopsis    [update Node info]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
void
MPTreeMgr::updateOptSol()
{
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
      _allNode[i]->updateOpt();
}

void
MPTreeMgr::updateCurSol()
{
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
      _allNode[i]->updateCur();
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
