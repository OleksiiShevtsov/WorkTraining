project(notifications)

file(GLOB notifications_SRC CONFIGURE_DEPENDS
  "include/notifications/*.h"
  "include/notifications/custom_notifications/*.h"
  "src/*.cpp"
  "src/custom_notifications/*.cpp"
)

add_library(${PROJECT_NAME} STATIC ${notifications_SRC})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    include
)

find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    Threads::Threads
    logic
  PUBLIC
    proto_messages
    common
)

# ”кажите путь к vcpkg toolchain file

set(WEB_SOCKETPP_PATH "../../../../LIB/websocketpp")
include_directories(${WEB_SOCKETPP_PATH})

set(Boost_INCLUDE_DIRS ../../../../LIB/boost/boost_1_82_0)
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

#add_subdirectory(${CMAKE_SOURCE_DIR}/../common/protobuf ${CMAKE_BINARY_DIR}/notifications/protobuf/output/)

#target_compile_options(notifications PRIVATE -Wall -Wextra -Wpedantic -Werror)
