//
// Created by steve on 6/25/2021.
//
#include "constants.h"

const std::string TDA_PRICE_HISTORY_ENDPOINT    = "https://api.tdameritrade.com/v1/marketdata/";
const std::string TDA_PRICE_HISTORY_PARAM_BEGIN = "/pricehistory?";
const unsigned    TDA_API_REQUEST_PER_MIN_LIMIT = 120;

const std::string TDA_API_KEY_DIR = "../apikey.txt";

const std::string DEFAULT_PERIOD = "6";
const std::string DEFAULT_PERIOD_TYPE = "month";
const std::string DEFAULT_FREQUENCY = "1";
const std::string DEFAULT_FREQUENCY_TYPE = "daily";
const std::string DEFAULT_TICKER_FILE = "../tickers.txt";
const std::string DEFAULT_TICKER_DIR = "../pricehistory";
const std::string DEFAULT_PARAMS_FILE = "../queryparams.txt";