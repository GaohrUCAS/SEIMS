#include <stdio.h>
#include <string>
#include "util.h"
#include "api.h"
#include "pothole_SWAT.h"
#include <iostream>
#include "SimulationModule.h"
#include "MetadataInfo.h"
#include "MetadataInfoConst.h"

extern "C" SEIMS_MODULE_API SimulationModule *GetInstance()
{
    return new IMP_SWAT();
}

/// function to return the XML Metadata document string
extern "C" SEIMS_MODULE_API const char *MetadataInformation()
{
    MetadataInfo mdi;
    string res;

    mdi.SetAuthor("Liang-Jun Zhu");
    mdi.SetClass(MCLS_PADDY, MCLSDESC_PADDY);
    mdi.SetDescription(MDESC_IMP_SWAT);
    mdi.SetID(MID_IMP_SWAT);
    mdi.SetName(MID_IMP_SWAT);
    mdi.SetVersion("1.0");
    mdi.SetEmail(SEIMS_EMAIL);
    mdi.SetWebsite(SEIMS_SITE);
    mdi.SetHelpfile("");
    /// set parameters from database
	mdi.AddParameter(Tag_ROUTING_LAYERS, UNIT_NON_DIM, DESC_ROUTING_LAYERS, Source_ParameterDB, DT_Array2D);
	mdi.AddParameter(Tag_CellWidth, UNIT_LEN_M, DESC_CellWidth, Source_ParameterDB, DT_Single);

    mdi.AddParameter(VAR_EVLAI, UNIT_AREA_RATIO, DESC_EVLAI, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_POT_TILEMM, UNIT_DEPTH_MM, DESC_POT_TILEMM, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_POT_NO3DECAY, UNIT_PER_DAY, DESC_POT_NO3DECAY, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_POT_SOLPDECAY, UNIT_PER_DAY, DESC_POT_SOLPDECAY, Source_ParameterDB, DT_Single);
	mdi.AddParameter(VAR_SLOPE, UNIT_PERCENT, DESC_SLOPE, Source_ParameterDB, DT_Raster1D);
	mdi.AddParameter(VAR_CONDUCT, UNIT_WTRDLT_MMH, DESC_CONDUCT, Source_ParameterDB, DT_Raster2D);
	mdi.AddParameter(VAR_SOILLAYERS, UNIT_NON_DIM, DESC_SOILLAYERS, Source_ParameterDB, DT_Raster1D);
	mdi.AddParameter(VAR_SOL_SUMAWC, UNIT_DEPTH_MM, DESC_SOL_SUMAWC, Source_ParameterDB, DT_Raster1D);
	mdi.AddParameter(VAR_SOL_AWC, UNIT_DEPTH_MM, DESC_SOL_AWC, Source_ParameterDB, DT_Raster2D);
	mdi.AddParameter(VAR_SOILTHICK, UNIT_DEPTH_MM, DESC_SOILTHICK, Source_ParameterDB, DT_Raster2D);
	mdi.AddParameter(VAR_POROST, UNIT_VOL_FRA_M3M3, DESC_POROST, Source_ParameterDB, DT_Raster2D);
    /// set input from other modules
	mdi.AddInput(VAR_IMPOUND_TRIG, UNIT_NON_DIM, DESC_IMPOUND_TRIG, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_POT_VOLMAXMM, UNIT_DEPTH_MM, DESC_POT_VOLMAXMM, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_POT_VOLLOWMM, UNIT_DEPTH_MM, DESC_POT_VOLLOWMM, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_NEPR, UNIT_DEPTH_MM, DESC_NEPR, Source_Module, DT_Raster1D);// m_pNet
    mdi.AddInput(VAR_LAIDAY, UNIT_AREA_RATIO, DESC_LAIDAY, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_PET, UNIT_DEPTH_MM, DESC_PET, Source_Module, DT_Raster1D); ///PET
	mdi.AddInput(VAR_SURU, UNIT_DEPTH_MM, DESC_SURU, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SOER, UNIT_KG, DESC_SOER, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SANDYLD, UNIT_KG, DESC_SANDYLD, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SILTYLD, UNIT_KG, DESC_SILTYLD, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_CLAYYLD, UNIT_KG, DESC_CLAYYLD, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SAGYLD, UNIT_KG, DESC_SAGYLD, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_LAGYLD, UNIT_KG, DESC_LAGYLD, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SOL_ST, UNIT_DEPTH_MM, DESC_SOL_ST, Source_Module, DT_Raster2D);
	mdi.AddInput(VAR_SOL_SW, UNIT_DEPTH_MM, DESC_SOL_SW, Source_Module, DT_Raster1D); /// sol_sw in SWAT
	mdi.AddInput(VAR_SUR_NO3, UNIT_CONT_KGHA, DESC_SUR_NO3, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SUR_SOLP, UNIT_CONT_KGHA, DESC_SUR_SOLP, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SEDORGN, UNIT_CONT_KGHA, DESC_SEDORGN, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SEDORGP, UNIT_CONT_KGHA, DESC_SEDORGP, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SEDMINPA, UNIT_CONT_KGHA, DESC_SEDMINPA, Source_Module, DT_Raster1D);
	mdi.AddInput(VAR_SEDMINPS, UNIT_CONT_KGHA, DESC_SEDMINPS, Source_Module, DT_Raster1D);
    /// set the output variables
	//mdi.AddOutput(VAR_POT_NO3, UNIT_KG, DESC_POT_NO3, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_ORGN, UNIT_KG, DESC_POT_ORGN, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_SOLPLOSS, UNIT_PER_DAY, DESC_POT_SOLPLOSS, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_ORGP, UNIT_KG, DESC_POT_ORGP, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_AMINP, UNIT_KG, DESC_POT_AMINP, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_SMINP, UNIT_KG, DESC_POT_SMINP, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_SED, UNIT_KG, DESC_POT_SED, DT_Raster1D);
	mdi.AddOutput(VAR_POT_VOL, UNIT_DEPTH_MM, DESC_POT_VOL, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_FLOWIN, UNIT_VOL_M3, DESC_POT_FLOWIN, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_FLOWOUT, UNIT_VOL_M3, DESC_POT_FLOWOUT, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_SEDIN, UNIT_KG, DESC_POT_SEDIN, DT_Raster1D);
	//mdi.AddOutput(VAR_POT_SEDOUT, UNIT_KG, DESC_POT_SEDOUT, DT_Raster1D);

    /// write out the XML file.
    res = mdi.GetXMLDocument();

    char *tmp = new char[res.size() + 1];
    strprintf(tmp, res.size() + 1, "%s", res.c_str());
    return tmp;
}