LINK_DIRECTORIES(${OUTPUT_DIR})
SET(SRC_LIST ${MODULE_SRC} SUR_GA.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(SUR_GA SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET SUR_GA PROPERTY FOLDER "modules/hydrology_longterm")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(SUR_GA module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(SUR_GA libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()
