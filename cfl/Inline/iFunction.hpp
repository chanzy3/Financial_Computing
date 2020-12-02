//do not include this file

inline double cfl::Function::operator()(double dX) const 
{
  return m_pF->operator()(dX);
}
		
inline bool cfl::Function::belongs(double dX) const 
{
  return m_pF->belongs(dX);
}

namespace cflFunction
{
  template<class F>
  class Adapter: public cfl::IFunction
  {
  public:
    Adapter(const F & rFunc, double dL, double dR)
      : m_uF(rFunc), m_dL(dL), m_dR(dR)
    {
      POSTCONDITION(dL<=dR);		
    }
    double operator()(double dX) const {
      PRECONDITION(belongs(dX));
      return m_uF(dX);
    }
    bool belongs(double dX) const { return (m_dL<=dX)&&(dX<=m_dR); }
  private:
    F m_uF;
    double m_dL, m_dR;
  };
}

template <class F>
inline cfl::Function cfl::toFunction(const F & rF, 
				     double dL, 
				     double dR)
{
  return Function(new cflFunction::Adapter<F>(rF, dL, dR));
}
