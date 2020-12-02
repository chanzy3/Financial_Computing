//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline cfl::InterestRateModel 
cfl::HullWhite::model(const Data & rData, 
		      double dInterval, double dQuality)
{
  return HullWhite::model(rData, dInterval, NBrownian::model(dQuality),
			  NExtended::model(dQuality)); 
}

inline cfl::InterestRateModel 
cfl::HullWhite::model(const Data & rData, double dInterval, 
		      double dQuality, double dPathDependQuality) 
{
  return cfl::HullWhite::model(rData, dInterval, NBrownian::model(dQuality), 
			       NExtended::model(dPathDependQuality)); 
}

inline cfl::InterestRateModel 
cfl::HullWhite::model(const Data & rData, double dInterval, 
		      const Brownian & rBrownian, const Approx & rApprox)
{
  return cfl::HullWhite::model(rData, dInterval, rBrownian, 
			       NExtended::model(rApprox)); 
}
