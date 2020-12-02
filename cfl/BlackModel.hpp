//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 

#ifndef __cflBlackModel_hpp__
#define __cflBlackModel_hpp__

/**
 * \file   BlackModel.hpp
 * \author Dmitry Kramkov
 * \date   2000-2006
 * 
 * \brief Implementation of Black model. 
 *
 * Contains implementations of Black model for a single asset. 
 */


#include "cfl/BlackData.hpp"
#include "cfl/Brownian.hpp"
#include "cfl/AssetModel.hpp"

namespace cfl
{
  namespace Black
  {
    /// \addtogroup cflBlack
    //@{

    /** 
     * Implements AssetModel as Black model. 
     * \param rData The parameters of Black model. 
     * \param dInterval The interval of initial values for the relative 
     * change in the spot price of the underlying asset. 
     * \param dQuality The trade-off between speed and accuracy. 
     * \return Implementation of AssetModel  as Black model. 
     */
    AssetModel model(const Data & rData, 
		     double dInterval, double dQuality);

    /** 
     * Implements AssetModel as Black model. 
     * \param rData The parameters of Black model. 
     * \param dInterval The interval of initial values for the relative 
     * change in the spot price of the underlying asset.
     * \param dQuality The trade-off between speed and accuracy for pricing of standard 
     * derivatives. 
     * \param dPathDependQuality The trade-off between speed and accuracy for pricing of 
     * path-dependent derivatives. 
     * \return Implementation of AssetModel  as Black model. 
     */
    AssetModel model(const Data & rData, 
		     double dInterval, double dQuality, double dPathDependQuality); 

    /** 
     * Implements AssetModel as Black model. 
     * \param rData The parameters of Black model. 
     * \param dInterval The interval of initial values for the relative 
     * change in the spot price of the underlying asset.
     * \param rBrownian Implementation of class Brownian. It is used to 
     * price standard derivatives. 
     * \param rApprox Implementation of class Approx. This class is used in numerical 
     * implementation of path dependent derivatives. 
     * \return Implementation of AssetModel  as Black model. 
     */
    AssetModel model(const Data & rData, 
		     double dInterval, const Brownian & rBrownian, 
		     const Approx & rApprox);

    /** 
     * Implements AssetModel as Black model. 
     * \param rData The parameters of Black model. 
     * \param dInterval The interval of initial values for the relative 
     * change in the spot price of the underlying asset.
     * \param rBrownian Implementation of class Brownian. It is used to 
     * price standard derivatives. 
     * \param rExtended Implementation of class Extended. This class is used in numerical 
     * implementation of path dependent derivatives. 
     * \return Implementation of AssetModel  as Black model. 
     */
    AssetModel model(const Data & rData, 
		     double dInterval, const Brownian & rBrownian, 
		     const Extended & rExtended);
    //@}
  }
}

#include "cfl/Inline/iBlackModel.hpp"
#endif // of __cflBlackModel_hpp__	
 
