LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} SUR_CN.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(SUR_CN SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET SUR_CN PROPERTY FOLDER "modules/hydrology_longterm")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(SUR_CN module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(SUR_CN libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()