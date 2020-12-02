//Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
//do not include this file

inline const char * cfl::Error::what() const throw()
{
  return m_sErr.data();
}
