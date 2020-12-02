//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflHullWhiteModel_hpp__
#define __cflHullWhiteModel_hpp__

/**
 * \file   HullWhiteModel.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Implementation of Hull and White model. 
 *
 * Contains the implementation of Hull and White model for interest rates. 
 */

#include "cfl/HullWhiteData.hpp"
#include "cfl/Brownian.hpp"
#include "cfl/InterestRateModel.hpp"

namespace cfl
{
  namespace HullWhite
  {
    /// \addtogroup cflHullWhite
    //@{

    /** 
     * Implements InterestRateModel as Hull and White model. 
     * \param rData The parameters of Hull and White model. 
     * \param dInterval The interval of initial values for short-term 
     * interest rate. 
     * \param dQuality The trade-off between speed and accuracy. 
     * \return InterestRateModel as Hull and White model. 
     */
    InterestRateModel model(const Data & rData, double dInterval, double dQuality);

    /** 
     * Implements InterestRateModel as Hull and White model. 
     * \param rData The parameters of Hull and White model. 
     * \param dInterval The interval of initial values for short-term 
     * interest rate. 
     * \param dQuality The trade-off between speed and accuracy for pricing of standard 
     * derivatives. 
     * \param dPathDependQuality The trade-off between speed and accuracy for pricing of 
     * path-dependent derivatives. 
     * \return InterestRateModel as Hull and White model. 
     */
    InterestRateModel model(const Data & rData, double dInterval, double dQuality, 
			    double dPathDependQuality); 

    /** 
     * Implements InterestRateModel as Hull and White model. 
     * \param rData The parameters of Hull and White model. 
     * \param dInterval The interval of initial values for short-term 
     * interest rate. 
     * \param rBrownian Implementation of class Brownian. It is used to 
     * price standard derivatives. 
     * \param rApprox Implementation of class Approx. This class is used in numerical 
     * implementation of path dependent derivatives. 
     * \return InterestRateModel as Hull and White model. 
     */
    InterestRateModel model(const Data & rData, double dInterval, const Brownian & rBrownian, 
			    const Approx & rApprox);

    /** 
     * Implements InterestRateModel as Hull and White model. 
     * \param rData The parameters of Hull and White model. 
     * \param dInterval The interval of initial values for short-term 
     * interest rate. 
     * \param rBrownian Implementation of class Brownian. It is used to 
     * price standard derivatives. 
     * \param rExtended Implementation of class Extended. This class is used in numerical 
     * implementation of path dependent derivatives. 
     * \return InterestRateModel as Hull and White model. 
     */
    InterestRateModel model(const Data & rData, double dInterval, const Brownian & rBrownian, 
			    const Extended & rExtended);

    //@}
  }
}

#include "cfl/Inline/iHullWhiteModel.hpp"
#endif // of __cflHullWhiteModel_hpp__	 
