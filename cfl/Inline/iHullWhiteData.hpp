// do not include this file

inline double cfl::HullWhite::Data::initialTime() const 
{
	return m_dInitialTime;
}

inline const cfl::Function & cfl::HullWhite::Data::discount() const 
{
	return m_uDiscount;
}

inline const cfl::Function & cfl::HullWhite::Data::volatility() const 
{
	return m_uVolatility;
}

inline const cfl::Function & cfl::HullWhite::Data::shape() const 
{ 
	return m_uShape;
}
