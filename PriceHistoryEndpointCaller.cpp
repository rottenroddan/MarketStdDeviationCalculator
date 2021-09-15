//
// Created by steve on 6/25/2021.
//

#include "PriceHistoryEndpointCaller.h"

PriceHistoryEndpointCaller::PriceHistoryEndpointCaller(const std::string &period,       const std::string &periodType,
                                                       const std::string &frequency,    const std::string &frequencyType,
                                                       const std::string &apiKey,       const std::string &&endDate,
                                                       const std::string &&startDate,   const std::string &&extendedHours)
{
    // Essential
    this->PeriodStr         = period;
    this->PeriodTypeStr     = periodType;
    this->FrequencyStr      = frequency;
    this->FrequencyTypeStr  = frequencyType;
    this->APIKeyStr         = apiKey;

    // Non-essential
    this->EndDateStr        = endDate;
    this->StartDateStr      = startDate;
    this->ExtendedHoursStr  = extendedHours;
}

int helperGetRequestToString(char *data, size_t size, size_t nmemb, std::string *buffer)
{
    int result = 0;
    if(buffer != nullptr) {
        buffer->append(data, size * nmemb);
        result = size * nmemb;
    }
    return result;
}

std::string PriceHistoryEndpointCaller::operator<<(const std::string &_right) {
    // build url.
    std::string urlStr = TDA_PRICE_HISTORY_ENDPOINT;

    // https://api.tdameritrade.com/v1/marketdata/{ticker}
    urlStr.append(_right);

    // https://api.tdameritrade.com/v1/marketdata/{ticker}/pricehistory?
    urlStr.append(TDA_PRICE_HISTORY_PARAM_BEGIN);

    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&
    urlStr.append("apikey=");
    urlStr.append(this->APIKeyStr);
    urlStr.append("&");

    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&periodType=month&
    urlStr.append("periodType=");
    urlStr.append(this->PeriodTypeStr);
    urlStr.append("&");

    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&periodType=month&period=6&
    urlStr.append("period=");
    urlStr.append(this->PeriodStr);
    urlStr.append("&");

    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&periodType=month&period=6&frequencyType=daily&
    urlStr.append("frequencyType=");
    urlStr.append(this->FrequencyTypeStr);
    urlStr.append("&");


    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&periodType=month&period=6&frequencyType=daily&frequency=1&
    urlStr.append("frequency=");
    urlStr.append(this->FrequencyStr);
    urlStr.append("&");

    // https://api.tdameritrade.com/v1/marketdata/AAPL/pricehistory?apikey=RANDOM_KEY_HERE&periodType=month&period=6&frequencyType=daily&frequency=1&needExtendedHoursData=false"
    urlStr.append("needExtendedHoursData=");
    urlStr.append(this->ExtendedHoursStr);
    //dont append ampersand this time.


    // Initialize CURL object.
    CURL *curl;
    curl = curl_easy_init();

    // String to hold response from API.
    std::string buff;

    //
    if(curl) {
        // Built URL
        curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());

        // Ignore Header of the content.
        curl_easy_setopt(curl, CURLOPT_HEADER, 0);
        // Stay on page.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0);
        // Write to buff string. Using helper function pointer.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, helperGetRequestToString);
        // Address of where to store content in.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buff);
    }
    else {
        std::cout << "Curl Request Failed for ticker: " << _right << std::endl;
        curl_easy_cleanup(curl);
        return nullptr;
    }

    // Perform request.
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return buff;
}

bool PriceHistoryEndpointCaller::checkQueryParameters(const std::string& period, const std::string& periodType, const std::string& frequency, const std::string& frequencyType)
{
    bool status = true;
    if(periodType.compare("day") == 0)
    {
        if(!(period.compare("1") || period.compare("2") || period.compare("3") || period.compare("4") || period.compare("5") || period.compare("10")))
        {
            status = false;
            std::cout << "[Input Error] Period provided is not correct format for daily. You provided: " << period << ", while accepted for daily type is: 1, 2, 3, 4, 5, 10." << std::endl;
        }

        if(frequencyType != "minute")
        {
            status = false;
            std::cout << "[Input Error] Frequency Type must be set to minute for a daily chart." << std::endl;
        }
    }
    else if(periodType.compare("month") == 0)
    {
        if(!(period.compare("1") || period.compare("2") || period.compare("3") || period.compare("6")))
        {
            status = false;
            std::cout << "[Input Error] Period provided is not correct format for month. You provided: " << period << ", while accepted for month type is: 1, 2, 3, 6." << std::endl;
        }

        if(frequencyType != "daily" && frequencyType != "weekly")
        {
            status = false;
            std::cout << "[Input Error] Frequency Type must be set to daily or weekly for a monthly chart." << std::endl;
        }
    }
    else if(periodType.compare("year") == 0)
    {
        if(!(period.compare("1") || period.compare("2") || period.compare("3") || period.compare("5") || period.compare("10") || period.compare("15") || period.compare("20")))
        {
            status = false;
            std::cout << "[Input Error] Period provided is not correct format for year. You provided: " << period << ", while accepted for year type is: 1, 2, 3, 5, 10, 15, 20." << std::endl;
        }

        if(frequencyType != "daily" && frequencyType != "weekly" && frequencyType != "monthly")
        {
            status = false;
            std::cout << "[Input Error] Frequency Type must be set to daily, weekly, or monthly for a yearly chart." << std::endl;
        }
    }
    else if(periodType.compare("ytd") == 0)
    {
        if(!(period.compare("1") == 0))
        {
            status = false;
            std::cout << "[Input Error] Period provided is not correct format for ytd. You provided: " << period << ", while accepted for year type is: 1." << std::endl;
        }

        if(frequencyType != "daily" && frequencyType != "weekly")
        {
            status = false;
            std::cout << "[Input Error] Frequency Type must be set to daily or weekly for a ytd chart." << std::endl;
        }
    }
    else
    {
        status = false;
        std::cout << "[Input Error] Period Type is incorrect param. You provided: " << periodType << ", while accepted is: day, month, year, ytd." << std::endl;
    }

    // Return.
    return status;
}
