//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline cfl::Slice 
cfl::PathDependent::resetValues(unsigned iEventTime, double dBeforeReset) const 
{ 
  ASSERT(std::binary_search(m_uTimeIndexes.begin(), m_uTimeIndexes.end(), iEventTime));
  return m_pP->resetValues(iEventTime, dBeforeReset); 
}
		
inline const std::vector<unsigned> & cfl::PathDependent::timeIndexes() const 
{ 
  return m_uTimeIndexes; 
}
		
inline double cfl::PathDependent::origin() const 
{ 
  return m_dOrigin; 
}
		
inline double  cfl::PathDependent::interval() const 
{ 
  return m_dInterval; 
}
