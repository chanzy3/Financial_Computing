//  Copyright (c) Dmitry Kramkov, 2000-2006. All rights reserved. 
// do not include this file

template <class T>	
inline const T * cfl::begin(const std::valarray<T> & rIn) 
{
  std::valarray<T> & rCastIn = const_cast<std::valarray<T> &>(rIn);
  return &rCastIn[0];
}
	
template <class T>
inline const T * cfl::end(const std::valarray<T> & rIn) 
  {
    return cfl::begin(rIn) + rIn.size();
  }
	
template <class T>
inline bool cfl::equal(const std::valarray<T> & rIn1, const std::valarray<T> & rIn2) 
{
  if (rIn1.size() == rIn2.size()) { 
    return std::equal(begin(rIn1), end(rIn1), begin(rIn2));
  }
  return false;
}

