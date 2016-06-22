/*!
 * \file BMPText.h
 * \brief BMP related const strings
 * \revised Liang-Jun Zhu
 * \date 2016-6-16
 */
#ifndef BMP_TEXT_H
#define BMP_TEXT_H

//database name
#define BMP_DATABASE_NAME	"BMP.db3"

//Table names in BMP database
#define TAB_BMP_INDEX				"BMP_index"
#define TAB_BMP_SCENARIO		"BMP_scenarios"
#define TAB_BMP_REACH				"Reach_BMP"
#define TAB_BMP_POINT_SRC	"pointsource"
#define TAB_BMP_FLOWDIVERSION	"flowdiversion"
#define TAB_BMP_RESERVOIR	"reservoir"

/// Table field names 
/// TAB_BMP_SCENARIO
#define FLD_SCENARIO_ID				"ID"
#define FLD_SCENARIO_NAME		"NAME"
#define FLD_SCENARIO_BMPID		"BMPID"
#define FLD_SCENARIO_SUB			"SUBSCENARIO"
#define FLD_SCENARIO_DIST			"DISTRIBUTION"
#define FLD_SCENARIO_LOOKUP	"COLLECTION"
#define FLD_SCENARIO_LOCATION "LOCATION"
/// TAB_BMP_INDEX
#define FLD_BMP_ID							"ID"
#define FLD_BMP_TYPE						"TYPE"
#define FLD_BMP_PRIORITY			"PRIORITY"

//BMPs
//also the column name in Reach_BMP table for reach BMPs
#define BMP_NAME_POINTSOURCE			"PointSource"
#define BMP_NAME_FLOWDIVERSION_STREAM	"FlowDiversion"
#define BMP_NAME_RESERVOIR				"Reservoir"
#define BMP_NAME_RIPARIANWETLAND		"RiparianWetland"
#define BMP_NAME_RIPARIANBUFFER			"RiparianBuffer"
#define BMP_NAME_GRASSWATERWAY			"Grass Water Way"
#define BMP_NAME_FILTERSTRIP			"Filter Strip"
#define BMP_NAME_POND					"Pond"
#define BMP_NAME_ISOLATEDPOND			"Isolated Pond"
#define BMP_NAME_TERRACE				"Terrace"
#define BMP_NAME_FLOWDIVERSION_OVERLAND	"Overland Flow Diversion"
#define BMP_NAME_CROP					"Crop Management"
#define BMP_NAME_RESIDUAL				"Residual Management"
#define BMP_NAME_TILLAGE				"Tillage Management"
#define BMP_NAME_FERTILIZER				"Fertilizer Management"
#define BMP_NAME_GRAZING				"Grazing Management"
#define BMP_NAME_PESTICIDE				"Pesticide Management"
#define BMP_NAME_IRRIGATION				"Irrigation Management"
#define BMP_NAME_TILEDRAIN				"Tile Drain Management"
#define BMP_NAME_URBAN					"Urban Management"

//the hydro climate data columns of pointsource, flowdiversion and reservoir
#define BMP_REACH_FLOW_NAME			"flow"
#define BMP_REACH_SEDIMENT_NAME		"sediment"
#define BMP_REACH_ORGANICN_NAME		"organicN"
#define BMP_REACH_ORGANICP_NAME		"organicP"
#define BMP_REACH_NO3_NAME			"NO3"
#define BMP_REACH_NH3_NAME			"NH3"
#define BMP_REACH_NO2_NAME			"NO2"
#define BMP_REACH_MINERALP_NAME		"mineralP"

#define BMP_REACH_UNKONWN_TYPE		-1
#define BMP_REACH_FLOW_TYPE			1
#define BMP_REACH_SEDIMENT_TYPE		2
#define BMP_REACH_ORGANICN_TYPE		3
#define BMP_REACH_ORGANICP_TYPE		4
#define BMP_REACH_NO3_TYPE			5
#define BMP_REACH_NH3_TYPE			6
#define BMP_REACH_NO2_TYPE			7
#define BMP_REACH_MINERALP_TYPE		8

#define BMP_REACH_X_INDEX				1
#define BMP_REACH_Y_INDEX				2



//base scenario id
#define BASE_SCENARIO_ID	0

//BMP Type
#define BMP_TYPE_POINTSOURCE			1
#define BMP_TYPE_FLOWDIVERSION_STREAM	2
#define BMP_TYPE_RESERVOIR				3
#define BMP_TYPE_RIPARIANWETLAND		4
#define BMP_TYPE_RIPARIANBUFFER			5
#define BMP_TYPE_GRASSWATERWAY			6
#define BMP_TYPE_FILTERSTRIP			7
#define BMP_TYPE_POND					8
#define BMP_TYPE_ISOLATEDPOND			9
#define BMP_TYPE_TERRACE				10
#define BMP_TYPE_FLOWDIVERSION_OVERLAND	11
#define BMP_TYPE_PLANT_MGT		12
/// TODO
#define BMP_TYPE_CROP					12
#define BMP_TYPE_RESIDUAL				13
#define BMP_TYPE_TILLAGE				14
#define BMP_TYPE_FERTILIZER				15
#define BMP_TYPE_GRAZING				16
#define BMP_TYPE_PESTICIDE				17
#define BMP_TYPE_IRRIGATION				18
#define BMP_TYPE_TILEDRAIN				19
#define BMP_TYPE_URBAN					20

