
//Author: Chengzhi Huang
//Andrew Id: chengzhh
/**
 * 
 * This assignment focuses on Pricing of standard and barrier options in single asset model, namely,
 * -Straddle option
 * -American call on forward
 * -Down-and-rebate barrier option
 * -Up-and-in american put
 * 
 **/

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include "Homework2/Homework2.hpp"
#include "cfl/Interp.hpp"
#include "Examples/Examples.hpp"

// Straddle option is the sum of the standard european put and call options with the same strke
// and maturity
cfl::MultiFunction prb::straddle(double dStrike, double dMaturity,
                                 cfl::AssetModel &rModel)
{
    PRECONDITION(rModel.initialTime() < dMaturity);
    std::vector<double> uEventTimes(1, rModel.initialTime());
    uEventTimes.push_back(dMaturity);
    rModel.assignEventTimes(uEventTimes);

    int iEventTime = 1;
    cfl::Slice uOption = cfl::max(dStrike - rModel.spot(iEventTime), 0.) + cfl::max(rModel.spot(iEventTime) - dStrike, 0.);
    uOption.rollback(0);
    return cfl::interpolate(uOption);
}

cfl::MultiFunction
prb::americanCallOnForward(double dForwardPrice,
                           double dTimeToMaturity,
                           const std::vector<double> &rExerciseTimes,
                           cfl::AssetModel &rModel)
{
    PRECONDITION(rModel.initialTime() < rExerciseTimes.front());

    std::vector<double> uEventTimes(rExerciseTimes);
    uEventTimes.insert(uEventTimes.begin(), rModel.initialTime());
    rModel.assignEventTimes(uEventTimes);

    int iTime = uEventTimes.size() - 1;
    cfl::Slice uOption = rModel.cash(iTime, 0.);
    while (iTime > 0)
    {
        //uOption is the value to continue
        cfl::Slice payoff = rModel.forward(iTime,uEventTimes[iTime] + dTimeToMaturity) - dForwardPrice;
        cfl::Slice discount = rModel.discount(iTime,uEventTimes[iTime] + dTimeToMaturity);
        
        uOption = cfl::max(uOption, payoff*discount);
        iTime--;
        uOption.rollback(iTime);
    }
    return cfl::interpolate(uOption);
}

cfl::MultiFunction
prb::downAndRebate(double dLowerBarrier, double dNotional,
                   const std::vector<double> &rBarrierTimes,
                   cfl::AssetModel &rModel)
{
    PRECONDITION(rModel.initialTime() < rBarrierTimes.front());

    std::vector<double> uEventTimes(rBarrierTimes);
    uEventTimes.insert(uEventTimes.begin(), rModel.initialTime());
    rModel.assignEventTimes(uEventTimes);
	
    int iTime = uEventTimes.size()-1;
    cfl::Slice uOption = rModel.cash(iTime, 0); 
    while (iTime > 0) {
        //uOption is the value to continue (the value of the option 
        //if no barriers have been crossed before and now)
        uOption = cfl::indicator(rModel.spot(iTime), dLowerBarrier) * uOption + cfl::indicator(dLowerBarrier, rModel.spot(iTime)) * dNotional;
        iTime--;
        uOption.rollback(iTime);
    }
    return cfl::interpolate(uOption);
}

cfl::MultiFunction
prb::upAndInAmericanPut(double dBarrier,
                        const std::vector<double> &rBarrierTimes,
                        double dStrike,
                        const std::vector<double> &rExerciseTimes,
                        cfl::AssetModel &rModel)
{
    PRECONDITION(rModel.initialTime() < rBarrierTimes.front());
    PRECONDITION(rModel.initialTime() < rExerciseTimes.front());
    PRECONDITION(rBarrierTimes.back() < rExerciseTimes.back());

    std::vector<double> 
        uEventTimes(1 + rBarrierTimes.size() + rExerciseTimes.size());
    uEventTimes.front() = rModel.initialTime();
    std::vector<double>::iterator itEnd = 
        std::set_union(rBarrierTimes.begin(), rBarrierTimes.end(), 
            rExerciseTimes.begin(), rExerciseTimes.end(), 
            uEventTimes.begin()+1);
    uEventTimes.resize(itEnd-uEventTimes.begin());
    rModel.assignEventTimes(uEventTimes);
        
    int iTime = uEventTimes.size()-1;
    cfl::Slice ameriOption = rModel.cash(iTime, 0);
    // cfl::Slice euroOption = rModel.cash(iTime, 0);
    cfl::Slice uOption = rModel.cash(iTime, 0);
    //cfl::Slice uOption = max(dStrike - rModel.spot(iTime), 0.);

    while (iTime > 0) {
        //uOption is the value to continue
        double dTime = uEventTimes[iTime];
        if (std::binary_search(rExerciseTimes.begin(),rExerciseTimes.end(),dTime)){
        ameriOption = max(ameriOption, dStrike - rModel.spot(iTime));
        }
        if (std::binary_search(rBarrierTimes.begin(),rBarrierTimes.end(), dTime)){
        // uOption *= indicator(rModel.spot(iTime), dLowerBarrier);
        uOption = cfl::indicator(rModel.spot(iTime), dBarrier) * ameriOption + cfl::indicator(dBarrier, rModel.spot(iTime)) * uOption;
        }
        iTime--;
        ameriOption.rollback(iTime);
        uOption.rollback(iTime);
    }

    return interpolate(uOption);
}
