cmake_minimum_required (VERSION 3.0)

set (PROJECT psvdmac5-cli-lib)
project (${PROJECT})

FILE (GLOB HEADER_FILES "../Dmac5ClientAPI.h"
                        "../Dmac5Client.h"
                        "../Dmac5ClientTypes.h")
source_group ("Header Files" FILES ${HEADER_FILES})

FILE (GLOB SOURCE_FILES "../Dmac5ClientAPI.cpp"
                        "../Dmac5Client.cpp")
source_group ("Source Files" FILES ${SOURCE_FILES})

add_library (${PROJECT} STATIC ${HEADER_FILES} ${SOURCE_FILES})

install(TARGETS ${PROJECT}
        DESTINATION "lib"
        )
install(FILES ${HEADER_FILES}
        DESTINATION "include"
        )

include_directories(../)