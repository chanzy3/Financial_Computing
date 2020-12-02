//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline cfl::AssetModel 
cfl::Black::model(const Data & rData, double dInterval, double dQuality)
{
  return Black::model(rData, dInterval, NBrownian::model(dQuality), 
		      NExtended::model(dQuality)); 
}

inline cfl::AssetModel 
cfl::Black::model(const Data & rData, double dInterval, double dQuality, 
		  double dPathDependQuality)
{
  return cfl::Black::model(rData, dInterval, NBrownian::model(dQuality), 
			   NExtended::model(dPathDependQuality)); 
}

inline cfl::AssetModel 
cfl::Black::model(const Data & rData, double dInterval, 
		  const Brownian & rBrownian, const Approx & rApprox)
{
  return cfl::Black::model(rData, dInterval, rBrownian, 
			   NExtended::model(rApprox)); 
}
