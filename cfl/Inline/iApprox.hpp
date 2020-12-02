//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
//do not include this file directly

inline void cfl::Approx::assign(double dL, double dR) 
{
  PRECONDITION(dL <= dR);
  m_uP.reset(m_uP->newApprox(dL,dR));
}

inline const std::valarray<double> & cfl::Approx::arg() const 
{
  return m_uP->arg();
}
		
inline cfl::Function 
cfl::Approx::approximate(const std::valarray<double> & rValues) const 
{
  PRECONDITION(rValues.size() == arg().size());
  return m_uP->approximate(rValues);
}
