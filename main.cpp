#include <stdio.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "curl/curl.h"
#include "constants.h"
#include "PriceHistoryEndpointCaller.h"
#include "StdDeviationList.h"

/* * * * * * * * -IMPORTANT- * * * * * * * *\
 * In case you can't build and you are sure *
 * you configured libcurl correctly this    *
 * entire time...                           *
 *                                          *
 * Below are dependencies that can be       *
 * uncommented out to include as libcurl    *
 * needs to link against.                   *
\* * * * * * * * * * * * * * * * * * * * * */
//#pragma comment ( lib, "wldap32.lib" )
//#pragma comment ( lib, "ws2_32.lib" )
//#pragma comment ( lib, "advapi32.lib" )
//#pragma comment ( lib, "kernel32.lib" )
//#pragma comment ( lib, "comdlg32.lib" )
//#pragma comment ( lib, "crypt32.lib" )
//#pragma comment ( lib, "normaliz.lib" )


/**
 * @brief       Attempts a fix for fixing the output file
 *          directory if an attempt was interrupted originally.
 */
void attemptFixingMissingTickers()
{
    // File streams.
    std::ifstream inputParamFile(DEFAULT_PARAMS_FILE);
    std::ifstream tickerInputFile;
    std::ofstream tickerOutputFile;

    // Get input on API parameters.
    std::string periodStr;
    std::string periodTypeStr;
    std::string frequencyStr;
    std::string frequencyTypeStr;

    // Grab params from query file.
    inputParamFile >> periodStr;
    inputParamFile >> periodTypeStr;
    inputParamFile >> frequencyStr;
    inputParamFile >> frequencyTypeStr;

    // Counters for stat stuff to display.
    unsigned brokenCounter = 0;
    unsigned fixedCounter = 0;

    // API caller along with the requests/min.
    std::ifstream keyFile(TDA_API_KEY_DIR);
    std::string apiKey;
    if(!std::getline(keyFile, apiKey))
    {
        std::cout << "Error, can't read from API Key file either because it doesn't exist or permission denied." << std::endl;
    }

    PriceHistoryEndpointCaller caller(periodStr, periodTypeStr, frequencyStr , frequencyTypeStr, apiKey);
    unsigned requestPerMinCounter = 0;

    std::cout << "Fixing files, this may take a bit." << std::endl;

    // Iterate through all files in pricehistory dir.
    for(const auto &file : std::filesystem::directory_iterator(DEFAULT_TICKER_DIR))
    {
        std::string fileContents;
        tickerInputFile.open(file.path());

        // Read full line with whitespaces.
        std::getline(tickerInputFile, fileContents);
        if(fileContents == "{\"error\":\"Not Found\"}" || fileContents == "")
        {
            if(requestPerMinCounter >= TDA_API_REQUEST_PER_MIN_LIMIT)
            {
                std::cout << "Sleeping for 60s. REQUEST LIMIT REACHED. Currently on tickerStr: " << file.path().filename().string() << std::endl;
                Sleep(60000);
                requestPerMinCounter = 0;
            }

            // Add to broken file counter.
            brokenCounter++;

            // Close input file, and open it for output.
            tickerInputFile.close();
            tickerOutputFile.open(file.path(), std::ios::trunc);

            // Call API with ticker.
            std::string tickerStr = file.path().filename().string();
            tickerOutputFile << (caller << tickerStr );

            // Close output file.
            tickerOutputFile.close();

            // Open file to check again error not found or empty.
            tickerInputFile.open(file.path());

            // Read full line with whitespaces one last time to update counters.
            std::getline(tickerInputFile, fileContents);
            if(fileContents != "{\"error\":\"Not Found\"}" && fileContents != "")
                fixedCounter++;

            // Increment request counter.
            requestPerMinCounter++;
        }

        // close file.
        tickerInputFile.close();
    }

    std::cout << "Total Files Missing Data: " << brokenCounter << std::endl;
    std::cout << "Total Files Fixed:        " << fixedCounter << std::endl;
    std::cout << "[NOTE] Some Tickers as part of the Tickets.txt file may be missing from TDA API as they might not be tradeable on their platform." << std::endl;

    inputParamFile.close();
}

/**
 * @brief       Updates the stock directory with fresh data. Provided
 *          the user enters what they want the new tickers to have in terms
 *          of period/frequency params for the api calls. After gathering,
 *          uses the PriceHistoryEndpointCaller class to gather ticker info
 *          from the TDA endpoint. Saves the provided information into the
 *          valid ticker file.
 */
