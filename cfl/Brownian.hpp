//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflBrownian_hpp__
#define __cflBrownian_hpp__

#include "Slice.hpp"
#include "GaussRollback.hpp"
#include "Interp.hpp"
#include "Ind.hpp"

/**
 * \file   Brownian.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 *
 * \brief Basic financial model where the state process is given by a 
 * one-dimensional Brownian motion.  
 *
 * Contains classes and functions related with the basic financial model where 
 * the state process is given by a one-dimensional Brownian motion.
 */

namespace cfl
{
  /** 
   * \ingroup cflCommonElements
   * \defgroup cflBrownian Basic model with Brownian motion. 
   * This module is dealing with implementation of the basic financial model 
   * where the state process is given by a one-dimensional Brownian motion. 
   * The interest rates are assumed to be equal to zero.
   */
  //@{

  //! Interface class for the basic financial model with Brownian motion. 
  /**
   * This is the interface class for the financial model where interest rate equals 
   * to zero and the state process is given by a one-dimensional Brownian motion. 
   * This abstract class is used to implement concrete class Brownian.
   * \see Brownian
   */
  class IBrownian: public IModel
  {
  public:
    /**
     * Virtual destructor. 
     */ 
    virtual ~IBrownian(){};
    
    /** 
     * Virtual constructor of the model. 
     * \param rVar The vector of variances of the state process.  
     * The element with index \a i of this vector equals the average variance 
     * between the initial time  and the event time with index \a i.  The size
     * of this vector should be equal the size of \a rEventTimes.
     * \param rEventTimes The vector of event times in the model. 
     * \param dInterval The width of the interval of initial values of the Brownian motion 
     * which plays the role of the state process. The center of the interval of initial 
     * values equals zero. 
     * \return A pointer to dynamically allocated implementation of IBrownian. This implementation 
     * corresponds to the model with the parameters \a rEventTimes,  \a rVar, and \a dInterval.
     */
    virtual IBrownian * newModel(const std::vector<double> & rVar, 
				 const std::vector<double> & rEventTimes, 
				 double dInterval) const = 0; 
  };

  //! Concrete class for the basic financial model with Brownian motion. 
  /**
   * This is the standard concrete class for the basic financial model
   * where the interest rate equals to zero and the state process is
   * given by a one-dimensional Brownian motion. This class is
   * implemented by a dynamically allocated implementation of the
   * interface class IBrownian.  \see IBrownian
   */
  class Brownian: public IModel
  {
  public:
    /** 
     * Constructs \a *this from dynamically allocated implementation
     * of the interface class IBrownian. 
     *
     *\param pNewP A pointer to dynamically allocated implementation
     * of the interface class IBrownian.
     */
    explicit Brownian(IBrownian * pNewP);

    /** 
     * Changes event times, variances and the interval of
     * initial values for \a *this. 
     * \param rVar The vector of average variances of the state process between initial and event times.  
     * The element with index \a i of this vector equals the average variance 
     * between the initial time  and the event time with index \a i.  
     * \param rEventTimes The vector of event times in the model. 
     * \param dInterval The width of the interval of initial values of the Brownian motion 
     * which plays the role of the state process. The center of the interval of initial 
     * values equals zero. 
     */
    void assign(const std::vector<double> & rVar, 
		const std::vector<double> & rEventTimes,
		double dInterval);

    /**
     * \copydoc IModel::eventTimes
     */
    const std::vector<double> & eventTimes() const;

    /**
     * \copydoc IModel::numberOfStates  
     */ 
    unsigned numberOfStates() const;

    /**
     * \copydoc IModel::numberOfNodes  
     */
    unsigned numberOfNodes(unsigned iEventTime, 
			   const std::vector<unsigned> & rStates) const;

    /**
     * \copydoc IModel::origin
     */
    std::valarray<double> origin() const;

    /**
     * \copydoc IModel::state
     */   
    Slice state(unsigned iEventTime, unsigned iState) const;

    /**
     * \copydoc IModel::addDependence
     */  
    void addDependence(Slice & rSlice, 
		       const std::vector<unsigned> & rStates) const;

    /**
     * \copydoc IModel::rollback  
     */
    void rollback(Slice & rSlice, unsigned iEventTime) const;

    /**
     * \copydoc IModel::indicator
     */  
    void indicator(Slice & rSlice, double dBarrier) const;

    /**
     * \copydoc IModel::interpolate
     */	
    MultiFunction interpolate(const Slice & rSlice) const;

  private:
    std::shared_ptr<IBrownian> m_pBrownian;
  };

  //! Implementations of class Brownian.
  /** 
   * This namespace contains implementations of the basic 
   * financial model where the state process is a standard
   * Brownian motion and the interest rate equals zero. 
   */
  namespace NBrownian 
  {
    /** 
     * Implements Brownian model from different components. 
     * \param dQuality A trade-off between speed and accuracy of the implementation 
     * of the basic state process. 
     * \param rRollback An implementation of the operator of conditional expectation with 
     * respect to gaussian distribution. 
     * \param rInd A numerically efficient implementation of discontinuous functions. 
     * \param rInterp An implementation of numerical interpolation. 
     * \return Implementation of Brownian model. 
     */
    Brownian model(double dQuality, 
		   const GaussRollback & rRollback = NGaussRollback::improved(), 
		   const Ind & rInd = NInd::smart(), 
		   const Interp & rInterp = NInterp::spline()
		   );	
  }
  //@}
}

#include "cfl/Inline/iBrownian.hpp"
#endif // of __cflBrownian_hpp__

