# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appAI-Girlfriend_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appAI-Girlfriend_autogen.dir\\ParseCache.txt"
  "appAI-Girlfriend_autogen"
  )
endif()
