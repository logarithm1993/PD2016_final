/**CFile****************************************************************

  FileName    [utility.cpp]

  SystemName  [PD final project : MP trees macro placement]

  PackageName []

  Synopsis    [utility functions]

  Author      [Nian-Ze Lee]
  
  Affiliation [NTU]

  Date        [May 29, 2016.]

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <climits>
#include <cassert>
#include "utility.h"
#include "Element.h"
using namespace std;

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [parse tokens]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

void
Nz_ParseToken( const string & str , Token & token )
{
	string temp;
	size_t head , tail;
   
	head = tail = 0;
	while ( true ) {
	   head = str.find_first_not_of( ' ' , tail );
      if ( head == string::npos ) break;
		tail = str.find_first_of( ' ' , head );
		temp = str.substr( head , tail-head );
		token.push_back( temp );
	}
}

void
Nz_PrintToken( const Token & token )
{
	int i , n;

	cout << " > print token : " << endl;
	for ( i = 0 , n = token.size() ; i < n ; ++i )
		cout << token[i] << endl;
}

/**Function*************************************************************

  Synopsis    [string v.s. Orient conversion]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

Orient
Nz_Str2Orient( const string & str )
{
	if ( str == "N"  ) return N;
	if ( str == "W"  ) return W;
	if ( str == "S"  ) return S;
	if ( str == "E"  ) return E;
	if ( str == "FN" ) return FN;
	if ( str == "FW" ) return FW;
	if ( str == "FS" ) return FS;
	if ( str == "FE" ) return FE;
}

string
Nz_Orient2Str( const Orient & ort )
{
	if ( ort == N  )  return "N";
	if ( ort == W  )  return "W";
	if ( ort == S  )  return "S";
	if ( ort == E  )  return "E";
	if ( ort == FN )  return "FN";
	if ( ort == FW )  return "FW";
	if ( ort == FS )  return "FS";
	if ( ort == FE )  return "FE";
}

/**Function*************************************************************

  Synopsis    [int v.s. Orient conversion]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

Orient
Nz_Int2Orient( const int & i )
{
	if ( i == 0 ) return N;
	if ( i == 1 ) return W;
	if ( i == 2 ) return S;
	if ( i == 3 ) return E;
	if ( i == 4 ) return FN;
	if ( i == 5 ) return FW;
	if ( i == 6 ) return FS;
	if ( i == 7 ) return FE;
}

int
Nz_Orient2Int( const Orient & ort )
{
	if ( ort == N  )  return 0;
	if ( ort == W  )  return 1;
	if ( ort == S  )  return 2;
	if ( ort == E  )  return 3;
	if ( ort == FN )  return 4;
	if ( ort == FW )  return 5;
	if ( ort == FS )  return 6;
	if ( ort == FE )  return 7;
}
/**Function*************************************************************

  Synopsis    [contour list function]

  Description []
               
  SideEffects []

  SeeAlso     []

***********************************************************************/

List::List(const int& outwidth)
{
   _begin = new ListNode(0,0,0);
   _end   = new ListNode(outwidth,outwidth,0);
   ListNode* newnode = new ListNode(0,outwidth,0);
   concate(_begin,newnode);
   concate(newnode,_end);
}
List::~List()
{
   ListNode* cur = _begin;
   ListNode* nxt = 0;
   while( cur != _end )
   {
      nxt = cur->_next;
      delete cur;
      cur = nxt;
   }
   delete cur;
}

