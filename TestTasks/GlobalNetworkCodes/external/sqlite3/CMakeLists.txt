project(sqlite3)

set(SQLITE3_DIRECTORY "${CMAKE_SOURCE_DIR}/external/sqlite3")

set(SQLITE_VERSION "3460100")
set(ZIP_URL "https://www.sqlite.org/2024/sqlite-amalgamation-${SQLITE_VERSION}.zip")
set(ZIP_OUTPUT "${SQLITE3_DIRECTORY}/sqlite-amalgamation-${SQLITE_VERSION}.zip")

if(EXISTS ${ZIP_OUTPUT})
    message(STATUS "Zip file already exists at ${ZIP_OUTPUT}.")
else()
    message(STATUS "Downloading zip file from ${ZIP_URL}")

    file(DOWNLOAD
        ${ZIP_URL}
        ${ZIP_OUTPUT}
        SHOW_PROGRESS
    )

    message(STATUS "Unpacking the zip file to ${SQLITE3_DIRECTORY}")

    file(ARCHIVE_EXTRACT
        INPUT ${ZIP_OUTPUT}
        DESTINATION ${SQLITE3_DIRECTORY}
    )
endif()

add_library(${PROJECT_NAME} STATIC ${SQLITE3_DIRECTORY}/sqlite-amalgamation-${SQLITE_VERSION}/sqlite3.c)
target_include_directories(${PROJECT_NAME} INTERFACE  ${SQLITE3_DIRECTORY}/sqlite-amalgamation-${SQLITE_VERSION})