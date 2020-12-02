
//Author: Chengzhi Huang
//Andrew Id: chengzhh
/**
 * 
 * This assignment focuses on construction of data curves, namely,
 * -Forward curve for exchange rate 
 * -Forward price curve for a counpon bond
 * -Forward curve obtained from forward prives by log linear interpolation
 * -Staionary volatility curve for Black model obtained by the least squire approximation of 
 * market implied volatilities
 * 
 **/
#include <iostream>
#include <vector>
#include <numeric>
#include "Homework1/Homework1.hpp"
#include "cfl/Interp.hpp"
#include "Examples/Examples.hpp"

namespace chengzhhhw1
{
    class forwardFXFunc : public cfl::IFunction
    {
    public:
        forwardFXFunc(double dSpotFX, const cfl::Function &rDomesticDiscount, const cfl::Function &rForeignDiscount)
        {
            m_dSpotFX = dSpotFX;
            m_rDomesticDiscount = rDomesticDiscount;
            m_rForeignDiscount = rForeignDiscount;
        }

        double operator()(double dX) const
        {
            return m_dSpotFX * m_rForeignDiscount(dX) / m_rDomesticDiscount(dX);
        }

        bool belongs(double dX) const
        {
            return m_rDomesticDiscount.belongs(dX) && m_rForeignDiscount.belongs(dX);
        }

    private:
        double m_dSpotFX;
        cfl::Function m_rDomesticDiscount;
        cfl::Function m_rForeignDiscount;
    };

    class forwardFXCouponFunc : public cfl::IFunction
    {
    public:
        forwardFXCouponFunc(const cfl::Data::CashFlow &rBond, const cfl::Function &rDiscount, double dInitialTime, bool bClean)
        {
            m_rBond = rBond;
            m_rDiscount = rDiscount;
            m_dInitialTime = dInitialTime;
            m_bClean = bClean;
        }

        // Dirty Price = sum(q*N*d(ti)/d(t))+N*d(T)/d(t)
        // Clean Price = Dirty Price - Accured Interest
        double operator()(double dX) const
        {
            double dirtyPrice = 0;
            std::vector<double> timeFrames;
            for (int i = 0; i < m_rBond.numberOfPayments + 1; i++)
            {
                timeFrames.push_back(m_dInitialTime + i * m_rBond.period);
            }
            double ti = *(upper_bound(timeFrames.begin(), timeFrames.end(), dX) - 1); // binary search (Ologn)
            int iplus1 = static_cast<int>(upper_bound(timeFrames.begin(), timeFrames.end(), dX) - timeFrames.begin());
            for (int i = iplus1; i < timeFrames.size(); i++)
            {
                dirtyPrice += m_rBond.rate * m_rBond.period * m_rBond.notional * m_rDiscount(timeFrames[i]) / m_rDiscount(dX);
                if (i == timeFrames.size() - 1)
                    dirtyPrice += m_rBond.notional * m_rDiscount(timeFrames.back()) / m_rDiscount(dX);
            }

            if (!m_bClean)
                return dirtyPrice;

            double accuredInterest = m_rBond.rate * (dX - ti) * m_rBond.notional;

            return dirtyPrice - accuredInterest;
        }

        bool belongs(double dX) const
        {
            return m_rDiscount.belongs(dX) && dX >= m_dInitialTime; //dX belongs to the interval [t_0, T]
        }

    private:
        cfl::Data::CashFlow m_rBond;
        cfl::Function m_rDiscount;
        double m_dInitialTime;
        bool m_bClean;
    };

    class forwardLogLinearInterpFunc : public cfl::IFunction
    {
    public:
        forwardLogLinearInterpFunc(double dSpot, const std::vector<double> &rDeliveryTimes, const std::vector<double> &rForwardPrices, double dInitialTime)
        {
            m_dSpot = dSpot;
            m_dInitialTime = dInitialTime;
            m_DT = std::vector<double>(rDeliveryTimes);
            r_FP = std::vector<double>(rForwardPrices);
            fitLinear();
        }

        void fitLinear()
        {
            m_DT.insert(m_DT.begin(), m_dInitialTime);

            std::vector<double> uLogForwardPrices(m_DT.size());
            uLogForwardPrices.front() = std::log(m_dSpot);
            std::transform(r_FP.begin(), r_FP.end(), uLogForwardPrices.begin() + 1,
                           [](double dX) { return std::log(dX); });

            cfl::Interp uLinear = cfl::NInterp::linear();
            uLogForwardPriceFunction =
                uLinear.interpolate(m_DT.begin(), m_DT.end(), uLogForwardPrices.begin());
        }

