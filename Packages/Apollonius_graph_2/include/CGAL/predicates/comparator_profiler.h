// ======================================================================
//
// Copyright (c) 2003 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------
//
// release       : 
// release_date  : 
//
// file          : include/CGAL/predicates/comparator_profiler.h
// package       : Apollonius_graph_2
// source        : $RCSfile$
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Menelaos Karavelas <mkaravel@cse.nd.edu>
//
// coordinator   :
//
// ======================================================================



#ifndef CGAL_COMPARATOR_PROFILER_H
#define CGAL_COMPARATOR_PROFILER_H

CGAL_BEGIN_NAMESPACE

class comparator_profiler
{
public:
  static bool count_cases;
  static unsigned long case_1_counter;
  static unsigned long case_2_counter;
  static unsigned long case_3a_Jpos_counter;
  static unsigned long case_3a_Jneg_counter;
  static unsigned long case_3b_Jpos_counter;
  static unsigned long case_3b_Jneg_counter;
  static unsigned long case_4_counter;
  static unsigned long case_5_counter;
  static unsigned long case_degenerate_counter;
public:
  static unsigned long counter_rr;
  static unsigned long counter_rr_p3inf;
  static unsigned long counter_rr_p4;
  static unsigned long counter_rr_e;
  static unsigned long counter_rr_r0;
  //  static unsigned long counter_rr_jneg;

  static void reset()
  {
    count_cases = false;
    case_1_counter = 0;
    case_2_counter = 0;
    case_3a_Jpos_counter = 0;
    case_3a_Jneg_counter = 0;
    case_3b_Jpos_counter = 0;
    case_3b_Jneg_counter = 0;
    case_4_counter = 0;
    case_5_counter = 0;
    case_degenerate_counter = 0;

    counter_rr = 0;
    counter_rr_p3inf = 0;
    counter_rr_p4 = 0;
    counter_rr_e = 0;
    counter_rr_r0 = 0;
    //    counter_rr_jneg = 0;
  }

  template< class FT >
  static void count_case(const FT& a1, const FT& b1, const FT& c1,
			 const FT& a2, const FT& b2, const FT& c2)
  {
    // works correctly only with leda_real
    FT D1 = CGAL_NTS square(b1) - a1 * c1;
    
    FT l1 = (b1 - CGAL_NTS sqrt(D1)) / a1;
    FT r1 = (b1 + CGAL_NTS sqrt(D1)) / a1;
    if ( a1 < 0 ) { std::swap(r1, l1); }
    
    FT D2 = CGAL_NTS square(b2) - a2 * c2;

    if ( D1 == 0 || D2 == 0 ) {
      case_degenerate_counter++;
      return;
    }

    FT l2 = (b2 - CGAL_NTS sqrt(D2)) / a2;
    FT r2 = (b2 + CGAL_NTS sqrt(D2)) / a2;
    if ( a2 < 0 ) { std::swap(r2, l2); }

    if ( l1 < l2 ) {
      if ( r1 > r2 ) {
	FT J = a1 * b2 - a2 * b1;
	if ( J > 0 ) {
	  case_3b_Jpos_counter++;
	} else if ( J < 0 ) {
	  case_3b_Jneg_counter++;
	} else {
	  case_degenerate_counter++;
	}
      } else if ( r1 < r2 ) {
	if ( r1 < l2 ) {
	  case_5_counter++;
	} else if ( r1 > l2 ) {
	  case_4_counter++;
	} else {
	  case_degenerate_counter++;
	}
      } else {
	case_degenerate_counter++;
      }
    } else if ( l1 > l2 ) {
      if ( r1 < r2 ) {
	FT J = a1 * b2 - a2 * b1;
	if ( J > 0 ) {
	  case_3a_Jpos_counter++;
	} else if ( J < 0 ) {
	  case_3a_Jneg_counter++;
	} else {
	  case_degenerate_counter++;
	}
      } else if ( r1 > r2 ) {
	if ( l1 < r2 ) {
	  case_2_counter++;
	} else if ( l1 > r2 ) {
	  case_1_counter++;
	} else {
	  case_degenerate_counter++;
	}
      } else {
	case_degenerate_counter++;
      }
    } else {
      case_degenerate_counter++;
    }
  }
};

bool comparator_profiler::count_cases = false;
unsigned long comparator_profiler::case_1_counter = 0;
unsigned long comparator_profiler::case_2_counter = 0;
unsigned long comparator_profiler::case_3a_Jpos_counter = 0;
unsigned long comparator_profiler::case_3a_Jneg_counter = 0;
unsigned long comparator_profiler::case_3b_Jpos_counter = 0;
unsigned long comparator_profiler::case_3b_Jneg_counter = 0;
unsigned long comparator_profiler::case_4_counter = 0;
unsigned long comparator_profiler::case_5_counter = 0;
unsigned long comparator_profiler::case_degenerate_counter = 0;

unsigned long comparator_profiler::counter_rr = 0;
unsigned long comparator_profiler::counter_rr_p3inf = 0;
unsigned long comparator_profiler::counter_rr_p4 = 0;
unsigned long comparator_profiler::counter_rr_e = 0;
unsigned long comparator_profiler::counter_rr_r0 = 0;
//unsigned long comparator_profiler::counter_rr_jneg = 0;

CGAL_END_NAMESPACE

#endif // CGAL_COMPARATOR_PROFILER_H
