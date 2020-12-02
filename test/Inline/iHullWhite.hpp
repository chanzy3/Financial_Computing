inline cfl::HullWhite::Data cfl::test::HullWhite::data() 
{
  return cfl::test::HullWhite::data(c_dYield,  c_dHullWhiteSigma,c_dLambda, c_dInitialTime);
}


inline cfl::Data::Swap cfl::test::HullWhite::swapParameters()
{
  cfl::Data::Swap uSwap;
  uSwap.notional = c_dNotional;
  uSwap.rate = c_dSwapRate;
  uSwap.period = c_dPeriod;
  uSwap.numberOfPayments = c_iNumberOfPeriods;
  uSwap.payFloat = true;
  return uSwap;
}
