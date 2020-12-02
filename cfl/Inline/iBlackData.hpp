//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline double cfl::Black::Data::initialTime() const 
{ 
  return m_dInitialTime; 
}

inline const cfl::Function & cfl::Black::Data::discount() const 
{ 
  return m_uDiscount; 
}

inline const cfl::Function & cfl::Black::Data::forward() const 
{ 
  return m_uForward; 
}

inline const cfl::Function & cfl::Black::Data::volatility() const 
{ 
  return m_uVolatility; 
}

inline const cfl::Function & cfl::Black::Data::shape() const 
{ 
  return m_uShape; 
}
