current page		 - 505 
( 547 - 566 )
______________________________
installation gdb	 - 82
installation GDB and GCC - 66

**** connecting googletest to the project *****
	* download project from github repository https://github.com/google/googletest
	then build him. In CMakeLists.txt of your project write the following:
	
	add_subdirectory(googletest) ** OR ** add_subdirectory(path_to_googletest_directory ${CMAKE_CURRENT_BINARY_DIR}/googletest) 
	target_link_libraries(${PROJECT_NAME} PUBLIC gtest gtest_main gmock)
	
**** boost ****
	* download project from github repository https://www.boost.org/users/download/
	library already built. In CMakeLists.txt of your project write the following:
	
	set(Boost_INCLUDE_DIRS path_to_boost_directory)
	include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

**** NEXT ****
	
	

	
	
