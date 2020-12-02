//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// Implementation of classes and functions declared in the corresponding *.hpp file. 

#include "cfl/HullWhiteData.hpp"
#include "cfl/Data.hpp"

cfl::HullWhite::Data::Data(const cfl::Function & rDiscount, 
			   const Function & rVolatility, 
			   const Function & rShape, 
			   double dInitialTime
			   )
  :m_uDiscount(rDiscount), m_uVolatility(rVolatility), 
   m_uShape(rShape), m_dInitialTime(dInitialTime)
{}

cfl::HullWhite::Data::Data(const cfl::Function & rDiscount, double dSigma, 
			   double dLambda, double dInitialTime)
  :m_uDiscount(rDiscount), 
   m_uVolatility(cfl::Data::volatility(dSigma, dLambda, dInitialTime)),
   m_uShape(cfl::Data::bondShape(dLambda, dInitialTime)),
   m_dInitialTime(dInitialTime)
{}





