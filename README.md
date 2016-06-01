# Data Structure
1. MPTreeMgr ( MPTreeMgr.h )
  - top manager
  - public member functions
    + readInput ( .nodes , .pl , .nets ) 
    + simAnneal
    + writeOutput ( .fp )
    + test ( test interface )
  - private data members
    + _allNode(macros) , _allNet(nets) , _allTerm(terminals)
  - private helper functions 
    > simAnneal
     * [ ] cost computation //TODO

2. Node ( Element.h ) 
  - macro class
  - public member functions 
    + [ ] 'centerX,Y()' --> return the center of the macro
  - private data members
    + _name , _width , _height
    + _initCord , _initOrt ( initial positions and orientaion from placer )
    + _pinList ( pins on the macro )

3. Pin ( Element.h ) 
  - pin class ( connection points on a macro )
  - public member functions
    + [ ] 'x() , y()' --> get the positions of the pin ( use _node and offset to compute )
  - private data members
    + Node * _node ( ptr to the macro )
    + offsetX,Y ( w.r.t. the center of the macro , orient = N )

4. Net ( Element.h ) 
  - net class
  - public member functions
    + [ ] 'computeHPWL()'
  - private data members
    + _name
    + _pinList
    + _termList

5. Term ( Element.h ) 
 - terminal class
 - public member functions
   + 
 - private data members
   + _name
   + _fixXY ( fixed positions ) 
