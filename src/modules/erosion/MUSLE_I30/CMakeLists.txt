LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} MUSLE_I30.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(MUSLE_I30 SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET MUSLE_I30 PROPERTY FOLDER "modules/erosion")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(MUSLE_I30 module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(MUSLE_I30 libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()