LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} PER_PI.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(PER_PI SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET PER_PI PROPERTY FOLDER "modules/hydrology_longterm")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(PER_PI module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(PER_PI libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()