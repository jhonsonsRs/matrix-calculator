# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/MatrixCalculator_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/MatrixCalculator_autogen.dir/ParseCache.txt"
  "MatrixCalculator_autogen"
  )
endif()
