INCLUDE_DIRECTORIES(${VERLIHUB_BINARY_DIR} ${VERLIHUB_SOURCE_DIR}/src)

SET(PLUGMAN_SRCS cpiplug.cpp cplugs.cpp cconsole.cpp)
SET(PLUGMAN_HDRS cpiplug.h cplugs.h cconsole.h )

ADD_LIBRARY(plug_pi MODULE ${PLUGMAN_SRCS})
target_link_libraries(plug_pi libverlihub)
INSTALL(TARGETS plug_pi LIBRARY DESTINATION lib)
