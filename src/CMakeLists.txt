# output settings
set(LIBRARY_OUTPUT_FILENAME ${CMAKE_PROJECT_NAME}-${PROJECT_VERSION})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

if(BUILD_NUMBER GREATER -1)
   set(LIBRARY_OUTPUT_FILENAME ${LIBRARY_OUTPUT_FILENAME}.${BUILD_NUMBER})
endif()

if(NOT RELEASE)
   set(LIBRARY_OUTPUT_FILENAME ${LIBRARY_OUTPUT_FILENAME}-dev)
endif()

# source files
set(PROJECT_SOURCES
   Connection.cpp
   NetworkService.cpp
   Server.cpp
   TCPConnection.cpp
   TCPServer.cpp
   TransportProtocol.cpp
   UDPServer.cpp
   )

# compile library
add_library(${LIBRARY_OUTPUT_FILENAME} SHARED ${PROJECT_SOURCES})

# link dependencies
target_link_libraries(${LIBRARY_OUTPUT_FILENAME} ${Boost_LIBRARIES})
