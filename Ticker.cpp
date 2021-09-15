//
// Created by steve on 6/28/2021.
//

#include "Ticker.h"

Ticker::Ticker(const std::string &ticker) {
    this->Abbv = ticker;
}

void Ticker::CalculateLatestStdDev()
{
    // Regression
    float sumX = 0;
    float sumY = 0;
    float sumXY = 0;
    float sumX2 = 0;
    float squareSumX2 = 0;

    // Std Deviation
    float yBar = 0;
    float deviationSum = 0;
    float stdDev = 0;

    // Total Data points.
    float N = this->Candles.size();

    // Main loop!
    for(unsigned i = 0; i < N; i++)
    {
        // Regression
        sumX += i;
        sumY += this->Candles[i];
        sumXY += i * this->Candles[i];
        sumX2 += i*i;

        // Deviation
        yBar += this->Candles[i];
    }

    // Regression
    float m = N * sumXY - sumX * sumY;
    m = m / (N * sumX2 - sumX * sumX);

    float b = (sumY - m * sumX)/N;
    //std::cout << "y = " << m << "x + " << b << std::endl;

    // Deviation
    yBar /= N;

    for(unsigned i = 0; i < N; i++)
    {
        deviationSum += (this->Candles[i] - yBar) * (this->Candles[i] - yBar);
    }
    stdDev = std::sqrt(deviationSum / (N - 1));

    // Set this object in case we ever want to use again.
    this->RegresM = m;
    this->RegresB = b;
    this->StdDev  = stdDev;
    this->TotalDataPoints = N;

    // Positive is Under, negative is over the linear reg.
    this->TotalStdDevsUnderLinearReg = ( (m * N + b) - this->LastCandleClosePrice) / stdDev;
}

bool Ticker::operator<(const Ticker &right) const {
    return this->TotalStdDevsUnderLinearReg > right.TotalStdDevsUnderLinearReg;
}