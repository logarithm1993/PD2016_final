=========================
=    Data Structure     =
=========================
----------------------------------------------------------------
1. MPTreeMgr ( MPTreeMgr.h )
   a. top manager
	b. public member functions
	   (1) readInput ( .nodes , .pl , .nets )
		(2) simAnneal
		(3) writeOutput ( .fp )
		(4) test ( test interface )
	c. private data members
	   (1) _allNode(macros) , _allNet(nets) , _allTerm(terminals)
	d. private helper functions
	   (2) simAnneal
		   (a) cost computation //TODO

-----------------------------------------------------------------
2. Node ( Element.h ) 
   a. macro class
	b. public member functions
	   (1) centerX,Y() --> return the center of the macro //TODO 
	c. private data members
	   (1) _name , _width , _height
		(2) _initCord , _initOrt ( initial positions and orientaion from placer )
		(3) _pinList ( pins on the macro )

-----------------------------------------------------------------
3. Pin ( Element.h ) 
   a. pin class ( connection points on a macro )
	b. public member functions
	   (1) x() , y() --> get the positions of the pin ( use _node and offset to compute ) //TODO
	c. private data members
	   (1) Node * _node ( ptr to the macro )
		(2) offsetX,Y ( w.r.t. the center of the macro , orient = N )

-----------------------------------------------------------------
4. Net ( Element.h ) 
   a. net class
	b. public member functions
	   (1) computeHPWL() //TODO
	c. private data members
	   (1) _name
		(2) _pinList
		(3) _termList

-----------------------------------------------------------------
5. Term ( Element.h ) 
   a. terminal class
	b. public member functions
	   (1) 
	c. private data members
	   (1) _name
		(2) _fixXY ( fixed positions ) 
