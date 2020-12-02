//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflMacros_hpp__
#define __cflMacros_hpp__

#include <assert.h>
#include <algorithm>

/**
 * \file   Macros.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief  Macros and constants for cfl library. 
 * 
 * This file contains macros and definitions for cfl library.
 */


namespace cfl
{
  /**
   * \ingroup cflMisc
   * \defgroup cflMacros Macros and constants for cfl library. 
   * This module contains macros and constants for cfl library. 
   */
  //@{

  /// Check conditions at the beginning of a function. 
  /**
   * The term \a ASSERT is used instead of \a std::asset to verify the
   * validity of input parameters.
   * 
   */
#define PRECONDITION assert

  /// Check conditions everywhere. 
 /**
   * The term \a ASSERT is used instead of \a std::asset to verify the
   * validity of variables everywhere in the program.
   * 
   */
#define ASSERT assert

  /// Check conditions at the end of a function. 
 /**
   * The term \a POSTCONDITION is used instead of \a std::asset to
   * verify the validity of output parameters.
   * 
   */
#define POSTCONDITION assert 

  /// A small positive number.
 /**
   * This is a small positive number on which it is safe to divide and
   * perform other numerical operations impossible for 0. 
   * 
   */
  const double c_dEps = 10E-8;

  //@}
}

#endif//__cflMacros_hpp__