void updateStockPriceHistory()
{
    // Get input on API parameters.
    std::string periodStr;
    std::string periodTypeStr;
    std::string frequencyStr;
    std::string frequencyTypeStr;

    bool dataIsCorrectFormat = false;
    while(dataIsCorrectFormat == false)
    {
        std::cout << "Enter Period Type[day, month, year, ytd]: ";
        std::getline(std::cin, periodTypeStr);

        std::cout << "Enter Period: ";
        std::getline(std::cin, periodStr);

        std::cout << "Enter Frequency Type[minute, daily, weekly, monthly]: ";
        std::getline(std::cin, frequencyTypeStr);

        std::cout << "Enter Frequency: ";
        std::getline(std::cin, frequencyStr);

        // Check params for query.
        dataIsCorrectFormat = PriceHistoryEndpointCaller::checkQueryParameters(periodStr, periodTypeStr, frequencyStr, frequencyTypeStr);
    }

    /*
     * First Save data to queryparams.txt
     */
    std::ofstream params(DEFAULT_PARAMS_FILE, std::ios::trunc);
    params << periodStr << " " << periodTypeStr << " " << frequencyStr << " " << frequencyTypeStr;
    params.close();

    std::ifstream inputTickerListFile(DEFAULT_TICKER_FILE);
    std::ofstream outputTickerFile;
    std::string ticker;

    // API caller along with the requests/min.
    std::ifstream keyFile(TDA_API_KEY_DIR);
    std::string apiKey;
    if(!std::getline(keyFile, apiKey))
    {
        std::cout << "Error, can't read from API Key file either because it doesn't exist or permission denied." << std::endl;
    }

    PriceHistoryEndpointCaller caller(periodStr, periodTypeStr, frequencyStr , frequencyTypeStr, apiKey );
    unsigned requestPerMinCounter = 0;

    while(inputTickerListFile >> ticker)
    {
        // Make sure we can make requests after.
        if(requestPerMinCounter >= TDA_API_REQUEST_PER_MIN_LIMIT)
        {
            // Damn you....
            std::cout << "Sleeping for a minute. " << std::endl;
            Sleep(60000);
            requestPerMinCounter = 0;
        }

        std::cout << "Collecting: " << ticker << std::endl;
        outputTickerFile.open(DEFAULT_TICKER_DIR + "/" + ticker, std::ios::trunc);

        outputTickerFile << (caller << ticker);

        // close file ready for next.
        outputTickerFile.close();
        // increment request/min counter.
        requestPerMinCounter++;
    }

    // close input file.
    inputTickerListFile.close();
}

/**
 * @brief       Function used to calculate stdDev from the
 *          StdDeviationList class. Gets user input on how
 *          many tickers they would like displayed, then
 *          uses the object from above to generate the list
 *          of top results.
 */
void calculate()
{
    StdDeviationList stdDev;
    bool goodData = false;
    std::string input;

    while(goodData == false)
    {
        std::cout << "Enter How Many Tickers you'd like to display: ";
        std::cin >> input;

        std::string::const_iterator it = input.begin();
        while(it != input.end() && std::isdigit(*it))
            ++it;

        // If all checks out.
        if(!input.empty() && it == input.end())
            goodData = true;
    }

    stdDev.GenerateList(std::stoul(input));
}

/**
 * @brief       Main, guides the used to what they want to
 *          do as far as updating ticker information, or
 *          attempting a fix in tickers, or calculating the
 *          stdDev.
 * @return
 */
int main() {
    bool exit_condition = false;
    std::string input;

    // Main loop.
    while(exit_condition == false)
    {
        std::cout   << " --Options--" << std::endl
                    << "1.  Update Current Price History." << std::endl
                    << "1a. Fix Potential Missing Tickers." << std::endl
                    << "2.  Calculate and Show Top Results." << std::endl
                    << "Type \"exit\" To exit. "
                    << " Enter input: " << std::flush;
        std::getline(std::cin, input);

        if(input.compare("1") == 0)
        {
            updateStockPriceHistory();
        }
        else if(input == "1a")
        {
            attemptFixingMissingTickers();
        }
        else if(input.compare("2") == 0)
        {
            calculate();
        }
        else if(input.compare("exit") == 0)
        {
            // exit the loop.
            exit_condition = true;
        }

    }
    return 0;
}
