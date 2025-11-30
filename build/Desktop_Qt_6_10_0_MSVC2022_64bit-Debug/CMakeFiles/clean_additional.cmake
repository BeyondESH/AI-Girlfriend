# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appAIGirlFriend_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appAIGirlFriend_autogen.dir\\ParseCache.txt"
  "appAIGirlFriend_autogen"
  )
endif()
