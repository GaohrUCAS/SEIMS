
/*!
 * \file NutrientinGroundwater.h
 * \brief Calculates the nitrate and soluble phosphorus loading contributed by groundwater flow.
 * \author Huiran Gao
 * \date Jun 2016
 */

#pragma once
#ifndef SEIMS_NutRemv_PARAMS_INCLUDE
#define SEIMS_NutRemv_PARAMS_INCLUDE

#include <string>
#include "api.h"
#include "SimulationModule.h"

using namespace std;
/** \defgroup NutGW
 * \ingroup Nutrient
 * \brief Calculates the nitrate and soluble phosphorus loading contributed by groundwater flow.
 */

/*!
 * \class NutrientinGroundwater
 * \ingroup NutGW
 *
 * \brief Calculates the nitrate and soluble phosphorus loading contributed by groundwater flow.
 *
 */

class NutrientinGroundwater : public SimulationModule
{
public:
    NutrientinGroundwater(void);

    ~NutrientinGroundwater(void);

    virtual void SetValue(const char *key, float value);

    virtual void Set1DData(const char *key, int n, float *data);
	virtual void Set2DData(const char *key, int nRows, int nCols, float **data);
	virtual void SetReaches(clsReaches *reaches);
    //virtual void Set2DData(const char* key, int nRows, int nCols, float** data);
    virtual int Execute();

    //virtual void GetValue(const char* key, float* value);
    virtual void Get1DData(const char *key, int *n, float **data);
    //virtual void Get2DData(const char* key, int* nRows, int* nCols, float*** data);

	virtual void SetSubbasins(clsSubbasins *subbasins);

private:
    /// cell width of grid map (m)
    float m_cellWidth;
    /// number of cells
    int m_nCells;
	/// time step (s)
	int m_TimeStep;

    /// input data
    /// nitrate N concentration in groundwater loading to reach (g/L, i.e. kg/m3)
    float *m_gwno3Con;
    /// soluble P concentration in groundwater loading to reach (g/L, i.e. kg/m3)
    float *m_gwminpCon;
    /// groundwater contribution to stream flow (m3/s)
    float *m_gw_q; 
	/// groundwater storage
	float *m_gwStor;
	/// amount of nitrate percolating past bottom of soil profile
	float *m_perco_no3_gw;
	/// amount of solute P percolating past bottom of soil profile
	float *m_perco_solp_gw;

	// soil related
	/// amount of nitrogen stored in the nitrate pool in soil layer
	float **m_sol_no3;
	/// max number of soil layers
	int m_nSoilLayers;
	/// number of soil layers of each cell
	float *m_soilLayers;


    /// outputs

    /// nitrate loading to reach in groundwater to channel
    float *m_no3gwToCh;
    /// soluble P loading to reach in groundwater to channel
    float *m_minpgwToCh;

	/// subbasin related
	/// the total number of subbasins
	int m_nSubbasins;
	//! subbasin IDs
	vector<int> m_subbasinIDs;
	/// subbasin grid (subbasins ID)
	float *m_subbasin;
	/// subbasins information
	clsSubbasins *m_subbasinsInfo;

private:

    /*!
     * \brief check the input data. Make sure all the input data is available.
     * \return bool The validity of the input data.
     */
    bool CheckInputData(void);

    /*!
     * \brief check the input size. Make sure all the input data have same dimension.
     *
     * \param[in] key The key of the input data
     * \param[in] n The input data dimension
     * \return bool The validity of the dimension
     */
    bool CheckInputSize(const char *, int);

    void initialOutputs();

};

#endif





