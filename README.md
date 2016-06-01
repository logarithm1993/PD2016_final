# Data Structure
----------------------------------------------------------------
1. MPTreeMgr ( MPTreeMgr.h )
   - top manager
   - public member functions
      + readInput ( .nodes , .pl , .nets ) 
      + simAnneal
      + writeOutput ( .fp )
      + test ( test interface )
   - private data members
      (1) _allNode(macros) , _allNet(nets) , _allTerm(terminals)
   - private helper functions 
      (2) simAnneal
      (a) cost computation //TODO
-----------------------------------------------------------------
2. Node ( Element.h ) 
   - macro class
   - public member functions 
      (1) centerX,Y() --> return the center of the macro //TODO 
   - private data members
      (1) _name , _width , _height
      (2) _initCord , _initOrt ( initial positions and orientaion from placer )
      (3) _pinList ( pins on the macro )

-----------------------------------------------------------------
3. Pin ( Element.h ) 
   - pin class ( connection points on a macro )
   - public member functions
      (1) x() , y() --> get the positions of the pin ( use _node and offset to compute ) //TODO
   - private data members
      (1) Node * _node ( ptr to the macro )
      (2) offsetX,Y ( w.r.t. the center of the macro , orient = N )
-----------------------------------------------------------------
4. Net ( Element.h ) 
   - net class
   - public member functions
      (1) computeHPWL() //TODO
   - private data members
      (1) _name
      (2) _pinList
      (3) _termList
-----------------------------------------------------------------
5. Term ( Element.h ) 
   - terminal class
   - public member functions
     (1) 
   - private data members
     (1) _name
     (2) _fixXY ( fixed positions ) 
