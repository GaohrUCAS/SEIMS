LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} NutrOL_IKW.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(NutrOL_IKW SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET NutrOL_IKW PROPERTY FOLDER "modules/nutrient")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(NutrOL_IKW module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(NutrOL_IKW libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()