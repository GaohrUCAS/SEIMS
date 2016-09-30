#include <stdio.h>
#include <string>
#include "api.h"
#include "util.h"
#include "NutrientTransportSediment.h"
#include <iostream>
#include "SimulationModule.h"
#include "MetadataInfo.h"
#include "MetadataInfoConst.h"

extern "C" SEIMS_MODULE_API SimulationModule *GetInstance()
{
    return new NutrientTransportSediment();
}

//! function to return the XML Metadata document string
extern "C" SEIMS_MODULE_API const char *MetadataInformation()
{
    MetadataInfo mdi;
    mdi.SetAuthor("Huiran Gao, Liang-Jun Zhu");
    mdi.SetClass(MCLS_NUTRSED, MCLSDESC_NUTRSED);
    mdi.SetDescription(MDESC_NUTRSED);
    mdi.SetEmail(SEIMS_EMAIL);
    mdi.SetID(MDESC_NUTRSED);
    mdi.SetName(MDESC_NUTRSED);
    mdi.SetVersion("1.1");
    mdi.SetWebsite(SEIMS_SITE);
    mdi.SetHelpfile("NutrSED.html");

    // set the parameters
    mdi.AddParameter(Tag_CellWidth, UNIT_LEN_M, DESC_CellWidth, Source_ParameterDB, DT_Single);

	mdi.AddParameter(VAR_SOL_BD, UNIT_DENSITY, DESC_SOL_BD, Source_ParameterDB, DT_Raster2D);
	mdi.AddParameter(VAR_SOILDEPTH, UNIT_DEPTH_MM, DESC_SOILDEPTH, Source_ParameterDB, DT_Raster2D);
	mdi.AddParameter(VAR_SOILLAYERS, UNIT_NON_DIM, DESC_SOILLAYERS, Source_ParameterDB, DT_Raster1D);
	
	// parameters for subbasin sum
	mdi.AddParameter(VAR_SUBBSN, UNIT_NON_DIM, DESC_SUBBSN, Source_ParameterDB, DT_Raster1D);
	mdi.AddParameter(VAR_SUBBASIN_PARAM, UNIT_NON_DIM, DESC_SUBBASIN_PARAM, Source_ParameterDB, DT_Subbasin);

    // set input from other modules
	mdi.AddInput(VAR_OLFLOW, UNIT_DEPTH_MM, DESC_OLFLOW, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SEDYLD, UNIT_KG, DESC_SEDYLD, Source_Module, DT_Raster1D);

    mdi.AddInput(VAR_SOL_AORGN, UNIT_CONT_KGHA, DESC_SOL_AORGN, Source_Module, DT_Raster2D);
    mdi.AddInput(VAR_SOL_SORGN, UNIT_CONT_KGHA, DESC_SOL_SORGN, Source_Module, DT_Raster2D);    
	mdi.AddInput(VAR_SOL_FORGN, UNIT_CONT_KGHA, DESC_SOL_FORGN, Source_Module, DT_Raster2D);
    mdi.AddInput(VAR_SOL_FORGP, UNIT_CONT_KGHA, DESC_SOL_FORGP, Source_Module, DT_Raster2D);
    mdi.AddInput(VAR_SOL_HORGP, UNIT_CONT_KGHA, DESC_SOL_HORGP, Source_Module, DT_Raster2D);
    mdi.AddInput(VAR_SOL_ACTP, UNIT_CONT_KGHA, DESC_SOL_ACTP, Source_Module, DT_Raster2D);
    mdi.AddInput(VAR_SOL_STAP, UNIT_CONT_KGHA, DESC_SOL_STAP, Source_Module, DT_Raster2D);

    // set the output variables
	// output surface runoff related variables
    mdi.AddOutput(VAR_SEDORGN, UNIT_CONT_KGHA, DESC_SEDORGN, DT_Raster1D);
    mdi.AddOutput(VAR_SEDORGP, UNIT_CONT_KGHA, DESC_SEDORGP, DT_Raster1D);
    mdi.AddOutput(VAR_SEDMINPA, UNIT_CONT_KGHA, DESC_SEDMINPA, DT_Raster1D);
    mdi.AddOutput(VAR_SEDMINPS, UNIT_CONT_KGHA, DESC_SEDMINPS, DT_Raster1D);

	// to channel 
	mdi.AddOutput(VAR_SEDORGN_TOCH, UNIT_KG, DESC_SEDORGN_CH, DT_Array1D);
	mdi.AddOutput(VAR_SEDORGP_TOCH, UNIT_KG, DESC_SEDORGP_CH, DT_Array1D);
	mdi.AddOutput(VAR_SEDMINPA_TOCH, UNIT_KG, DESC_SEDMINPA_CH, DT_Array1D);
	mdi.AddOutput(VAR_SEDMINPS_TOCH, UNIT_KG, DESC_SEDMINPS_CH, DT_Array1D);

    string res = mdi.GetXMLDocument();
    char *tmp = new char[res.size() + 1];
    strprintf(tmp, res.size() + 1, "%s", res.c_str());
    return tmp;
}