//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
//do not include this file

inline void cfl::Brownian::assign(const std::vector<double> & rVar, 
				  const std::vector<double> & rEventTimes,
				  double dInterval)
{
  m_pBrownian.reset(m_pBrownian->newModel(rVar, rEventTimes, dInterval));
}

inline const std::vector<double> & cfl::Brownian::eventTimes() const
{
  return m_pBrownian->eventTimes();
}

   
inline unsigned cfl::Brownian::numberOfStates() const
{
  return m_pBrownian->numberOfStates();
}

   
inline unsigned cfl::Brownian::numberOfNodes(unsigned iEventTime, 
					     const std::vector<unsigned> & rStates) const
{
  return m_pBrownian->numberOfNodes(iEventTime, rStates);
}

inline std::valarray<double> cfl::Brownian::origin() const
{
  return m_pBrownian->origin();
}
    
inline cfl::Slice cfl::Brownian::state(unsigned iEventTime, unsigned iState) const
{
  Slice uState(m_pBrownian->state(iEventTime, iState));
  uState.assign(*this);
  return uState;
}

inline void cfl::Brownian::addDependence(Slice & rSlice, 
					 const std::vector<unsigned> & rStates) const
{
  rSlice.assign(*m_pBrownian);
  m_pBrownian->addDependence(rSlice, rStates);
  rSlice.assign(*this);
}

   
inline void cfl::Brownian::rollback(Slice & rSlice, unsigned iEventTime) const
{
  rSlice.assign(*m_pBrownian);
  rSlice.rollback(iEventTime);
  rSlice.assign(*this);
}

inline void cfl::Brownian::indicator(Slice & rSlice, double dBarrier) const
{
  rSlice.assign(*m_pBrownian);
  m_pBrownian->indicator(rSlice, dBarrier); 
  rSlice.assign(*this);
}

 	
inline cfl::MultiFunction cfl::Brownian::interpolate(const Slice & rSlice) const
{
  Slice uSlice(rSlice);
  uSlice.assign(*m_pBrownian);
  return m_pBrownian->interpolate(uSlice);
}


