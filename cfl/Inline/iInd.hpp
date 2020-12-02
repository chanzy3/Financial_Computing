//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline void 
cfl::Ind::indicator(std::valarray<double> & rValues, double dBarrier) const 
{
  return m_pInd->indicator(rValues, dBarrier);
}

