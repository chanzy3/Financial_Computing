//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
#ifndef __cflInterp_hpp__
#define __cflInterp_hpp__

#include "cfl/Function.hpp"
#include <vector>

/**
 * \file   Interp.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief  Interpolation of one-dimensional functions.
 * 
 * This file contains functions and classes related with interpolation of one-dimensional functions. 
 */

namespace cfl
{
  /**
   * \ingroup cflNumeric
   * \defgroup cflInterp Interpolation of one-dimensional functions.
   * This module is dealing with numerical interpolation of one-dimensional functions. 
   */
  //@{

  //! Interface class for numerical interpolation.
  /** 
   * This is the abstract class for numerical implementation. Its implementations 
   * are used to construct concrete class Interp.
   * \see Interp and NInterp
   */
  class  IInterp
  {
  public:
    /**
     * Virtual destructor. 
     */ 
    virtual ~IInterp(){}

    /**
     * Interpolation of the data supplied by the vectors of arguments (\a rArg) and values (\a rVal).
     * \param rArg The vector of arguments.
     * \param rVal The vector of values. 
     * \return Interpolating function. 
     */
    virtual Function interpolate(const std::vector<double> & rArg, 
				 const std::vector<double> & rVal) const = 0;
  };

  //! Standard concrete class for interpolation of one-dimensional functions. 
  /** 
   * This is the standard class for one-dimensional interpolation. It is implemented by a
   * dynamically allocated object derived from interface class IInterp. 
   * \see IInterp and NInterp
   */
  class Interp
  {
  public:
    /**
     * A constructor. 
     * \param pNewP A dynamically allocated implementation of IInterp. 
     */
    explicit Interp(IInterp * pNewP = 0);

    /**
     * Returns interpolated function for given arguments and values. 
     * \param itArgBegin The iterator to the first element of the 
     * sequence of arguments
     * \param itArgEnd The iterator to the last plus one element of the 
     * sequence of arguments
     * \param itValBegin The iterator to the first element of the 
     * sequence of values
     * \return Interpolating function.
     */
    template <class InIt1, class InIt2> 
    Function interpolate(InIt1 itArgBegin, InIt1 itArgEnd, 
			 InIt2 itValBegin) const;
  private:
    std::shared_ptr<IInterp> m_uP;
  };

  //! Implementations of different interpolation methods.
  /** 
   * This namespace contains implementations of interpolation
   * schemes for one-dimensional functions
   * \see IInterp and Interp
   */
  namespace NInterp 
  {
    /** 
     * Constructs linear interpolation scheme. 
     * \return Class Interp that corresponds to linear interpolation. 
     */ 
    Interp linear(); 

    /** 
     * Constructs natural cubic spline interpolation scheme. 
     * \return Class Interp that corresponds to natural cubic spline. 
     */ 
    Interp spline(); 
  }
  //@}
}

#include "cfl/Inline/iInterp.hpp"
#endif // of __cflInterp_hpp__
