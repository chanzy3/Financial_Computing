//do not include this file

inline double cfl::MultiFunction::operator()(const std::valarray<double> & rX) const 
{ 
	return (*m_pF)(rX); 
}

inline bool cfl::MultiFunction::belongs(const std::valarray<double> & rX) const 
{
	return m_pF->belongs(rX); 
}

inline unsigned cfl::MultiFunction::dim() const 
{ 
	return m_pF->dim(); 
}

