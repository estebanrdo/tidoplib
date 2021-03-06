message(STATUS "\nOperating system info:\n")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  message(STATUS "  Linux ${CMAKE_SYSTEM_VERSION}\n")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  message(STATUS "  macOS ${CMAKE_SYSTEM_VERSION}\n")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  message(STATUS "  Windows ${CMAKE_SYSTEM_VERSION}\n")
elseif(CMAKE_SYSTEM_NAME STREQUAL "AIX")
  message(STATUS "  IBM AIX ${CMAKE_SYSTEM_VERSION}\n")
else()
  message(STATUS "  ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}\n")
endif()