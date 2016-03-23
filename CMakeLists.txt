cmake_minimum_required(VERSION 3.2)
FIND_PACKAGE( Boost COMPONENTS interval system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

ADD_EXECUTABLE( main main.cpp )

TARGET_LINK_LIBRARIES( anyExecutable ${Boost_LIBRARIES} )