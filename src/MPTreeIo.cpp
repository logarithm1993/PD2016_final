/**CFile****************************************************************

  FileName    [MPTreeIo.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [I/O methods.]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 28, 2016]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <cassert>
#include "MPTreeMgr.h"
#include "utility.h"
#include "Element.h"

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [read input data : nodes , pl(positions) , nets ]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

bool
MPTreeMgr::readInput( const char * node , const char * pl , const char * net )
{
	cout << " >  readInput() : start reading input data\n";
	if ( !readNode     ( node )   )      return false;
	if ( !readPosition ( pl   )   )      return false;
	if ( !readNet      ( net  )   )      return false;
   buildInitMPTree();
   // packingMPTree();
   // initCost();
	return true;
}

/**Function*************************************************************

  Synopsis    [read node list]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

bool
MPTreeMgr::readNode( const char * file )
{
	cout << " >  readNode() : reading node file " << file << endl;
   ifstream input( file );

	if ( !input.is_open() ) {
	   cout << "[Error] readNode() : opening input file " << file << " has failed\n";
		return false;
	}

	readNodeHeader  ( input );
	readNodeList    ( input );
	
	input.close();
	cout << " >  readNode() : reading node success!\n";
	return true;
}

void
MPTreeMgr::readNodeHeader( ifstream & input )
{
   string temp;
	int numMacro;
	Token token;
   
	token.reserve( 8 );
	while ( true ) {
	   getline( input , temp );
		if ( !token.empty() ) token.clear();
		Nz_ParseToken( temp , token );
		if ( token.empty() ) continue;
		if ( token[0] == "NumTerminals" ) break;
	}
   numMacro = atoi( token[2].c_str() );
	cout << " >  Number of macros = " << numMacro << endl;
}

void
MPTreeMgr::readNodeList( ifstream & input )
{
	string temp;
	Token token;

	token.reserve( 8 );
	while ( getline( input , temp ) ) {
		if ( !token.empty() ) token.clear();
		Nz_ParseToken( temp , token );
		if ( token.empty() ) continue;
		if ( isNode( token ) ) readOneNode( token );
		if ( isTerm( token ) ) readOneTerm( token );
	}
}

bool
MPTreeMgr::isNode( const Token & token ) const
{
	return ( token.size() == 4 && token[3] == "terminal" );
}

bool
MPTreeMgr::isTerm( const Token & token ) const
{
	return ( token.size() == 3 && token[1] == "0" && token[2] == "0" );
}

void
MPTreeMgr::readOneNode( const Token & token )
{
	Node * pNode;
	NodeNameMap::iterator iNode;
   
	iNode = _nodeNameMap.find( token[0] );
   if ( iNode != _nodeNameMap.end() ) {
	   cout << "[Error] readOneNode() : node " << token[0] 
			  << " is defined twice!\n";
		assert(0);
	}
	pNode = new Node;
	pNode->_name    = token[0];
	pNode->_width   = atoi( token[1].c_str() );
	pNode->_height  = atoi( token[2].c_str() );
	_allNode.push_back( pNode );
	_nodeNameMap[pNode->_name] = pNode;
}

void
MPTreeMgr::readOneTerm( const Token & token )
{
	Term * pTerm;
	TermNameMap::iterator iTerm;
   
	iTerm = _termNameMap.find( token[0] );
   if ( iTerm != _termNameMap.end() ) {
	   cout << "[Error] readOneTerm() : terminal " << token[0] 
			  << " is defined twice!\n";
		assert(0);
	}
	pTerm = new Term;
	pTerm->_name = token[0];
	_allTerm.push_back( pTerm );
	_termNameMap[pTerm->_name] = pTerm;
}

/**Function*************************************************************

  Synopsis    [read positions]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

bool
MPTreeMgr::readPosition( const char * file )
{
	cout << " >  readPosition() : reading position file " << file << endl;
   ifstream input( file );

	if ( !input.is_open() ) {
	   cout << "[Error] readPosition() : opening input file " << file << " has failed\n";
		return false;
	}

	readPosList    ( input );
	
	input.close();
	cout << " >  readPosition() : reading position success!\n";
	cout << " >  initial chip boundary : \n";
   cout << " >    Width  = " << _chipWidth  << endl;
   cout << " >    Height = " << _chipHeight << endl;
	return true;
}

void
MPTreeMgr::readPosList( ifstream & input )
{
   string temp;
	Token token;
	NodeNameMap::iterator iNode;
	TermNameMap::iterator iTerm;
   Node * pNode;
	Term * pTerm;
   
	token.reserve( 8 );
	while ( getline( input , temp ) ) {
		if ( !token.empty() ) token.clear();
		Nz_ParseToken( temp , token );
		if ( token.empty() ) continue;
		iNode = _nodeNameMap.find( token[0] );
		if ( iNode != _nodeNameMap.end() ) {
		   pNode = iNode->second;
			setNodeInitPos( pNode , token );
			continue;
		}
		iTerm = _termNameMap.find( token[0] );
		if ( iTerm != _termNameMap.end() ) {
			pTerm = iTerm->second;
			setTermPos( pTerm , token );
			continue;
		}
	}
}

void
MPTreeMgr::setNodeInitPos( Node * pNode , const Token & token )
{
	if ( token.size() < 5 ) {
	   cout << "[Error] node " << pNode->_name << " lacks of position info\n";
		assert(0);
	}
	pNode->_curCord._x = pNode->_initCord._x = atoi( token[1].c_str() );
	pNode->_curCord._y = pNode->_initCord._y = atoi( token[2].c_str() );
	pNode->_curOrt     = pNode->_initOrt     = Nz_Str2Orient( token[4] );

   if ( pNode->_initCord._x > _chipWidth  ) _chipWidth  = pNode->_initCord._x;
   if ( pNode->_initCord._y > _chipHeight ) _chipHeight = pNode->_initCord._y;
}

void
MPTreeMgr::setTermPos( Term * pTerm , const Token & token )
{
	if ( token.size() < 5 ) {
	   cout << "[Error] terminal " << pTerm->_name << " lacks of position info\n";
		assert(0);
	}
	pTerm->_fixXY._x = atoi( token[1].c_str() );
	pTerm->_fixXY._y = atoi( token[2].c_str() );
   
   if ( pTerm->_fixXY._x > _chipWidth  ) _chipWidth  = pTerm->_fixXY._x;
   if ( pTerm->_fixXY._y > _chipHeight ) _chipHeight = pTerm->_fixXY._y;
}

/**Function*************************************************************

  Synopsis    [read net list]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

bool
MPTreeMgr::readNet( const char * file )
{
	cout << " >  readNet() : reading net file " << file << endl;
   ifstream input( file );

	if ( !input.is_open() ) {
	   cout << "[Error] readNet() : opening input file " << file << " has failed\n";
		return false;
	}

	readNetHeader    ( input );
	readNetList      ( input );

	input.close();
	cout << " >  readNet() : reading net success!\n";
	return true;
}

void
MPTreeMgr::readNetHeader( ifstream & input )
{
   string temp;
	Token token;
   
	token.reserve( 8 );
	while ( true ) {
	   getline( input , temp );
		if ( !token.empty() ) token.clear();
		Nz_ParseToken( temp , token );
		if ( token.empty() ) continue;
		if ( token[0] == "NumPins" ) break;
	}
}

void
MPTreeMgr::readNetList( ifstream & input )
{
	string temp;
	Token  token;

	while ( getline( input , temp ) ) {
	   Nz_ParseToken( temp , token );
		if ( token.empty() ) continue;
		if ( token[0] == "NetDegree" ) {
		   readOneNet( input , token );
			token.clear();
			continue;
		}
		token.clear();
	}
}

void
MPTreeMgr::readOneNet( ifstream & input , const Token & netToken )
{
	string temp;
	Token token;
	Net  * pNet;
	Term * pTerm;
	Node * pNode;
	NodeNameMap::iterator iNode;
	TermNameMap::iterator iTerm;
	int netDeg , numPin , i;
   
	netDeg = atoi( netToken[2].c_str() );
	numPin = 0;
	pNet   = new Net;
	pNet->_name = netToken[3];

	for ( i = 0 ; i < netDeg ; ++i ) {
	   getline( input , temp );
		if ( !token.empty() ) token.clear();
		Nz_ParseToken( temp , token );
		iNode = _nodeNameMap.find( token[0] );
		if ( iNode != _nodeNameMap.end() ) {
			pNode = iNode->second;
			setNetPin( pNode , pNet , token );
			++numPin;
			continue;
		}
		iTerm = _termNameMap.find( token[0] );
		if ( iTerm != _termNameMap.end() ) {
		   pTerm = iTerm->second;
			setNetTerm( pTerm , pNet , token );
			++numPin;
			continue;
		}
	}
   if ( numPin != 0 ) _allNet.push_back( pNet );
	else delete pNet;
}

void
MPTreeMgr::setNetPin( Node * pNode , Net * pNet , const Token & token )
{
	Pin * pPin;
	double offsetX , offsetY;

	offsetX = atof( token[3].c_str() );
	offsetY = atof( token[4].c_str() );
	pPin    = nodeFindPin( pNode , offsetX , offsetY );
	if ( !pPin ) {
	   pPin = new Pin( pNode , offsetX , offsetY );
		pNode->_pinList.push_back( pPin );
	}
	pNet->_pinList.push_back( pPin );
}

void
MPTreeMgr::setNetTerm( Term * pTerm , Net * pNet , const Token & token )
{
	pNet->_termList.push_back( pTerm );
}

Pin*
MPTreeMgr::nodeFindPin( Node * pNode , double offsetX , double offsetY )
{
	Pin * pPin;
	int i , n;

	for ( i = 0 , n = pNode->_pinList.size() ; i < n ; ++i ) {
	   pPin = pNode->_pinList[i];
		if ( pPin->_offsetX == offsetX && pPin->_offsetY == offsetY )
			return pPin;
	}
	return NULL;
}

/**Function*************************************************************

  Synopsis    [write output file (macro positions)]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

bool
MPTreeMgr::writeOutput( const char * file ) const
{
	cout << " >  writeOutput() : opening output file " << file << endl;
   ofstream out( file );

	if ( !out.is_open() ) {
	   cout << "[Error] writeOutput() : opening output file " << file << " has failed\n";
		return false;
	}
	writeNodePosition( out );
	out.close();
	return true;
}

void
MPTreeMgr::writeNodePosition( ostream & out ) const
{
	int i , n;
	Node * pNode;

	for ( i = 0 , n = _allNode.size() ; i < n ; ++i ) {
	   pNode = _allNode[i];
		out << pNode->_name << "  " 
			 //<< pNode->_optCord._x << "  "
			 //<< pNode->_optCord._y << "  : "
			 //<< Nz_Orient2Str( pNode->_optOrt ) << " /FIXED"
			 << pNode->_initCord._x << "  "
			 << pNode->_initCord._y << "  : "
			 << Nz_Orient2Str( pNode->_initOrt ) << " /FIXED"
			 << endl;
	}
}

void
MPTreeMgr::writeAllNode( ostream & out ) const
{
	int i , n;
	out << "Print all nodes:\n";
   for ( i = 0 , n = _allNode.size() ; i < n ; ++i ) {
		out << " >  i = " << i << endl;
		printNode( out , _allNode[i] );
	}
}

void
MPTreeMgr::printNode( ostream & out , Node * pNode ) const
{
   out << "   >  Name   = "  << pNode->_name << " ;";
   printNodePtr( out , pNode );
		 //<< " width = "  << pNode->_width  << " ;"
		 //<< " height = " << pNode->_height << " ;"
		 //<< " init x = " << pNode->_initCord._x << " ;"
		 //<< " init y = " << pNode->_initCord._y << endl;
	
	//printNodePin( out , pNode );
}

void
MPTreeMgr::printNodePin( ostream & out , Node * pNode ) const
{
	Pin * pPin;
	int i , n;
	for ( i = 0 , n = pNode->_pinList.size() ; i < n ; ++i ) {
	   pPin = pNode->_pinList[i];
		out << "     >  pin " << i << " :"
			 << " offsetX = " << pPin->_offsetX << " ;"
			 << " offsetY = " << pPin->_offsetY << endl;
	}
}

void
MPTreeMgr::printNodePtr( ostream & out , Node * pNode ) const
{
   Node * p , * left , * right;
   p     = pNode->_curPtr._p;
   left  = pNode->_curPtr._left;
   right = pNode->_curPtr._right;

   out << " parent : " << ( p     ? p->_name     : "none" ) << " ;"
       << " left   : " << ( left  ? left->_name  : "none" ) << " ;"
       << " right  : " << ( right ? right->_name : "none" ) << endl;
}

void
MPTreeMgr::writeAllTerm( ostream & out ) const
{
	int i , n;
	out << "Print all terminals:\n";
   for ( i = 0 , n = _allTerm.size() ; i < n ; ++i ) {
		out << " >  i = " << i << endl;
		printTerm( out , _allTerm[i] );
	}
}

void
MPTreeMgr::printTerm( ostream & out , Term * pTerm ) const
{
   out << "   >  Name = " << pTerm->_name << " ;"
		 << " x = " << pTerm->x() << " ;"
		 << " y = " << pTerm->y() << endl;
}

void
MPTreeMgr::writeAllNet( ostream & out ) const
{
	int i , n ;
	out << "Print all nets:\n";
	for ( i = 0 , n = _allNet.size() ; i < n ; ++i ) {
		out << " >  i = " << i << endl;
		printNet( out , _allNet[i] );
	}
}

void
MPTreeMgr::printNet( ostream & out , Net * pNet ) const
{
   out << "   >  Name = " << pNet->_name << endl;	
	printNetPin  ( out , pNet );
	printNetTerm ( out , pNet );
}

void
MPTreeMgr::printNetPin( ostream & out , Net * pNet ) const
{
	Pin * pPin;
	int i , n;
   
	for ( i = 0 , n = pNet->_pinList.size() ; i < n ; ++i ) {
	   pPin = pNet->_pinList[i];
		out << "     >  pin "  << i << ": Node = " << pPin->_node->_name << " ;"
			 << " offsetX = " << pPin->_offsetX << " ;"
			 << " offsetY = " << pPin->_offsetY << endl;
	}
}

void
MPTreeMgr::printNetTerm( ostream & out , Net * pNet ) const
{
	Term * pTerm;
	int i , n;
   
	for ( i = 0 , n = pNet->_termList.size() ; i < n ; ++i ) {
	   pTerm = pNet->_termList[i];
		out << "     >  terminal "  << i << ": Term = " << pTerm->_name << " ;"
			 << " x = " << pTerm->x() << " ;"
			 << " y = " << pTerm->y() << endl;
	}
}

/**Function*************************************************************

  Synopsis    [build initial MP trees based on initial positions]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::buildInitMPTree()
{
   vector<NodeList> node(4); // 0: BL; 1: BR; 2: TL;3: TR
   Node * pNode;
   int centerX , centerY , nodeX , nodeY , i , n;

   centerX = _chipWidth  / 2;
   centerY = _chipHeight / 2;
   
   for ( i = 0 , n = _allNode.size() ; i < n ; ++i ) {
      pNode = _allNode[i];
      nodeX = pNode->centerX();
      nodeY = pNode->centerY();
      if ( nodeX <= centerX ) { // left
         if ( nodeY <= centerY ) // BL
            node[0].push_back( pNode );
         else // TL
            node[2].push_back( pNode );
      }
      else { // right
         if ( nodeY <= centerY ) // BR
            node[1].push_back( pNode );
         else // TR
            node[3].push_back( pNode );
      }
   }
   
   initMPTreeRoot();

   for ( i = 0 ; i < 4 ; ++i ) {
      if ( !buildRegionTree( node[i] ) ) {
         cout << "[Warning] Empty region! i =  " << i << endl;
         continue;
      }
      if ( i == 3 ) {
         node[i][0]->_curPtr._p = _treeRoot[i-1];
         _treeRoot[i-1]->_curPtr._right = node[i][0];
      }
      else {
         node[i][0]->_curPtr._p = _treeRoot[i];
         _treeRoot[i]->_curPtr._left = node[i][0];
      }
   }
}

void
MPTreeMgr::initMPTreeRoot()
{
   Node * pNode;
   int i , n;

   for ( i = 0 ; i < 3 ; ++i ) _treeRoot.push_back( new Node );
   _treeRoot[0]->_name = "root-0";
   _treeRoot[1]->_name = "root-1";
   _treeRoot[2]->_name = "root-2";
   for ( i = 0 , n = _treeRoot.size()-1 ; i < n ; ++i ) {
      _treeRoot[i]->_curPtr._right = _treeRoot[i+1];
      _treeRoot[i+1]->_curPtr._p   = _treeRoot[i];
   }
}

bool
MPTreeMgr::buildRegionTree( NodeList & node )
{
   if ( node.empty() ) return false;

   Node * pNode , * pPrev , * pBottom;
   int bound , curX , i , n;
   
   pBottom = pPrev = node[0];
   bound   = _chipWidth / 2;
   curX    = node[0]->width();

   for ( i = 1 , n = node.size() ; i < n ; ++i ) {
      pNode = node[i];
      if ( curX >= bound ) {
         pBottom->_curPtr._right = pNode;
         pNode->_curPtr._p       = pBottom;
         pBottom                 = pNode;
         curX                    = pNode->width();
      }
      else {
         pPrev->_curPtr._left = pNode;
         pNode->_curPtr._p    = pPrev;
         curX += pNode->width();
      }
      pPrev = pNode;
   }
   return true;
}

/**Function*************************************************************

  Synopsis    [print MP tree]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
MPTreeMgr::printMPTree() const
{
   cout << " >  printMPTree():\n\n";
	printMPTree_rec( _treeRoot[0] , 0 );
}

void
MPTreeMgr::printMPTree_rec( const Node * pNode , int d ) const
{
	indent(d);
	cout << pNode->_name << endl;
	if ( (pNode->_curPtr)._left ) {
	   indent(d);
	   cout << "Left child:\n";
		printMPTree_rec( (pNode->_curPtr)._left , d+1 );
	}
	if ( (pNode->_curPtr)._right ) {
	   indent(d);
	   cout << "Right child:\n";
		printMPTree_rec( (pNode->_curPtr)._right , d+1 );
	}
}

void
MPTreeMgr::indent( int d ) const
{
	int i;
	for ( i = 0 ; i < d ; ++i ) cout << "    ";
}

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
