LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} IUH_NUTR_OL.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(IUH_NUTR_OL SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET IUH_NUTR_OL PROPERTY FOLDER "modules/nutrient")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(IUH_NUTR_OL module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(IUH_NUTR_OL libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()