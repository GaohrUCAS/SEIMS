LINK_DIRECTORIES(${PROJECT_BINARY_DIR})
SET(SRC_LIST ${MODULE_SRC} NutrCH_QUAL2E.cpp api.cpp)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
ADD_LIBRARY(NUTRCH_QUAL2E SHARED ${SRC_LIST})
IF (MSVC)
    SET_PROPERTY(TARGET NUTRCH_QUAL2E PROPERTY FOLDER "modules/nutrient")
ENDIF ()
IF (WIN32)
    TARGET_LINK_LIBRARIES(NUTRCH_QUAL2E module_setting bmps data util)
ELSE ()
    TARGET_LINK_LIBRARIES(NUTRCH_QUAL2E libmodule_setting.a bmps.a libdata.a libutil.a)
ENDIF ()