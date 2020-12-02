//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflAuxiliary_hpp__
#define __cflAuxiliary_hpp__

#include <vector>
#include "cfl/Function.hpp"
#include "cfl/Error.hpp"




/**
 * \file   Auxiliary.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief  Auxiliary functions and classes. 
 * 
 * This file contains auxiliary (probably, unnecessary) 
 * functions and classes. 
 */


namespace cfl
{
  /**
   * \ingroup cflMisc
   * \defgroup cflAuxiliary Auxiliary functions and classes. 
   * This group contains auxiliary (probably, unnecessary) 
   * functions and classes. 
   */
  //@{

  /** 
   * This function returns constant pointer to the first element of valarray. 
   * This allows us to use STL algorithms with valarray. 
   * \param rIn A reference to valarray. 
   * \return A constant pointer to the first element of valarray. 
   */
  template <class T>	
  const T * begin(const std::valarray<T> & rIn);

  /** 
   * This function returns constant pointer to the memory position 
   * immediately following the last element of valarray. 
   * This allows us to use STL algorithms with valarray. 
   * \param rIn A reference to valarray. 
   * \return A constant pointer to the memory immediately following 
   * the last element of valarray. 
   */
  template <class T>	
  const T * end(const std::valarray<T> & rIn);

  /** 
   * This function compares the identity of two valarrays. 
   * \param rIn1 The first valarray. 
   * \param rIn2 The second valarray. 
   * \return Returns \p true if \a rIn1 equals \a rIn2 and \p false otherwise. 
   */	
  template <class T>
  bool equal(const std::valarray<T> & rIn1, const std::valarray<T> & rIn2);

  //! Solver for tridiagonal system of equations. 
  /**
   * This class solves tridiagonal system of equations. 
   */
  class Tridiag
  {
  public:
    /** 
     * Default constructor. 
     */
    Tridiag();

    /** 
     * Constructor for tridiagonal matrix. 
     * \param rL The vector below diagonal. 
     * \param rD The vector on diagonal. 
     * \param rU The vector above diagonal. 
     */
    Tridiag(const std::valarray<double> & rL,
	    const std::valarray<double> & rD,
	    const std::valarray<double> & rU
	    );
		
    /** 
     * Replaces \a rX with the solution \p x of the linear equation: <code> y = Ax </code>, where
     * \p A is the given tridiagonal matrix and \p y is initial value of \a rX. 
     * \param rX \em Before the operation \a rX coincides with \p y 
     * and \em after the operation it coincides with \p x. Here \p y and \p x 
     * are related by the linear equation: <code> y = Ax </code>.
     */
    void solve(std::valarray<double> & rX) const;
		
    /** 
     * Replaces \p this with tridiagonal matrix which elements 
     * are defined by vectors \a rL, \a rD and \a rU.  
     * \param rL The vector below diagonal. 
     * \param rD The vector on diagonal. 
     * \param rU The vector above diagonal. 
     */
    void assign(const std::valarray<double> & rL, 
		const std::valarray<double> & rD,
		const std::valarray<double> & rU
		);
  private:
    std::valarray<double> m_uL, m_uD, m_uU;
  };

  //@}
}

#include "cfl/Inline/iAuxiliary.hpp"
#endif // of __cflAuxiliary_hpp__
