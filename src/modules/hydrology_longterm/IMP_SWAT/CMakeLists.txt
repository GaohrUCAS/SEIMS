LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} pothole_SWAT.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(IMP_SWAT SHARED ${SRC_LIST})
IF (WIN32)
    TARGET_LINK_LIBRARIES(IMP_SWAT module_setting bmps data util)
    IF (MSVC)
        SET_PROPERTY(TARGET IMP_SWAT PROPERTY FOLDER "modules/hydrology_longterm")
    ENDIF ()
ELSE ()
    TARGET_LINK_LIBRARIES(IMP_SWAT libmodule_setting.so libbmps.so libdata.so libutil.so)
ENDIF ()
