//
// Created by steve on 6/28/2021.
//

#ifndef MARKETSTDDEVIATIONCALCULATOR_STDDEVIATIONLIST_H
#define MARKETSTDDEVIATIONCALCULATOR_STDDEVIATIONLIST_H


#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <Windows.h>
#include <vector>

#include "constants.h"
#include "Ticker.h"



class StdDeviationList {
private:
    /// List of tickers.
    std::vector<Ticker> Tickers;

    /**
     * @brief       Loads data from the provided ticker
     *          files in the default dir. Populates this
     *          Tickers vector with Ticker objects that
     *          represent the close and open prices of that
     *          ticker. Used just to prep the data before
     *          calculations.
     */
    void loadFileIntoList();

    /**
     * @brief       Calculates the stdDeviation in each
     *          element of Tickers by calling Tickers
     *          method for stdDev.
     */
    void calculateStdDev();


public:
    /**
     * @brief       Method used for outputting to cout
     *          about the top userInput param stdDev
     *          results.
     *
     * @param userInput total amount
     */
    void GenerateList(unsigned userInput);
};


#endif //MARKETSTDDEVIATIONCALCULATOR_STDDEVIATIONLIST_H
