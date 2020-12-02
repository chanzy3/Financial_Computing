//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

inline void cfl::GaussRollback::assign(unsigned iSize, double dH, double dVar) 
{
  m_uP.reset(m_uP->newObject(iSize, dH, dVar));
}

inline void cfl::GaussRollback::rollback(std::valarray<double> & rValues) const 
{
  m_uP->rollback(rValues);
}