        double operator()(double dX) const
        {
            // std::cout << "time is " << dX << "y" <<  uLogForwardPriceFunction(dX) << std::endl;
            return std::exp(uLogForwardPriceFunction(dX));
        }

        bool belongs(double dX) const
        {
            return dX >= m_dInitialTime;
        }

    private:
        double m_dSpot;
        double m_dInitialTime;
        cfl::Function uLogForwardPriceFunction;
        std::vector<double> m_DT;
        std::vector<double> r_FP;
    };

    class fitVolBlackModel : public cfl::IFunction
    {
    public:
        fitVolBlackModel(const std::vector<double> &rMaturities, const std::vector<double> &rVolatilities, double dLambda, double dInitialTime)
        {
            m_rMaturities = std::vector<double>(rMaturities);
            m_rVols = std::vector<double>(rVolatilities);
            m_dLambda = dLambda;
            m_InitialTime = dInitialTime;
        }

        void fit()
        {
        }

        double operator()(double dX) const
        {
            int initialTime = m_InitialTime;

            cfl::Function uVolShape = cfl::sqrt(prb::yieldShapeHullWhite(2 * m_dLambda, m_InitialTime));
            std::vector<double> uA(m_rMaturities.size());
            std::transform(m_rMaturities.begin(), m_rMaturities.end(), uA.begin(), uVolShape);

            double dCov =
                std::inner_product(m_rVols.begin(), m_rVols.end(), uA.begin(), 0.);
            double dVar = std::inner_product(uA.begin(), uA.end(), uA.begin(), 0.);

            double dA = dCov / dVar;

            return dA * uVolShape(dX);
        }

        bool belongs(double dX) const
        {
            return dX >= m_InitialTime;
        }

    private:
        std::vector<double> m_rMaturities;
        std::vector<double> m_rVols;
        double m_dLambda;
        double m_InitialTime;
    };
} // namespace chengzhhhw1

cfl::Function prb::forwardFX(double dSpotFX, const cfl::Function &rDomesticDiscount, const cfl::Function &rForeignDiscount)
{
    return cfl::Function(new chengzhhhw1::forwardFXFunc(dSpotFX, rDomesticDiscount, rForeignDiscount));
}
// return Function(new cflData::Discount(dYield, dInitialTime));

cfl::Function prb::forwardCouponBond(const cfl::Data::CashFlow &rBond, const cfl::Function &rDiscount, double dInitialTime, bool bClean)
{
    return cfl::Function(new chengzhhhw1::forwardFXCouponFunc(rBond, rDiscount, dInitialTime, bClean));
}

cfl::Function prb::forwardLogLinearInterp(double dSpot, const std::vector<double> &rDeliveryTimes, const std::vector<double> &rForwardPrices, double dInitialTime)
{
    return cfl::Function(new chengzhhhw1::forwardLogLinearInterpFunc(dSpot, rDeliveryTimes, rForwardPrices, dInitialTime));
}

cfl::Function prb::volatilityFitBlack(const std::vector<double> &rMaturities, const std::vector<double> &rVolatilities, double dLambda, double dInitialTime)
{
    return cfl::Function(new chengzhhhw1::fitVolBlackModel(rMaturities, rVolatilities, dLambda, dInitialTime));
}

/************
 * 
 * 
 *  
 * 
 * */
using namespace cfl;
using namespace cfl::Data;

class YieldShapeHW : public cfl::IFunction
{
public:
    YieldShapeHW(double dLambda, double dInitialTime)
    {
        PRECONDITION(dLambda >= 0);
        m_dLambda = dLambda;
        m_dInitialTime = dInitialTime;
    }

    double operator()(double dT) const
    {
        PRECONDITION(belongs(dT));
        double dF = m_dLambda * (dT - m_dInitialTime);
        if (dF < cfl::c_dEps)
        {
            return 1.;
        }
        double dYield = (1. - std::exp(-dF)) / dF;
        return dYield;
    }

    bool belongs(double dT) const
    {
        return (dT >= m_dInitialTime);
    }

private:
    double m_dInitialTime, m_dLambda;
};

cfl::Function prb::
    yieldShapeHullWhite(double dLambda, double dInitialTime)
{
    return cfl::Function(new YieldShapeHW(dLambda, dInitialTime));
}