ostream& 
operator << (ostream& os,const ListNode& n)
{
   os<<"("<<n._x0<<","<<n._x1<<","<<n._y1<<") ";
}
ostream& 
operator << (ostream& os,const List& l)
{
   ListNode* cur = l._begin->_next;
   //cout<<"_start = "<<l._start<<endl;
   while(cur != l._end)
   {
      os << *cur;
      cur = cur->_next;
   }
   os << *cur;

   os<<endl;
}
double
List::area()
{
   double sum = 0;
   ListNode* ptr = _begin->_next;
   while(ptr != _end) {
      sum += (ptr->_x1 - ptr->_x0) * ptr->_y1;
      ptr = ptr->_next;
   }
   return sum;
}
void
List::compute_balance_area(double& Larea , double& Rarea, const int& chipwidth)
{
   ListNode* ptr = _begin->_next;
   Larea = Rarea = 0;
   while( (ptr->_x0 + ptr->_x1) / 2 < chipwidth / 2) {
      Larea += (ptr->_x1 - ptr->_x0) * ptr->_y1;
      ptr = ptr->_next;
   }
   while(ptr != _end) {
      Rarea += (ptr->_x1 - ptr->_x0) * ptr->_y1;
      ptr = ptr->_next;
   }
   
}
void
List::concate(ListNode* const & n1,ListNode* const & n2) // concate n2 after n1
{
   assert(n1 != 0 && n2 != 0);
   assert(n1 != _end);
   assert(n1->_next == 0);
   assert(n2->_prev == 0);
   
   n1->_next = n2;
   n2->_prev = n1;
}
int
List::update(const int& x0,const int& x1,const int& h,const bool& isfromleft,double& deadArea) // [x0,x1] is safe range
{
   int y0max = INT_MIN;
   if(isfromleft) {
      // handle ptr1
      ListNode* ptr1 = _begin->_next;
      while(ptr1->_x1 <= x0) ptr1 = ptr1->_next;

      // handle ptr2
      ListNode* ptr2 = _end->_prev;
      while(ptr2->_x0 >= x1) ptr2 = ptr2->_prev;

      // calculate y0max
      ListNode* ptr1p  = ptr1->_prev;
      ListNode* ptr2n  = ptr2->_next;
      int       ptr2y1 = ptr2->_y1;
      ListNode* cur = ptr1;
      ListNode* nxt = NULL;
      while(cur != ptr2n) {
         if(cur->_y1 > y0max) y0max = cur->_y1;
         cur = cur->_next;
      }
      // delete internal nodes , compute _deadArea
      cur = ptr1;
      nxt = NULL;
      while(cur != ptr2) {
         deadArea += (cur->_x1 - cur->_x0) * (y0max - cur->_y1);
         nxt = cur->_next;
         delete cur;
         cur = nxt;
      }
      assert(cur == ptr2);
      assert(ptr2->_x1 >= x1);
      if(cur->_x1 == x1)
         deadArea += (cur->_x1 - cur->_x0) * (y0max - cur->_y1);
      else
         deadArea += (x1 - cur->_x0) * (y0max - cur->_y1);
      delete ptr2;

      ListNode* n1 = new ListNode(x0,x1,y0max+h);
      ptr1p->_next = NULL;
      ptr2n->_prev = NULL;
      concate(ptr1p,n1);
      // handle ptr2
      if(ptr2n->_x0 > x1) {
         ListNode* n2 = new ListNode(x1,ptr2n->_x0,ptr2y1);
         concate(n1,n2);
         concate(n2,ptr2n);
      }
      else if(ptr2n->_x0 == x1) {
         concate(n1,ptr2n);
      }
      else{cout<<"[from left] ptr2n->_x0 < x1\n";}
   }
   else {
      // handle ptr1
      ListNode* ptr1 = _end->_prev;
      while(ptr1->_x0 >= x1) ptr1 = ptr1->_prev;

      // handle ptr2
      ListNode* ptr2 = _begin->_next;
      while(ptr2->_x1 <= x0) ptr2 = ptr2->_next;
      
      // calculate y0max
      ListNode* ptr1p  = ptr1->_next;
      ListNode* ptr2n  = ptr2->_prev;
      int       ptr2y1 = ptr2->_y1;
      ListNode* cur = ptr1;
      ListNode* nxt = NULL;
      while(cur != ptr2n) {
         if(cur->_y1 > y0max) y0max = cur->_y1;
         cur = cur->_prev;
      }
      // delete internal nodes , compute _deadArea
      cur = ptr1;
      nxt = NULL;
      while(cur != ptr2) {
         deadArea += (cur->_x1 - cur->_x0) * (y0max - cur->_y1);
         nxt = cur->_prev;
         delete cur;
         cur = nxt;
      }
      assert(cur == ptr2);
      assert(ptr2->_x0 <= x0);
      if(ptr2->_x0 == x0)
         deadArea += (ptr2->_x1 - ptr2->_x0) * (y0max - ptr2->_y1);
      else
         deadArea += (ptr2->_x1 - x0) * (y0max - ptr2->_y1);
      delete ptr2;
     
      ListNode* n1 = new ListNode(x0,x1,y0max+h);
      ptr1p->_prev = NULL;
      ptr2n->_next = NULL;
      concate(n1,ptr1p);
      // handle ptr2
      if(ptr2n->_x1 < x0) {
         ListNode* n2 = new ListNode(ptr2n->_x1,x0,ptr2y1);
         concate(n2,n1);
         concate(ptr2n,n2);
      }
      else if(ptr2n->_x1 == x0) {
         concate(ptr2n,n1);
      }
      else{cout<<"[from right] ptr2n->_x1 > x0\n";}
   }
   
   
   return y0max;
}
////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////
