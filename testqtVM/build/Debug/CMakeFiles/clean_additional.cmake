# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/HelloWorldQt_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/HelloWorldQt_autogen.dir/ParseCache.txt"
  "HelloWorldQt_autogen"
  )
endif()
