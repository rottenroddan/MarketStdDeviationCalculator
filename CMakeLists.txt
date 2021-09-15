cmake_minimum_required(VERSION 3.17)
project(MarketStdDeviationCalculator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCURL_STATICLIB")

set(SOURCE_FILES main.cpp constants.h constants.cpp PriceHistoryEndpointCaller.cpp PriceHistoryEndpointCaller.h StdDeviationList.cpp StdDeviationList.h Ticker.cpp Ticker.h)
set(EXC_LINK_NAME "MarketStdDeviationCalculator")

include_directories(curl/libcurl-vc16-x86-debug-static-ipv6-sspi-schannel/include)

add_executable(${EXC_LINK_NAME} ${SOURCE_FILES})
target_link_libraries(MarketStdDeviationCalculator C:/Users/steve/CLionProjects/MarketStdDeviationCalculator/curl/libcurl-vc16-x86-debug-static-ipv6-sspi-schannel/lib/libcurl_a_debug.lib)

if(WIN32)
   target_link_libraries(${EXC_LINK_NAME} wldap32.lib)
   target_link_libraries(${EXC_LINK_NAME} ws2_32.lib)
   target_link_libraries(${EXC_LINK_NAME} crypt32.lib)
   target_link_libraries(${EXC_LINK_NAME} normaliz.lib)
endif()
