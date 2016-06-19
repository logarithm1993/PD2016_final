/**CFile****************************************************************

  FileName    [MPTreeSA.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [Simulated annealing methods.]

  Author      [OAO]
  
  Affiliation [NTU]

  Date        [Jun 11, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////
#include "MPTreeMgr.h"
#include <cmath>
#include <cassert>
#include <cfloat>
#include <cstdlib>
#include <algorithm>

//#define ROTATE

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////
static inline int    chooseMove(double a = 0.0)
                     {  
                     #ifndef ROTATE
                        double p = (double)rand()/RAND_MAX;
                        if (a < 0.3){
                           if (p < 0.50)        return 1;
                           else                 return 2;
                        }else{
                                if (p < 0.40)   return 1;
                           else if (p < 0.80)   return 2;
                           else                 return 3;
                        }
                     #else
                        double p = (double)rand()/RAND_MAX;
                        if (a < 0.3){
                           if      (p < 0.20)   return 0;
                           else if (p < 0.60)   return 1;
                           else                 return 2;
                        }else{
                           if      (p < 0.20)   return 0;
                           else if (p < 0.60)   return 1;
                           else if (p < 0.80)   return 2;
                           else                 return 3;
                        }
                     #endif
                     }
static inline double prob()
                     { return (double)rand() / RAND_MAX; }
static inline bool   isAccepted(const double &C, const double &T)
                     { 
                        //printf("C = %f, T = %f, prob = %f\n", C, T, exp(-C/T));
                        return exp(-C/T) > prob(); 
                     }

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
   // TBD: sa_schedule
   cout << "simAnneal(): start\n";
   for(unsigned i = 0; i < 3; ++i){
      cout << i+1 << "th attempt!\n\n";
      simAnneal_int();
   }
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
   double tmpCost, costPrev, deltaCost;
   double T_start, T_end;
  
   setTemp(T_start, T_end);
   assert(packMPTree());
   _currCost = computeCost();
   
   // TBD
   unsigned repeat = 200;
   double   T      = T_start;
   double   r      = 0.98;
  
   unsigned uphillCnt   = 0;
   unsigned downhillCnt = 0;
   unsigned totalCnt    = 0;
   unsigned localCnt    = 0;
   cout <<"simAnneal_int(): start to SA\n";
   while (T > T_end){
      double progress = log(T_start/T)/log(T_start/T_end);
      for(unsigned i = 0; i < repeat; ++i){
         Node* obj1 = NULL;
         Node* obj2 = NULL;
         int   arg1 = -1;
         int   arg2 = -1;
         int   move = chooseMove( progress );
         perturbMPTree( &obj1, &obj2, &arg1, &arg2, move );
         while (!packMPTree()){
            //cout << "<perturb> packing failed, perturb again!\n";
            undoMPTree( &obj1, &obj2, &arg1, &arg2, move );
            obj1 = obj2 = NULL;
            arg1 = arg2 = -1;
            move = chooseMove( progress ); // TBD: choose another?
            perturbMPTree( &obj1, &obj2, &arg1, &arg2, move );
         }
         
         
         costPrev  = _currCost;
         tmpCost   = computeCost();
         deltaCost = tmpCost - costPrev;
         #if 0
         cout<<"current #:" << ++totalCnt <<endl;
         #else
         ++totalCnt;
         #endif
         if ( tmpCost < costPrev ){
            if( tmpCost < _optCost){
               _optCost = tmpCost;
               updateOptSol();
            }
            ++downhillCnt;
            localCnt  = 0;
            //cout << "delta < 0: " << cost << endl;
            //assert(cost < _currCost);
            _currCost = tmpCost;
            continue;
         }
         else if( tmpCost > costPrev && localCnt > 3 && isAccepted(deltaCost, T)){
            //cout << ">> uphillMove : current Temp("<< T <<")\tTMPcost: " << tmpCost << "\toptCost: " << _optCost << "\n";
            ++uphillCnt;
            _currCost = tmpCost;
            localCnt = 0;
            continue;
         }
         else{
            undoMPTree( &obj1, &obj2, &arg1, &arg2, move );
            ++localCnt;
            //cout << "UNDO: " << cost << endl;
         }
      }
      T *= r;
      cout <<"weather report: current Temp = " << T << "  Cost = " << _currCost << " [sa progress... "<< 100*progress << "%]"<<endl;
   }
   // restore opt-info
   updateCurSol();

   // TODO: output some message?
   cout<<"simAnneal_int(): done, _optCost = " << _optCost <<endl;
   cout<<"> total perturbation: "<< totalCnt <<" uphill: "<< uphillCnt <<" downhill: "<< downhillCnt << endl;
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
   // computeContour
   _initArea = _cntrArea; 
   // computeDisp
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
     _initDisp += _allNode[i]->displacement();
   // computeBalance
   double max = _BLArea;
   double min = _BLArea;
   if( _BRArea > max ) max = _BRArea;
   if( _BRArea < min ) min = _BRArea;
   if( _TLArea > max ) max = _TLArea;
   if( _TLArea < min ) min = _TLArea;
   if( _TRArea > max ) max = _TRArea;
   if( _TRArea < min ) min = _TRArea;
   _initBalance = max - min;

   _currCost = _optCost = computeCost();
   updateOptSol();
}

void
MPTreeMgr::setTemp(double & T0, double & Tx)
{
   // TBD
   double initAcceptRate  = 0.950;
   double finalAcceptRate = 0.005;
   unsigned repeat        = 1000;
   
   cout <<"setTemp(): start greedy search\n";
   vector<double> vCost;
   vCost.reserve(repeat);
   double cost, costPrev, deltaCost;
   cost = computeCost();
   // greedy approach
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
         if (deltaCost <= 0){
            if(cost < _optCost){
               _optCost = cost;
               updateOptSol();
            }
            _currCost = cost;
         }
         else{
            undoMPTree( &obj1, &obj2, &arg1, &arg2, move );
            vCost.push_back(deltaCost);
         }
   }
   updateCurSol();   
   // compute average delta cost, small/large deltas are discarded
   sort(vCost.begin(),vCost.end());
   double avg = 0;
   for(unsigned i = vCost.size()*0.2, n = vCost.size()*0.8; i < n; ++i){
      avg += vCost[i];
   }
   avg /= (int)vCost.size()*0.6;
   
   T0 = abs( avg / log(initAcceptRate) );
   Tx = abs( avg / log(finalAcceptRate));
   
   printf("avgDcost:%f T0:%f, Tx:%f\n", avg ,T0,Tx);
}

/**Function*************************************************************

  Synopsis    [cost computing function (main)]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
double
MPTreeMgr::computeCost() const
{
   // TODO: adjust vlaue of abcde
   double a = 6;
   double b = 3;
   double c = 3;
   double d = 6;
   double e = 2;

   double c1 = a/(a+b+c+d+e) * computeContour();
   double c2 = b/(a+b+c+d+e) * computeWL();
   double c3 = c/(a+b+c+d+e) * computeDisp();
   double c4 = d/(a+b+c+d+e) * computeBalance();
   double c5 = e/(a+b+c+d+e) * computeArea();
   //cout << " >  computeCost() : current cost = " << c1+c2+c3+c4 << endl;
   /*
   printf(" > computeCost() : area = %f, WL=%f, disp=%f, total=%f\n", 
         c1/a*(a+b+c+d), c2/b*(a+b+c+d), c3/c*(a+b+c+d), c1+c2+c3);
   */
   return c1 + c2 + c3 + c4 + c5;
}

