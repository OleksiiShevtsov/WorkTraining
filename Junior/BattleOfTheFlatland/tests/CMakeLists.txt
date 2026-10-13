project(tests)

file(GLOB tests_SRC CONFIGURE_DEPENDS
  "*.cpp"
  "helpers/*.h"
)

if(tests_SRC)
    add_executable(${PROJECT_NAME} ${tests_SRC})
    # Game-rule tests can link server/logic when sources appear.
    target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main)
else()
    add_custom_target(${PROJECT_NAME}
        COMMENT "tests: no sources yet (add *.cpp under tests/)"
    )
endif()
