include(FetchContent)
FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(my_tests test.cpp)

target_link_libraries(my_tests PRIVATE
	ClubManager
	Time
	FileParser
	GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(my_tests)
