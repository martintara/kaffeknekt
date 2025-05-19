# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SocketData_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SocketData_autogen.dir/ParseCache.txt"
  "SocketData_autogen"
  )
endif()
