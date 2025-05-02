# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/clock_Time_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/clock_Time_autogen.dir/ParseCache.txt"
  "clock_Time_autogen"
  )
endif()
