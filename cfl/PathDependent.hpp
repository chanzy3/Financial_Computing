//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflPathDependent_hpp__
#define __cflPathDependent_hpp__

#include "cfl/Slice.hpp"
#include "cfl/Approx.hpp"

/**
 * \file   PathDependent.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * \brief Path dependent processes as additional state processes.
 *
 * This file contains classes and functions related with path dependent 
 * processes that could be used as additional state processes. 
 */

namespace cfl
{
  /// \addtogroup cflPathDepend 
  //@{

  //! Interface class for a path dependent process.
  /**
   * This is the abstract class for a path dependent process that could be used 
   * as an additional state process. The implementation of this
   * object on a free store is used to construct standard concrete class PathDependent.
   * \see PathDependent
   */
  class IResetValues
  {
  public:
    /**
     * Virtual destructor. 
     */ 
    virtual ~IResetValues(){};
    
    /** 
     * Returns the values of the path-dependent process at the event 
     * time with index \a iEventTime under the condition that the 
     * value of the process immediately before the event time equals 
     * \a dBeforeReset. 
     * \param iEventTime The index of the reset time in the vector of all event 
     * times of the model.
     * \param dBeforeReset The value of the path dependent process immediately 
     * before the given reset time.
     * \return The values of the path-dependent state process at the event 
     * time with index \a iEventTime under the condition that the 
     * value of the process immediately before the event time equals 
     * \a dBeforeReset. 
     */
    virtual Slice resetValues(unsigned iEventTime, double dBeforeReset) const = 0;	
  };
	
  //!Standard concrete class for path dependent functions. 
  /** 
   * This is the standard class for a path dependent functional that 
   * could be used as an additional state process. 
   * It is implemented by a dynamically allocated object derived from 
   * the interface class IResetValues. 
   * \see IResetValues
   */
  class PathDependent
  {
  public:
    /** The constructor. 
     * \param pNewP A dynamically allocated implementation of the interface class IResetValues. 
     * \param rTimeIndexes A constant reference to the vector of indexes of reset times.
     * \param dOrigin The center of the interval of initial values of path dependent process. 
     * \param dInterval The width of the interval of initial values of path dependent process. The 
     * default (usual) value for this argument is 0. 
     */
    PathDependent(IResetValues * pNewP, 
		  const std::vector<unsigned> & rTimeIndexes, 
		  double dOrigin, 
		  double dInterval = 0.);
		
    /** \copydoc IResetValues::resetValues()*/
    Slice resetValues(unsigned iEventTime, double dBeforeReset) const;
		
    /** 
     * Accessor function to the vector of indexes of reset times for path dependent process. 
     * \return The vector of indexes of reset times for path dependent process. 
     */
    const std::vector<unsigned> & timeIndexes() const;
		
    /** 
     * Accessor function to the center of the interval of initial values of the 
     * path-dependent process.
     * \return The center of the interval of initial values of path-dependent process.
     */
    double origin() const;
		
    /** 
     * Accessor function to the width of the interval of initial values 
     * for path-dependent state process. Usually, this value is 0. 
     * \return The width of the interval of initial values of path-dependent process.
     */
    double  interval() const;
  private:
    std::shared_ptr<IResetValues> m_pP;
    std::vector<unsigned> m_uTimeIndexes;
    double m_dOrigin, m_dInterval;
  };
  //@} 
}

#include "cfl/Inline/iPathDependent.hpp"
#endif // of __cflPathDependent_hpp__

