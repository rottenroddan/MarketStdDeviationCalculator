//
// Created by steve on 6/28/2021.
//

#include "StdDeviationList.h"


#ifdef WIN32
/**
 * \brief       Used in print root methods. That way the key shows
 *          what the color being printed is. Mostly for debug purpose.
 *
 * @param s String to be colored in std::cout
 * @param color Int version of the color to use for std::cout. HEX.
 *
 * \note        This is windows only function.
 */
void printInColor(std::string &&s, int color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD oldColor;
    CONSOLE_SCREEN_BUFFER_INFO csbiData;

    GetConsoleScreenBufferInfo(hConsole, &csbiData);
    oldColor = csbiData.wAttributes;

    SetConsoleTextAttribute(hConsole, color);
    std::cout << s;

    SetConsoleTextAttribute(hConsole, oldColor);
}
#elif __unix__
void printInColor(std::string &&s, int color)
{
    /*
     * No color yet :/
     */
    std::cout << s;
}
#endif

void StdDeviationList::loadFileIntoList()
{
    std::ifstream tickerFile;

    // For each file in the ticker dir.
    for(const auto &file : std::filesystem::directory_iterator(DEFAULT_TICKER_DIR))
    {
        std::string fileText;
        tickerFile.open(file);

        if(tickerFile.is_open())
        {
            // Create new Ticker Object.
            Tickers.push_back(Ticker(file.path().filename().string()));

            // Store file in str.
            std::getline(tickerFile, fileText);

            // Read the entire file string. Look for close prices that way we can store that data a calculate off of it later.
            for(size_t offset = fileText.find("close"); offset != std::string::npos; offset = fileText.find("close", offset + 5))
            {
                // find price now.
                std::string strFloat;

                // Get substring of close price. Convert to actual float now.
                strFloat = fileText.substr(offset + 7, fileText.find(",", offset) - (offset + 7));
                float closePrice = std::stof(strFloat);

                // Add prices to Candles in this current ticker object. Also set last candle price.
                (Tickers.end()-1)->Candles.push_back(closePrice);
                (Tickers.end()-1)->LastCandleClosePrice = closePrice;
            }

            // Look for last open price data.
            for(size_t offset = fileText.find("open"); offset != std::string::npos; offset = fileText.find("open", offset + 4))
            {
                std::string openFloatStr;

                openFloatStr = fileText.substr(offset + 6, fileText.find(",", offset) - (offset + 6));
                float openPrice = std::stof(openFloatStr);
                (Tickers.end()-1)->LastCandleOpenPrice = openPrice;
            }
        }

        tickerFile.close();
    }
}

void StdDeviationList::calculateStdDev()
{
    // Calculate all Tickers Deviation
    for(auto & Ticker : this->Tickers)
    {
        Ticker.CalculateLatestStdDev();
    }

    // Using operator < from Ticker class.
    std::sort(this->Tickers.begin(), this->Tickers.end());
}

void StdDeviationList::GenerateList(unsigned userInput) {

    // Load data into the list.
    loadFileIntoList();

    // Calculate and sort the elements of this->Tickers.
    calculateStdDev();

    // Display data now.
    std::cout <<"[Ticker]------StdDev-------Open-Close----" << std::endl;
    //          .[FOA]     :  5.074263  :                :
    // Print Data loop of top results
    for(unsigned i = 0; (i < userInput && i < this->Tickers.size()); i++)
    {
        std::stringstream openStream;
        std::stringstream closeStream;

        openStream << std::setprecision(2) << std::fixed << Tickers[i].LastCandleOpenPrice;
        closeStream << std::setprecision(2) << std::fixed << Tickers[i].LastCandleClosePrice;


        std::cout << " " << std::left << std::setw(8);
        printInColor( "[" + std::string(Tickers[i].Abbv) +"]", 0xF);
        std::cout << "  :  " << std::right << std::setprecision(5) << std::fixed;
        printInColor(std::to_string(Tickers[i].TotalStdDevsUnderLinearReg), (i > (userInput / 2) ? 0x2 : 0xA));
        std::cout << "  :" << std::setw(14) << std::setprecision(2) << std::fixed;
        printInColor(openStream.str() + "-" + closeStream.str(), (Tickers[i].LastCandleClosePrice > Tickers[i].LastCandleOpenPrice ? 0xA : 0x4));
        std::cout << std::endl;
    }
}
