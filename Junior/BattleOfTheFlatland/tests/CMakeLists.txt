project(tests)

file(GLOB tests_SRC CONFIGURE_DEPENDS
  "*.cpp"
  "helpers/*.h"
)

add_executable(${PROJECT_NAME} ${tests_SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main PRIVATE logic)
