/**HFile****************************************************************

  FileName    [MPTreeMgr.h]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [MP tree mgr declaration.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

#ifndef __MPTMGR_H__
#define __MPTMGR_H__

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <ctime>
#include "utility.h"
#include "Element.h"

using namespace std;

////////////////////////////////////////////////////////////////////////
///                         DECLARATION                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                         PARAMETERS                               ///
////////////////////////////////////////////////////////////////////////

typedef vector<Node*>        NodeList;
typedef vector<Net*>         NetList;
typedef vector<Term*>        TermList;
typedef map<string , Node*>  NodeNameMap;
typedef map<string , Term*>  TermNameMap;

typedef vector<string> Token;

////////////////////////////////////////////////////////////////////////
///                         BASIC TYPES                              ///
////////////////////////////////////////////////////////////////////////

class MPTreeMgr
{
	public:
		MPTreeMgr();
		~MPTreeMgr();
		bool readInput     ( const char * , const char * , const char * ); // nodes , pl , nets
		void simAnneal     ();
		bool writeOutput   ( const char * ) const;
		void test();
	private:
		// member functions
		// destructor helper
		void     freeNode           ();
		void     freeNodePin        ( Node * );
		void     freeNet            ();
		void     freeTerm           ();
		//void     resetBstMgr         ();
		
		// readInput() helper
		bool     readNode           ( const char * );
		bool     readPosition       ( const char * );
		bool     readNet            ( const char * );
      void     buildInitMPTree    ();
		// readNode() helper
      void     readNodeHeader     ( ifstream& );
      void     readNodeList       ( ifstream& );
		bool     isNode             ( const Token& ) const;
		bool     isTerm             ( const Token& ) const;
      void     readOneNode        ( const Token& );
      void     readOneTerm        ( const Token& );
      // readPosition() helper
		void     readPosList        ( ifstream& );
		void     setNodeInitPos     ( Node * , const Token& );
		void     setTermPos         ( Term * , const Token& );
      // readNet() helper
      void     readNetHeader      ( ifstream& );
      void     readNetList        ( ifstream& );
      void     readOneNet         ( ifstream& , const Token& );
		void     setNetPin          ( Node* , Net* , const Token& );
		void     setNetTerm         ( Term* , Net* , const Token& );
		Pin*     nodeFindPin        ( Node* , double , double );
		// buildInitMPTree() helper
      void     initMPTreeRoot     ();
      bool     buildRegionTree    ( NodeList& );
      void     printMPTree        () const;
      void     printMPTree_rec    ( const Node* , int ) const;
      void     indent             ( int ) const;
      // contour operating helper
		/*
		void     initContour         ();
		void     deleteContour       ();
		Contour* getLastCtr          () const;
		int      updateContour       ( const Block* );
		void     printContour        () const;
		*/
		// cost computation helper
      
		void     initCost            ();
      double   computeCost         ();
      double   computeArea         ();
      double   computeWL           () const;
      double   computeDisp         () const;
      double   computeCongest      () const;
		void     updateOptSol        ();
		void     updateCurSol        ();
      //void     updateTempSolByOpt  ();
		
		
      // MP trees packing helper
		// XXX
      // remember to define class Contour!
      bool     packMPTree          ();
      bool     packMPTree_rec      ( Node* const & , List*& , const bool& , const bool& );
      bool     packcorner          ( Node* const & , List*& , const bool& , const bool& );
      
      // MP trees operation helper
      void     perturbMPTree       ( Node** , Node** , int* , int* , int );
      void     undoMPTree          ( Node** , Node** , int* , int* , int );
      // op1 : rotate
      void     rotateNode          ( Node** , int* );
      // op2 : delete and insert
      void     delInsNode          ( Node** , int* , Node** , int* );
      void     getDelInsPair       ( int& , int& ) const;
      void     delInsNode_int      ( Node* , int , Node* , int );
      // op3 : swap nodes
      void     swapNode            ( Node** , Node** );
      void     swapNode_int        ( Node*  , Node* );
      void     changeNbrPtr        ( Node*  , Node* );
      bool     pLRChild            ( const Node* ) const;
      bool     checkParentChild    ( const Node* , const Node* ) const;
      void     swapParentChild     ( Node* , Node* );
      bool     checkSibling        ( const Node* , const Node* ) const;
      void     swapSibling         ( Node* , Node* );
      // op4 : swap subtrees
      void     swapSubTree         ( int* , int* );
      void     swapSubTree_int     ( int  , int  );
      Node*    getSubTreeRoot      ( int );
		
		// simulated annealing helper	
      //  uncomment these function when needed
		//void     greedyRun           ();
		void     simAnneal_int       ();
	   //void     setSAPar            ();
	   void     setTemp             (double &, double &);
		//bool     simAnnealStop       ();
		
		// writeOutput() helper
		void     writeNodePosition   ( ostream& ) const;
		void     writeAllNode        ( ostream& ) const;
		void     writeAllTerm        ( ostream& ) const;
		void     writeAllNet         ( ostream& ) const;
		void     printNode           ( ostream& , Node* ) const;
		void     printNodePin        ( ostream& , Node* ) const;
		void     printNodePtr        ( ostream& , Node* ) const;
		void     printTerm           ( ostream& , Term* ) const;
		void     printNet            ( ostream& , Net*  ) const;
      void     printNetPin         ( ostream& , Net*  ) const;
      void     printNetTerm        ( ostream& , Net*  ) const;
		/*
		void     writeStat           ( ostream& ) const;
      void     writeNetList        ( ostream& ) const;
      void     writeCost           ( ostream& ) const;
      void     writeCoordinate     ( ostream& ) const;
		*/
		// test interface helper
		//void     printAllOpt         () const;      
		// data members
		NodeList           _allNode;
		NetList            _allNet;
		TermList           _allTerm;
		NodeNameMap        _nodeNameMap;
		TermNameMap        _termNameMap;
      NodeList           _treeRoot;
      int                _chipWidth;
      int                _chipHeight;

      // SA parameters
      double             _initArea;
      double             _initWL;
      double             _initDisp;
      double             _optCost;
};

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