/**Function*************************************************************

  Synopsis    [cost computing function (inner)]

  Description [
                 Area   : chip W x H
                 Contour: contour area(like Riemann sum)
                 WL     : HPWL
                 Disp   : Node displacement
              ]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
double
MPTreeMgr::computeContour() const
{
   return _cntrArea / _initArea;
}

double
MPTreeMgr::computeWL() const
{
   double sum = 0.0;
   for(unsigned i = 0, n = _allNet.size(); i < n; ++i)
      sum += _allNet[i]->HPWL();
   return sum / _initWL;
}

double
MPTreeMgr::computeDisp() const
{
   double sum = 0.0;
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
      sum += _allNode[i]->displacement();
   return sum / _initDisp;
}

double
MPTreeMgr::computeBalance() const
{
   // (maxSubTreeArea - minSubTreeArea) / initBalance
   double max = _BLArea;
   double min = _BLArea;
   if( _BRArea > max ) max = _BRArea;
   if( _BRArea < min ) min = _BRArea;
   if( _TLArea > max ) max = _TLArea;
   if( _TLArea < min ) min = _TLArea;
   if( _TRArea > max ) max = _TRArea;
   if( _TRArea < min ) min = _TRArea;
   return (max-min) / _initBalance;
}

double
MPTreeMgr::computeArea() const
{
   double r = (double)_chipWidth/_initChipWidth;
   return r*r;
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
   cout << "updateOptSol() : find better sol!\n";
   cout << ">> cost = " << _optCost << endl;
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
      _allNode[i]->updateOpt();
   for ( unsigned i = 0 , n = _treeRoot.size() ; i < n ; ++i )
      _treeRoot[i]->updateOpt();
   _optChipWidth  = _chipWidth;
   _optChipHeight = _chipHeight;
}

void
MPTreeMgr::updateCurSol()
{
   for(unsigned i = 0, n = _allNode.size(); i < n; ++i)
      _allNode[i]->updateCur();
   for ( unsigned i = 0 , n = _treeRoot.size() ; i < n ; ++i )
      _treeRoot[i]->updateCur();
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
