# INPORTANT: 
## ALWAYS DO `git pull` BEFORE STARTING YOUR WORK
### Something to be deternimed
- `class Coord`
  + use `int` or `double` to represent a coordinate

- simAnneal
  + considering *congestion* in cost function

### Data Structure
1. MPTreeMgr ( MPTreeMgr.h )
  - top manager
  - public member functions
    + readInput ( .nodes , .pl , .nets ) 
    + simAnneal
    + writeOutput ( .fp )
    + test ( test interface )
  - private data members
    + \_allNode(macros) 
    + \_allNet(nets) 
    + \_allTerm(terminals)
  - private helper functions 
    +   *simAnneal*
      - [ ] cost computation

2. Node ( Element.h ) 
  - macro class
  - public member functions 
    + [x] `centerX,Y()` --> return the center of the macro
  - private data members
    + \_name 
    + \_width, \_height
    + *Coordinates and Orientations*
      - \_initCord , \_initOrt ( initial positions from placer )
      - \_curCord , \_curOrt   ( current positions )
      - \_optCord , \_optOrt   ( optimal positions so far )
    + \_pinList ( pins on the macro )

3. Pin ( Element.h ) 
  - pin class ( connection points on a macro )
  - public member functions
    + [x] `x() , y()` --> get the positions of the pin ( use \_node and offset to compute )
  - private data members
    + Node\* \_node ( ptr to the macro )
    + \_offsetX,Y ( w.r.t. the center of the macro , orient = N )

4. Net ( Element.h ) 
  - net class
  - public member functions
    + [x] `computeHPWL()`
  - private data members
    + \_name
    + \_pinList
    + \_termList

5. Term ( Element.h ) 
 - terminal class
 - public member functions
   + x(), y() 
 - private data members
   + \_name
   + \_fixXY( fixed positions )

