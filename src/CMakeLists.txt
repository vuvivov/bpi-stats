add_library (src app.h dataRetriever.h priceDatabase.h printer.h statistic.h app.cpp dataRetriever.cpp priceDatabase.cpp printer.cpp statistic.cpp)
target_include_directories (src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIRS})
target_link_libraries(src ${Boost_LIBRARIES})
