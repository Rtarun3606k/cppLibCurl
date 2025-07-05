cmake_minimum_required(VERSION 3.10)
project(StablecoinDepegTracker)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

# ✅ Use local spdlog
add_subdirectory(external/spdlog)

# Define executable
add_executable(main
    src/main.cpp
    src/PricePoller.cpp
    src/DepegRiskEngine.cpp
)

# Link libraries
find_package(CURL REQUIRED)
target_link_libraries(main
    PRIVATE
    spdlog::spdlog
    CURL::libcurl
)
