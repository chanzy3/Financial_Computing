//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflError_hpp__
#define __cflError_hpp__

#include "cfl/Macros.hpp"
#include <exception>
#include <string>

/**
 * \file   Error.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief  Exceptions in cfl library.
 * 
 * The file contains exception classes and functions for cfl library. 
 */

namespace cfl
{
  /**
   * \defgroup cflMisc Various items. 
   * This module contains miscellaneous items related to the library. 
   */

  /**
   * \ingroup cflMisc
   * \defgroup cflError Exceptions.
   * This module contains exception classes and functions for cfl library. 
   */ 
  //@{

  //! Exception class for cfl library. 
  /**
   * This is the exception class for cfl library. 
   */
  class Error: public std::exception
  {
  public:
    /** 
     * Constructor. 
     * \param pWhere Information on an error. 
     */
    Error (const char * pWhere);

    /**
     * Virtual destructor. 
     */
    ~Error() throw() {};

    /** 
     * The function returns information on the thrown exception. 
     * \return Description of the error.
     */
    const char * what() const throw();

  private:
    std::string m_sErr; 
  };

  //! Implementations of Error class for different types of exceptions. 
  /** 
   * This namespace contains implementations of Error class adapted for 
   * different types of exceptions. 
   */
  namespace NError
  {
    /** 
     * This type of error indicates that some parameter is out of acceptable 
     * range. For example, negative volatility, invalid time. 
     * \param pWhere Additional info on the error.  
     * \return Generic class for exceptions in cfl library.
     */
    Error range(const char * pWhere);

    /** 
     * This type of error usually indicates that an input vector (for example, a vector 
     * of event times) is not sorted, while it should be. 
     * \param pWhere Additional info on the error. 
     * \return Generic class for exceptions in cfl library.
     */
    Error sort(const char * pWhere);

    /** 
     * This type of error indicates that some vector or array does not have 
     * correct size. 
     * \param pWhere Additional info on the error. For example, which vector 
     * has invalid size. 
     * \return Generic class for exceptions in cfl library.
     */
    Error size(const char * pWhere);
  }
  //@}
}

#include "cfl/Inline/iError.hpp"
#endif //__cflError_hpp__
