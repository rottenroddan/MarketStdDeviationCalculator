//
// Created by steve on 6/25/2021.
//

#ifndef MARKETSTDDEVIATIONCALCULATOR_PRICEHISTORYENDPOINTCALLER_H
#define MARKETSTDDEVIATIONCALCULATOR_PRICEHISTORYENDPOINTCALLER_H
#include <iostream>
#include <string>
#include "constants.h"
#include "curl/curl.h"

class PriceHistoryEndpointCaller {
private:
    // Below must be set. Essential to the API call.
    std::string PeriodStr;
    std::string PeriodTypeStr;
    std::string FrequencyStr;
    std::string FrequencyTypeStr;

    std::string EndDateStr;
    std::string StartDateStr;

    std::string ExtendedHoursStr;

    std::string APIKeyStr;
public:
    /**
     * @brief       Constructor for making the api call to TDA
     *          for the given time stats.
     *
     * @param period String of period.
     * @param periodType String of period type.
     * @param frequency String of frequency.
     * @param frequencyType String of frequency type.
     * @param apiKey String of ApiKey for TDA.
     * @param endDate String of Last date to gather.
     * @param startDate String of Start date.
     * @param extendedHours String of true or false for extended hours.
     */
    PriceHistoryEndpointCaller(const std::string& period,
                               const std::string& periodType,
                               const std::string& frequency,
                               const std::string& frequencyType,
                               const std::string& apiKey,
                               const std::string&& endDate = "",
                               const std::string&& startDate = "",
                               const std::string&& extendedHours = "false");

    /**
     * @brief       Operator that takes in a string argument
     *          which is a Ticker that is to be saved as a
     *          file from the results of the API call.
     *
     * @param _right String of Ticker file for the API call.
     * @return String of the output from the API call.
     */
    std::string operator<<(const std::string& _right);

    /**
     * @brief       Checks if the params provided are
     *          all valid api param values.
     *
     * @param period String of period val.
     * @param periodType String of period type.
     * @param frequency String of frequency val.
     * @param frequencyType String of frequency type.
     * @return Boolean true if all params are good. False else.
     */
    static bool checkQueryParameters(const std::string& period, const std::string& periodType, const std::string& frequency, const std::string& frequencyType);
};


#endif //MARKETSTDDEVIATIONCALCULATOR_PRICEHISTORYENDPOINTCALLER_H