/// Plant management code
#define BMP_PLTOP_PLANT				1
#define BMP_PLTOP_Irrigation			2
#define BMP_PLTOP_Fertilizer			3
#define BMP_PLTOP_Pesticide			4
#define BMP_PLTOP_HarvestKill		5
#define BMP_PLTOP_Tillage				6
#define BMP_PLTOP_Harvest				7
#define BMP_PLTOP_Kill					8
#define BMP_PLTOP_Grazing			9
#define BMP_PLTOP_AutoIrrigation	10
#define BMP_PLTOP_AutoFertilizer	11
#define BMP_PLTOP_StreetSweeping			12
#define BMP_PLTOP_ReleaseImpound		13
#define BMP_PLTOP_ContinuousFertilizer	14
#define BMP_PLTOP_ContinuousPesticide	15
#define BMP_PLTOP_Burning							16

/// Plant management fields
#define BMP_PLTOP_FLD_SUB					"SUBSCENARIO"
#define BMP_PLTOP_FLD_NAME					"NAME"
#define BMP_PLTOP_FLD_SEQUENCE		"SEQUENCE"
#define BMP_PLTOP_FLD_YEAR					"YEAR"
#define BMP_PLTOP_FLD_MONTH				"MONTH"
#define BMP_PLTOP_FLD_DAY					"DAY"
#define BMP_PLTOP_FLD_HUSC					"HUSC"
#define BMP_PLTOP_FLD_MGTOP				"MGT_OP"
#define BMP_PLTOP_FLD_MGT_PRE			"MGT"
//#define BMP_PLTOP_FLD_MGT2I				"MGT2I"
//#define BMP_PLTOP_FLD_MGT3I				"MGT3I"
//#define BMP_PLTOP_FLD_MGT4I				"MGT4"
//#define BMP_PLTOP_FLD_MGT5					"MGT5"
//#define BMP_PLTOP_FLD_MGT6					"MGT6"
//#define BMP_PLTOP_FLD_MGT7					"MGT7"
//#define BMP_PLTOP_FLD_MGT8					"MGT8"
//#define BMP_PLTOP_FLD_MGT9					"MGT9"
//#define BMP_PLTOP_FLD_MGT10I				"MGT10I"

//BMP Type
#define BMP_TYPE_REACH					1
#define BMP_TYPE_AREAL_STRUCTURAL		2
#define BMP_TYPE_AREAL_NON_STRUCTURAL	3

//Reservoir method column name
#define RESERVOIR_FLOW_ROUTING_METHOD_COLUMN_NAME		"METHOD"
#define RESERVOIR_SEDIMENT_ROUTING_METHOD_COLUMN_NAME	"SEDMETHOD"
#define RESERVOIR_NUTRIENT_ROUTING_METHOD_COLUMN_NAME	"NUTMETHOD"

//Reservoir flow routing method
#define RESERVOIR_FLOW_ROUTING_NAME_RATING_CURVE		"RAT_RES"
#define RESERVOIR_FLOW_ROUTING_NAME_DAILY_OUTFLOW		"MDO_RES"
#define RESERVOIR_FLOW_ROUTING_NAME_MONTHLY_OUTFLOW		"MMO_RES"
#define RESERVOIR_FLOW_ROUTING_NAME_ANUNAL_RELEASE_RATE	"AAR_RES"
#define RESERVOIR_FLOW_ROUTING_NAME_TARGET_RELEASE_RATE	"TRR_RES"

#define RESERVOIR_FLOW_ROUTING_NAME_UNKNOWN				-1
#define RESERVOIR_FLOW_ROUTING_RATING_CURVE				0
#define RESERVOIR_FLOW_ROUTING_DAILY_OUTFLOW			1
#define RESERVOIR_FLOW_ROUTING_MONTHLY_OUTFLOW			2
#define RESERVOIR_FLOW_ROUTING_ANUNAL_RELEASE_RATE		3
#define RESERVOIR_FLOW_ROUTING_TARGET_RELEASE_RATE		4

//Reservoir sediment routing method
#define RESERVOIR_SEDIMENT_ROUTING_NAME_MASS_BALANCE	"SMB_RES"

#define RESERVOIR_SEDIMENT_ROUTING_MASS_UNKONWN			-1
#define RESERVOIR_SEDIMENT_ROUTING_MASS_BALANCE			0

//Reservoir column index
#define RESERVOIR_SA_EM_INDEX			4   
#define RESERVOIR_V_EM_INDEX			5
#define RESERVOIR_SA_PR_INDEX			6   
#define RESERVOIR_V_PR_INDEX			7
#define RESERVOIR_INI_S_INDEX			8
#define RESERVOIR_Q_REL_INDEX			9
#define RESERVOIR_k_res_INDEX			10
#define RESERVOIR_OFLOWMN01_INDEX		11 
#define RESERVOIR_OFLOWMX01_INDEX		23 
#define RESERVOIR_METHOD_INDEX			35
#define RESERVOIR_SED_METHOD_INDEX		36	//The method used to do sediment routing
#define RESERVOIR_INI_SC_INDEX			37	//initial sediment concentration, Mg/m**3, default value is 0.03
#define RESERVOIR_NSED_INDEX			38	//equilibrium sediment concentration, Mg/m**3, 
#define RESERVOIR_D50_INDEX				39	//median particle size of the inflow sediment (um), default value is 10
#define RESERVOIR_NUT_METHOD_INDEX		40  //The method used to do nutrient routing

//Crop classification
#define CROP_IDC_WARM_SEASON_ANNUAL_LEGUME	1
#define CROP_IDC_CODE_SEASON_ANNUAL_LEGUME	2
#define CROP_IDC_PERENNIAL_LEGUME							3
#define CROP_IDC_WARM_SEASON_ANNUAL						4
#define CROP_IDC_COLD_SEASON_ANNUAL						5
#define CROP_IDC_PERENNIAL												6
#define CROP_IDC_TREES														7

#endif