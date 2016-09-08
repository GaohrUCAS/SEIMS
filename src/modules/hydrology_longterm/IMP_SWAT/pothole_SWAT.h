/*!
* \brief Simulates depressional areas that do not drain to the stream network (pothole) and impounded areas such as rice paddies
 * \author Liang-Jun Zhu
 * \date Sep 2016
 *           1. Source code of SWAT include: pothole.f
 * 
 */
#pragma once

#include <string>
#include "api.h"
#include "util.h"
#include "SimulationModule.h"

using namespace std;

class IMP_SWAT : public SimulationModule
{
private:
	/// conversion factor (mm/ha => m^3)
	float m_cnv; 
    /// valid cells number
    int m_nCells;
	/// cell width, m
	float m_cellWidth;
	/// cell area, ha
	float m_cellArea;
	/// soil layers
	float *m_soilLayers;
	/// max soil layers
	int m_nSoilLayers;
	/**
    *	@brief Routing layers according to the flow direction
    *
    *	There are not flow relationships within each layer.
    *	The first element in each layer is the number of cells in the layer
    */
    float **m_routingLayers;
	/// number of routing layers
    int m_nRoutingLayers;
	/// leaf area index at which no evaporation occurs from water surface
	float m_evLAI;
	/// slope gradient (%)
	float *m_slope;
	/// saturated conductivity
	float **m_ks;
	/// field capacity (FC-WP)
	float **m_sol_fc;
	/// field capacity on soil profile (mm, FC-WP)
	float *m_sol_sumfc;
	/// soil thickness
	float **m_soilThick;
	/// porosity mm/mm
	float **m_sol_por;
	/// Average daily outflow to main channel from tile flow if drainage tiles are installed in the pothole, mm
	float m_potTilemm;
	/// Nitrate decay rate in impounded water body
	float m_potNo3Decay;
	/// Soluble phosphorus decay rate in impounded water body
	float m_potSolPDecay;
	/// impounding trigger
	float *m_impoundTrig;
	/// surface area of impounded area, ha
	float *m_potSurfaceArea;
	/// net precipitation
	float *m_netPrec;
	/// lai in the current day
	float *m_LAIDay;
	/// pet
	float *m_pet;
	/// surface runoff generated
	float *m_surfaceRunoff;
	/// sediment caused by erosion
	float *m_sedYield;
	//! sand yield
	float *m_sandYield;
	//! silt yield
	float *m_siltYield;
	//! clay yield
	float *m_clayYield;
	//! small aggregate yield
	float *m_smaggreYield;
	//! large aggregate yield
	float *m_lgaggreYield;
	/// amount of water stored in soil layers on current day, sol_st in SWAT
	float **m_soilStorage;
	/// amount of water stored in soil profile on current day, sol_sw in SWAT
	float *m_soilStorageProfile;
	/// amount of nitrate transported with surface runoff
	float *m_surqNo3;
	/// amount of soluble phosphorus transported with surface runoff
	float *m_surqSolP;
	/// 
	float *m_sedOrgN;
	///
	float *m_sedOrgP;
	/// 
	float *m_sedActiveMinP;
	/// 
	float *m_sedStableMinP;

	/// no3 amount kg
	float *m_potNo3;
	/// orgN amount kg
	float *m_potOrgN;
	/// soluble phosphorus loss rate    1/day
	float *m_potSolP;
	/// orgP amount kg
	float *m_potOrgP;
	/// active mineral P kg
	float *m_potActMinP;
	/// stable mineral P kg
	float *m_potStaMinP;
	/// sediment amount kg
	float *m_potSed;
	/// sand 
	float *m_potSand;
	/// silt
	float *m_potSilt;
	/// clay
	float *m_potClay;
	/// small aggregate
	float *m_potSag;
	/// large aggregate
	float *m_potLag;
	/// volume   mm
	float *m_potVol;
	/// maximum volume mm
	float *m_potVolMax;
	/// lowest volume mm
	float *m_potVolLow;
	/// seepage water of pothole, mm
	float *m_potSeep;
	/// evaporation, mm
	float *m_potEvap;
	///// flow in   mm
	//float *m_potFlowIn;
	///// flow out  mm
	//float *m_potFlowOut;
	///// sediment entering pothole on day   kg
	//float *m_potSedIn;
	///// sand 
	//float *m_potSandIn;
	///// silt
	//float *m_potSiltIn;
	///// clay
	//float *m_potClayIn;
	///// small aggregate
	//float *m_potSagIn;
	///// large aggregate
	//float *m_potLagIn;

public:
    //! Constructor
    IMP_SWAT(void);

    //! Destructor
    ~IMP_SWAT(void);

    virtual int Execute();

	virtual void SetValue(const char *key, float data);

    virtual void Set1DData(const char *key, int n, float *data);

    virtual void Get1DData(const char *key, int *n, float **data);

    virtual void Set2DData(const char *key, int n, int col, float **data);

private:
    /*!
     * \brief check the input data. Make sure all the input data is available.
     * \return bool The validity of the input data.
     */
    bool CheckInputData(void);

    /*!
     * \brief check the input size. Make sure all the input data have same dimension.
     *
     *
     * \param[in] key The key of the input data
     * \param[in] n The input data dimension
     * \return bool The validity of the dimension
     */
    bool CheckInputSize(const char *, int);
	/*!
     * \brief check the input size of 2D data. Make sure all the input data have same dimension.
     *
     *
     * \param[in] key The key of the input data
     * \param[in] n The first dimension input data 
	 * \param[in] col The second dimension of input data 
     * \return bool The validity of the dimension
     */
    bool CheckInputSize2D(const char *key, int n, int col);
	/// initialize all possible outputs
	void initialOutputs();
	/*!
	 * \brief Simulates depressional areas that do not 
	 * drain to the stream network (pothole) and impounded areas such as rice paddies
	 * reWrite from pothole.f of SWAT
	 */
	void potholeSimulate(int id);
	/*!
	 * compute surface area assuming a cone shape, ha
	 */
	void potholeSurfaceArea(int id);
	/*!
	 * release water stored in pothole
	 */
	void releaseWater(int id);
};
