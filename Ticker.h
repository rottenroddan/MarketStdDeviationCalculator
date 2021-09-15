//
// Created by steve on 6/28/2021.
//

#ifndef MARKETSTDDEVIATIONCALCULATOR_TICKER_H
#define MARKETSTDDEVIATIONCALCULATOR_TICKER_H

#include <cmath>
#include <string>
#include <vector>

class Ticker {
private:
    std::string Abbv;
    std::vector<float> Candles;

    float RegresM;
    float RegresB;
    float StdDev;
    float TotalDataPoints;

    float LastCandleClosePrice;
    float LastCandleOpenPrice;

    float TotalStdDevsUnderLinearReg;

    friend class StdDeviationList;
public:
    /**
     * @brief       Default constructor for building
     *          Ticker with param ticker string.
     * @param ticker String for Ticker name.
     */
    Ticker(const std::string &ticker);

    /**
     * @brief       Calculates the StdDev for this
     *          ticker. Stores the data in this StdDev.
     */
    void CalculateLatestStdDev();

    /**
     * @brief       Compare ticker if one tickers stdDev
     *          is less than the other.
     * @return boolean based on if this is less than the right ticker.
     */
    bool operator < (const Ticker& ) const;
};


#endif //MARKETSTDDEVIATIONCALCULATOR_TICKER_H
