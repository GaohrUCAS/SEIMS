#include <iostream>
#include "NutrientRemviaSr.h"
#include "MetadataInfo.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "ModelException.h"
#include "util.h"
#include <omp.h>

using namespace std;

NutrientRemviaSr::NutrientRemviaSr(void) :
//input
        m_nCells(-1), m_cellWidth(-1), m_soiLayers(-1), m_sedimentYield(NULL), m_nperco(-1), m_phoskd(-1), m_pperco(-1), m_cod_n(-1), 
		m_cod_k(-1), m_qtile(-1), m_nSoilLayers(NULL), m_anion_excl(NULL), m_isep_opt(-1), m_ldrain(NULL), m_dis_stream(NULL), m_surfr(NULL), m_flat(NULL),
        m_sol_perco(NULL), m_sol_wsatur(NULL), m_sol_crk(NULL), m_sol_bd(NULL), m_sol_z(NULL), m_sol_thick(NULL),
        m_sol_om(NULL), m_flowOutIndex(NULL), m_nSubbasins(-1), m_subbasin(NULL), m_subbasinsInfo(NULL), m_streamLink(NULL),
		m_routingLayers(NULL), m_nRoutingLayers(-1),
        //output
        m_latno3(NULL), m_perco_n(NULL), m_perco_p(NULL), m_surqno3(NULL), m_sol_no3(NULL), m_surqsolp(NULL), m_wshd_plch(-1),
		m_latno3ToCh(NULL), m_sur_no3ToCh(NULL), m_sur_codToCh(NULL), m_sur_solpToCh(NULL), m_perco_n_gw(NULL), m_perco_p_gw(NULL),
        m_sol_solp(NULL), m_cod(NULL), m_chl_a(NULL) //,m_doxq(), m_soxy()
{
}

NutrientRemviaSr::~NutrientRemviaSr(void)
{
	if (m_latno3 != NULL) Release1DArray(m_latno3);
	if (m_perco_n != NULL) Release1DArray(m_perco_n);
	if (m_perco_p != NULL) Release1DArray(m_perco_p);
	if (m_surqno3 != NULL) Release1DArray(m_surqno3);
	if (m_surqsolp != NULL) Release1DArray(m_surqsolp);
	if (m_cod != NULL) Release1DArray(m_cod);
	if (m_chl_a != NULL) Release1DArray(m_chl_a);

	if(m_latno3ToCh != NULL) Release1DArray(m_latno3ToCh);
	if(m_sur_no3ToCh != NULL) Release1DArray(m_sur_no3ToCh);
	if(m_sur_solpToCh != NULL) Release1DArray(m_sur_solpToCh);
	if(m_sur_codToCh != NULL) Release1DArray(m_sur_codToCh);
	if(m_perco_n_gw != NULL) Release1DArray(m_perco_n_gw);
	if(m_perco_p_gw != NULL) Release1DArray(m_perco_p_gw);
}

void NutrientRemviaSr::SumBySubbasin()
{
	// reset to zero
	for(int subi = 0; subi <= m_nSubbasins; subi++)
	{
		m_sur_no3ToCh[subi] = 0.f;
		m_sur_solpToCh[subi] = 0.f;
		m_perco_n_gw[subi] = 0.f;
		m_perco_p_gw[subi] = 0.f;
		m_sur_codToCh[subi] = 0.f;
		m_latno3ToCh[subi] = 0.f;
	}

	float cellArea = m_cellWidth * m_cellWidth * 0.0001f; //ha
	// sum by subbasin
	for (int i = 0; i < m_nCells; i++)
	{
		//add today's flow
		int subi = (int) m_subbasin[i];
		if (m_nSubbasins == 1)
			subi = 1;
		else if (subi >= m_nSubbasins + 1)
			throw ModelException(MID_NUTRSED, "Execute", "The subbasin ID " + ValueToString(subi) + " is invalid.");

		m_sur_no3ToCh[subi] += m_surqno3[i] * cellArea; // kg/ha * ha = kg
		m_sur_solpToCh[subi] += m_surqsolp[i] * cellArea;
		m_sur_codToCh[subi] += m_cod[i] * cellArea;
		//if(i == 1762)
		//	cout<<"sum by subbasin: perocN: "<<m_perco_n[i]<<endl;
		float ratio2gw = 1.f;
		m_perco_n_gw[subi] += m_perco_n[i] * cellArea * ratio2gw;
		m_perco_p_gw[subi] += m_perco_p[i] * cellArea * ratio2gw;
		if(m_streamLink[i] > 0)
			m_latno3ToCh[subi] += m_latno3[i];
	}
	//cout<<"m_sur_codToCh: "<<m_sur_codToCh[2]<<endl;

	// sum all the subbasins and put the sum value in the zero-index of the array
	for (int i = 1; i < m_nSubbasins + 1; i++)
	{
		m_sur_no3ToCh[0] += m_sur_no3ToCh[i];
		//cout<<"subID: "<<i<<", surNo3ToCh: "<<m_sur_no3ToCh[i]<<endl;
		m_sur_solpToCh[0] += m_sur_solpToCh[i];
		m_sur_codToCh[0] += m_sur_codToCh[i];
		m_latno3ToCh[0] += m_latno3ToCh[i];
		m_perco_n_gw[0] += m_perco_n_gw[i];
		m_perco_p_gw[0] += m_perco_p_gw[i];
	}
}

bool NutrientRemviaSr::CheckInputSize(const char *key, int n)
{
    if (n <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputSize",
                             "Input data for " + string(key) + " is invalid. The size could not be less than zero.");
        return false;
    }
    if (m_nCells != n)
    {
        if (m_nCells <= 0)
        {
            m_nCells = n;
        } else
        {
            ostringstream oss;
            oss << "Input data for " + string(key) << " is invalid with size: " << n << ". The origin size is " <<
            m_nCells << ".\n";
            throw ModelException(MID_NUTRMV, "CheckInputSize", oss.str());
        }
    }
    return true;
}

bool NutrientRemviaSr::CheckInputData()
{
    if (this->m_nCells <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The cells number can not be less than zero.");
    }
    if (this->m_cellWidth <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The cell width can not be less than zero.");
    }
    if (this->m_soiLayers <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The layer number of the input 2D raster data can not be less than zero.");
    }
    if (this->m_nSoilLayers == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "Soil layers number must not be NULL");
    }
    if (this->m_sedimentYield == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The distribution of soil loss caused by water erosion can not be NULL.");
    }
    if (this->m_sol_om == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The percent organic matter in soil layer can not be NULL.");
    }
    if (this->m_anion_excl == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The fraction of porosity from which anions are excluded can not be NULL.");
    }
    if (this->m_isep_opt < 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The septic operational condition code can not be NULL.");
    }
    //if (this->m_ldrain == NULL)
    //{
    //    throw ModelException(MID_NUTRMV, "CheckInputData", "The soil layer where drainage tile is located can not be NULL.");
	//}
	if (this->m_dis_stream == NULL)
	{
		throw ModelException(MID_NUTRMV, "CheckInputData", "The distance to the stream data can not be NULL.");
	}
    if (this->m_surfr == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The distribution of surface runoff generated data can not be NULL.");
    }
    if (this->m_nperco <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The nitrate percolation coefficient can not be less than zero.");
	}
	if (this->m_cod_n <= 0)
	{
		throw ModelException(MID_NUTRMV, "CheckInputData", "The m_cod_n can not be less than zero.");
	}
	if (this->m_cod_k <= 0)
	{
		throw ModelException(MID_NUTRMV, "CheckInputData", "The m_cod_k can not be less than zero.");
	}
    if (this->m_flat == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The lateral flow in soil layer data can not be NULL.");
    }
    if (this->m_sol_perco == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "percolation from soil layer can not be NULL.");
    }
    if (this->m_sol_wsatur == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The amount of water held in the soil layer at saturation data can not be NULL.");
    }
    if (this->m_phoskd <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "Phosphorus soil partitioning coefficient can not be less than zero.");
    }
	if (this->m_sol_thick == NULL)
	{
		throw ModelException(MID_NUTRMV, "CheckInputData", "The m_sol_thick can not be NULL.");
	}
    if (this->m_sol_crk == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The crack volume potential of soil data can not be NULL.");
    }
    if (this->m_pperco <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "Phosphorus percolation coefficient can not be less than zero.");
    }
    if (this->m_sol_bd == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The bulk density of the soil data can not be NULL.");
    }
    if (this->m_sol_z == NULL)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The depth to bottom of soil layer can not be NULL.");
    }
	if (m_flowOutIndex == NULL)
		throw ModelException(MID_NUTRMV, "CheckInputData", "The parameter: flow out index has not been set.");
	if (m_nSubbasins <= 0) 
		throw ModelException(MID_NUTRMV, "CheckInputData", "The subbasins number must be greater than 0.");
	if (m_subbasinIDs.empty()) 
		throw ModelException(MID_NUTRMV, "CheckInputData", "The subbasin IDs can not be EMPTY.");
	if (m_subbasinsInfo == NULL)
		throw ModelException(MID_NUTRMV, "CheckInputData", "The parameter: m_subbasinsInfo has not been set.");

    return true;
}

void NutrientRemviaSr::SetSubbasins(clsSubbasins *subbasins)
{
	if(m_subbasinsInfo == NULL){
		m_subbasinsInfo = subbasins;
		m_nSubbasins = m_subbasinsInfo->GetSubbasinNumber();
		m_subbasinIDs = m_subbasinsInfo->GetSubbasinIDs();
	}
}

void NutrientRemviaSr::SetValue(const char *key, float value)
{
    string sk(key);
    if (StringMatch(sk, VAR_OMP_THREADNUM))
    {
        omp_set_num_threads((int) value);
    }
    //else if (StringMatch(sk, Tag_CellSize)) { this->m_nCells = value; }
    else if (StringMatch(sk, Tag_CellWidth)) { this->m_cellWidth = value; }
    else if (StringMatch(sk, VAR_QTILE)) { this->m_qtile = value; }
    else if (StringMatch(sk, VAR_NPERCO)) { this->m_nperco = value; }
    else if (StringMatch(sk, VAR_PPERCO)) { this->m_pperco = value; }
    else if (StringMatch(sk, VAR_PHOSKD)) { this->m_phoskd = value; }
	else if (StringMatch(sk, VAR_ISEP_OPT)) { this->m_isep_opt = value; }
	else if (StringMatch(sk, VAR_COD_N)) { this->m_cod_n = value; }
	else if (StringMatch(sk, VAR_COD_K)) { this->m_cod_k = value; }
	//else if (StringMatch(sk, VAR_WSHD_PLCH)) m_wshd_plch = value;
    else
    {
        throw ModelException(MID_NUTRMV, "SetValue", "Parameter " + sk + " does not exist.");
    }
}

void NutrientRemviaSr::Set1DData(const char *key, int n, float *data)
{
    if (!this->CheckInputSize(key, n)) return;
    string sk(key);
    if (StringMatch(sk, VAR_FLOW_OL)) 
		m_surfr = data; 
	else if (StringMatch(sk, VAR_SUBBSN))
		m_subbasin = data;
	else if(StringMatch(sk, VAR_STREAM_LINK))
		m_streamLink = data;
    else if (StringMatch(sk, VAR_ANION_EXCL)) 
		m_anion_excl = data; 
    else if (StringMatch(sk, VAR_LDRAIN)) 
		m_ldrain = data; 
	else if (StringMatch(sk, VAR_DISTSTREAM)) 
		m_dis_stream = data; 
    else if (StringMatch(sk, VAR_SOL_CRK)) 
		m_sol_crk = data; 
	else if (StringMatch(sk, VAR_SOILLAYERS)) 
		m_nSoilLayers = data; 
    else if (StringMatch(sk, VAR_SED_OL))
		this->m_sedimentYield = data;
	else if (StringMatch(sk, Tag_FLOWOUT_INDEX_D8))
		m_flowOutIndex = data;
    else if (StringMatch(sk, VAR_SEDORGN)) 
		m_sedorgn = data; 
    else if (StringMatch(sk, VAR_TMEAN)) 
		m_tmean = data;
    else
        throw ModelException(MID_NUTRMV, "Set1DData", "Parameter " + sk + " does not exist.");
}

void NutrientRemviaSr::Set2DData(const char *key, int nRows, int nCols, float **data)
{
	string sk(key);
	if (StringMatch(sk, Tag_ROUTING_LAYERS))
	{
		m_nRoutingLayers = nRows;
		m_routingLayers = data;
		return;
	}
    if (!this->CheckInputSize(key, nRows)) return;
	m_soiLayers = nCols;
    if (StringMatch(sk, VAR_SSRU)) { m_flat = data; }
    else if (StringMatch(sk, VAR_SOL_NO3)) { m_sol_no3 = data; }
    else if (StringMatch(sk, VAR_SOL_BD)) { m_sol_bd = data; }
    else if (StringMatch(sk, VAR_SOL_SOLP)) { m_sol_solp = data; }
    else if (StringMatch(sk, VAR_SOILDEPTH)) { m_sol_z = data; }
    else if (StringMatch(sk, VAR_PERCO)) { m_sol_perco = data; }
	else if (StringMatch(sk, VAR_SOL_OM)) { m_sol_om = data; }
	else if (StringMatch(sk, VAR_SOILTHICK)) { m_sol_thick = data; }
	else if (StringMatch(sk, VAR_SOL_UL)) { m_sol_wsatur = data; }
    else
        throw ModelException(MID_NUTRMV, "Set2DData", "Parameter " + sk + " does not exist.");
}

void NutrientRemviaSr::initialOutputs()
{
    if (this->m_nCells <= 0)
    {
        throw ModelException(MID_NUTRMV, "CheckInputData", "The dimension of the input data can not be less than zero.");
    }
    // allocate the output variables
	if (m_latno3 == NULL) Initialize1DArray(m_nCells, m_latno3, 0.f);
	if (m_perco_n == NULL) Initialize1DArray(m_nCells, m_perco_n, 0.f);
	if (m_perco_p == NULL) Initialize1DArray(m_nCells, m_perco_p, 0.f);
	if (m_surqno3 == NULL) Initialize1DArray(m_nCells, m_surqno3, 0.f);
	if (m_surqsolp == NULL) Initialize1DArray(m_nCells, m_surqsolp, 0.f);

	if (m_latno3ToCh == NULL) Initialize1DArray(m_nSubbasins+1, m_latno3ToCh, 0.f);
	if (m_sur_no3ToCh == NULL) Initialize1DArray(m_nSubbasins+1, m_sur_no3ToCh, 0.f);
	if (m_sur_solpToCh == NULL) Initialize1DArray(m_nSubbasins+1, m_sur_solpToCh, 0.f);
	if (m_sur_codToCh == NULL) Initialize1DArray(m_nSubbasins+1, m_sur_codToCh, 0.f);
	if (m_perco_n_gw == NULL) Initialize1DArray(m_nSubbasins+1, m_perco_n_gw, 0.f);
	if (m_perco_p_gw == NULL) Initialize1DArray(m_nSubbasins+1, m_perco_p_gw, 0.f);

    if (m_cod == NULL)
    {
		Initialize1DArray(m_nCells, m_cod, 0.f);
		Initialize1DArray(m_nCells, m_chl_a, 0.f);
    }
    if (m_wshd_plch < 0)
    {
        m_wshd_plch = 0.f;
    }
    // input variables
    if (m_flat == NULL) { Initialize2DArray(m_nCells, m_soiLayers, m_flat, 0.0001f); }
    if (m_sol_perco == NULL) { Initialize2DArray(m_nCells, m_soiLayers, m_sol_perco, 0.0001f); }
    if (m_ldrain == NULL) { Initialize1DArray(m_nCells, m_ldrain, -1.f); }
    m_qtile = 0.0001f;
}

int NutrientRemviaSr::Execute()
{
    if (!CheckInputData())
    {
        return false;
    }
    initialOutputs();
    // compute nitrate movement leaching
	//cout<<"NUTRMV-exec, cell id 5878, sol_no3[0]: "<<m_sol_no3[5878][0]<<endl;
    NitrateLoss();
	//cout<<"NUTRMV-loss, cell id 5878, sol_no3[0]: "<<m_sol_no3[5878][0]<<endl;
    // compute phosphorus movement
    PhosphorusLoss();
	// compute chl-a, CBOD and dissolved oxygen loadings
	SubbasinWaterQuality();
	// sum by sub-basin
	SumBySubbasin();
	//for (int i = 1; i <= m_nSubbasins; i++){
	//	cout<<"surNo3ToCh: "<<m_sur_no3ToCh[i]<<", ";
	//}
	//cout<<endl;
	//for (int i = 1; i <= m_nSubbasins; i++){
	//	cout<<"percoNToCh: "<<m_perco_n_gw[i]<<", ";
	//}
	//cout<<endl;
    return 0;
}

void NutrientRemviaSr::NitrateLoss()
{
// 	float tmpPercN = NODATA_VALUE;
// 	int tmpIdx = -1;
	for (int iLayer = 0; iLayer < m_nRoutingLayers; iLayer++)
	{
		// There are not any flow relationship within each routing layer.
		// So parallelization can be done here.
		int nCells = (int) m_routingLayers[iLayer][0];
#pragma omp parallel for
		for (int iCell = 1; iCell <= nCells; iCell++)
		{
			int i = (int) m_routingLayers[iLayer][iCell]; // cell ID
			float percnlyr = 0.f;
			m_latno3[i] = 0.f;
			for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
			{
				// add nitrate leached from layer above (kg/ha)
				m_sol_no3[i][k] = m_sol_no3[i][k] + percnlyr;
				//percnlyr = 0.f;
				if (m_sol_no3[i][k] < 1.e-6f) 
					m_sol_no3[i][k] = 0.f;
				// determine concentration of nitrate in mobile water
				float sro = 0.f;// surface runoff generated (sro)
				float mw = 0.f;// amount of mobile water in the layer (mw)
				float vno3 = 0.f; // kg/ha
				float con = 0.f;  // 100 mg/L
				float ww = 0.f;

				if (k == 0)
					sro = m_surfr[i];
				else
					sro = 0.f;
				if (m_ldrain[i] == k)
					mw += m_qtile;

				// Calculate the concentration of nitrate in the mobile water (con),
				// equation 4:2.1.2, 4:2.1.3 and 4:2.1.4 in SWAT Theory 2009, p269
				mw = m_sol_perco[i][k] + sro + m_flat[i][k] + 1.e-10f;
				float satportion = ((1.f - m_anion_excl[i]) * m_sol_wsatur[i][k]);
				//if (mw > satportion) mw = satportion;
				ww = -mw / satportion;
				vno3 = m_sol_no3[i][k] * (1.f - exp(ww)); // kg/ha
				if (mw > 1.e-10f)
					con = max(vno3 / mw, 0.f); // kg/ha/mm = 100 mg/L
				//if (con > 0.1) con = 0.1;
				//if (i == 5570)
				//{
				//	cout<<"perco water: "<<m_sol_perco[i][k]<<", satportion: "<<satportion<<", mv: "<<mw<<", ww: "<<ww<<", vno3: "<<vno3<<",con: "<<con<<endl;
				//}

				// calculate nitrate in surface runoff
				// concentration of nitrate in surface runoff (cosurf)
				float cosurf = 0.f;
				if (m_isep_opt == 2)
					cosurf = 1.f * con; // N percolation does not apply to failing septic place;
				else
					cosurf = m_nperco * con;
				if (k == 0)
				{
					m_surqno3[i] = m_surfr[i] * cosurf; // kg/ha
					m_surqno3[i] = min(m_surqno3[i], m_sol_no3[i][k]);
					m_sol_no3[i][k] -= m_surqno3[i];
				}
				// calculate nitrate in tile flow, TODO
				if (m_ldrain[i] == k)
				{
				}
				// nitrate moved with subsuface flow (kg/ha)
				float ssfnlyr = 0.f;
				// calculate nitrate in lateral flow
				// nitrate transported in lateral flow from layer (ssfnlyr)
				if (k == 0)
					ssfnlyr = cosurf * m_flat[i][k];
				else
					ssfnlyr = con * m_flat[i][k]; // 100 mg/L * mm = kg/ha
				ssfnlyr = min(ssfnlyr, m_sol_no3[i][k]);
				m_latno3[i] += ssfnlyr;
				// move the lateral no3 flow to the downslope cell (routing considered)
				m_sol_no3[i][k] -= ssfnlyr;
				int idDownSlope = (int)m_flowOutIndex[i];
				if (idDownSlope >= 0)
					m_sol_no3[idDownSlope][k] += ssfnlyr;
				
				// calculate nitrate in percolate
				percnlyr = con * m_sol_perco[i][k];
				//if(i == 5570)
				//	cout<<"layer: "<<k<<", con: "<<con<<", sol_perco: "<<m_sol_perco[i][k]<<", solno3: "<<m_sol_no3[i][k]<<endl;

				percnlyr = min(percnlyr, m_sol_no3[i][k]);
				m_sol_no3[i][k] -= percnlyr;
				//if(i == 0 && k == 0) cout << percnlyr << ", \n";
			}
			// calculate nitrate leaching from soil profile
			m_perco_n[i] = 0;
			m_perco_n[i] = percnlyr; // kg/ha
// 			if (tmpPercN < percnlyr){
// 				tmpIdx = i;
// 				tmpPercN = percnlyr;
// 			}
			// I think these should be removed, because the lost nitrate
			// have been added to it's downslope cell. by LJ
			//float nloss = 0.f;
			//nloss = (2.18f * m_dis_stream[i] - 8.63f) / 100.f;
			//nloss = min(1.f, max(0.f, nloss));
			//m_latno3[i] = (1.f - nloss) * m_latno3[i];
		}
	}
	//tmpIdx = 5570;
	//cout<<"NUTRMV, cell index: "<<tmpIdx<<", percoN "<<m_perco_n[tmpIdx]<<endl;
	//for(int i = 0; i < m_nSoilLayers[tmpIdx];i++)
	//	cout<<"layer: "<<i<<": "<<m_sol_no3[tmpIdx][i]<<", ";
	//cout<<endl;
}

void NutrientRemviaSr::PhosphorusLoss()
{
#pragma omp parallel for
    for (int i = 0; i < m_nCells; i++)
    {
        float wt1 = m_sol_bd[i][0] * m_sol_thick[i][0] / 100.f; // mg/kg => kg/ha
        float conv_wt = 1.e6f * wt1; // kg/kg => kg/ha

        // amount of P leached from soil layer (vap)
        float vap = 0.f;
        float vap_tile = 0.f;
        // compute soluble P lost in surface runoff
        float xx = 0.f;  // variable to hold intermediate calculation result
        xx = m_sol_bd[i][0] * m_sol_z[i][0] * m_phoskd;
        m_surqsolp[i] = m_sol_solp[i][0] * m_surfr[i] / xx;
        m_surqsolp[i] = min(m_surqsolp[i], m_sol_solp[i][0]);
        m_surqsolp[i] = max(m_surqsolp[i], 0.f);
        m_sol_solp[i][0] = m_sol_solp[i][0] - m_surqsolp[i];

        // compute soluble P leaching
        vap = m_sol_solp[i][0] * m_sol_perco[i][0] / ((conv_wt / 1000.f) * m_pperco);
        vap = min(vap, 0.5f * m_sol_solp[i][0]);
        m_sol_solp[i][0] = m_sol_solp[i][0] - vap;

        // estimate soluble p in tiles due to crack flow
        if (m_ldrain[i] > 0)
        {
            xx = min(1.f, m_sol_crk[i] / 3.f);
            vap_tile = xx * vap;
            vap = vap - vap_tile;
        }
        if (m_nSoilLayers[i] >= 2)
        {
            m_sol_solp[i][1] = m_sol_solp[i][1] + vap;
        }
        for (int k = 1; k < m_nSoilLayers[i]; k++)
        {
            vap = 0.f;
            //if (k != m_i_sep[i]) {  // soil layer where biozone exists (m_i_sep)
            vap = m_sol_solp[i][k] * m_sol_perco[i][k] / ((conv_wt / 1000.f) * m_pperco);
            vap = min(vap, 0.2f * m_sol_solp[i][k]);
            m_sol_solp[i][k] = m_sol_solp[i][k] - vap;

			if(k < m_nSoilLayers[i] - 1)
				m_sol_solp[i][k+1] += vap;//leach to next layer
			else
				m_perco_p[i] = vap;//leach to groundwater
            //}
        }
        // summary calculation
        m_wshd_plch = m_wshd_plch + vap * (1 / m_nCells);
    }
}

void NutrientRemviaSr::SubbasinWaterQuality()
{
#pragma omp parallel for
	for (int i = 0; i < m_nCells; i++)
	{
		// calculate water temperature
		// Stefan and Preudhomme. 1993.  Stream temperature estimation
		// from air temperature.  Water Res. Bull. p. 27-45
		// SWAT manual 2.3.13
// 		float wtmp = 0.f;
// 		wtmp = 5.f + 0.75f * m_tmean[i];
// 		if (wtmp < 0.1) wtmp = 0.1f;
// 		wtmp = wtmp + 273.15f;    // deg C to deg K

		// water in cell
		float qdr = 0.f;
		qdr = m_surfr[i] + m_flat[i][0] + m_qtile;
		if (qdr > 1.e-6f)
		{
			// kilo moles of phosphorus in nutrient loading to main channel (tp)
			float tp = 0.f;
			tp = 100.f * (m_sedorgn[i] + m_surqno3[i]) / qdr;   //100*kg/ha/mm = ppm
			// regional adjustment on sub chla_a loading
			float chla_subco = 40.f;
			m_chl_a[i] = chla_subco * tp;
			m_chl_a[i] = m_chl_a[i] / 1000.f;  // um/L to mg/L

			// calculate enrichment ratio
			float enratio;
			if (m_sedimentYield[i] < 1e-4)
			{
				m_sedimentYield[i] = 0.f;
			}
			// CREAMS method for calculating enrichment ratio, enrsb.f of SWAT
			float cy = 0.f;
			// Calculate sediment, equation 4:2.2.3 in SWAT Theory 2009, p272
			cy = 0.1f * (m_sedimentYield[i] / 1000.f) / (m_cellWidth * m_cellWidth * 0.0001f * m_surfr[i] + 1e-6f);
			if (cy > 1e-6f)
			{
				enratio = 0.78f * pow(cy, -0.2468f);
			} else
			{
				enratio = 0.f;
			}
			if (enratio > 3.5)
			{
				enratio = 3.5f;
			}
			// calculate organic carbon loading to main channel
			float org_c = (m_sol_om[i][0] * 0.58f / 100.f) * enratio * (m_sedimentYield[i] / 1000.f) * 1000.f;
			// calculate carbonaceous biological oxygen demand (CBOD) and COD(transform from CBOD)
			float cbod  = 2.7f * org_c / (qdr * m_cellWidth * m_cellWidth * 1.e-6f); //  kg/m3 
			// calculate COD
			cbod = m_cod_n * (cbod * (1.f - exp(-5.f * m_cod_k)));
			m_cod[i] = m_surfr[i] / 1000.f * cbod * 10.f;	// mg/L converted to kg/ha
		} else
		{
			m_chl_a[i] = 0.f;
			m_cod[i] = 0.f;
		}
	}
}

void NutrientRemviaSr::GetValue(const char *key, float *value)
{
    string sk(key);
    if (StringMatch(sk, VAR_WSHD_PLCH))
		*value = this->m_wshd_plch;
	else
		throw ModelException(MID_NUTRMV, "GetValue", "Parameter " + sk + " does not exist."); 
}

void NutrientRemviaSr::Get1DData(const char *key, int *n, float **data)
{
    string sk(key);
	initialOutputs();
    if (StringMatch(sk, VAR_LATNO3)) 
	{
		*data = this->m_latno3;
		*n = m_nCells;
	}
    else if (StringMatch(sk, VAR_PERCO_N_GW)) 
	{
		*data = m_perco_n_gw; 
		*n = m_nSubbasins + 1;
	}
	else if (StringMatch(sk, VAR_PERCO_P_GW)) 
	{
		*data = m_perco_p_gw; 
		*n = m_nSubbasins + 1;
	}
    else if (StringMatch(sk, VAR_SUR_NO3)) 
	{
		*data = this->m_surqno3; 
		*n = m_nCells;
	}
    else if (StringMatch(sk, VAR_SUR_SOLP)) 
	{
		*data = this->m_surqsolp; 
		*n = m_nCells;
	}
    else if (StringMatch(sk, VAR_COD)) 
	{
		*data = this->m_cod; 
		*n = m_nCells;
	}
    else if (StringMatch(sk, VAR_CHL_A)) 
	{
		*data = this->m_chl_a; 
		*n = m_nCells;
	}
	else if(StringMatch(sk, VAR_LATNO3_TOCH))
	{
		*data = m_latno3ToCh;
		*n = m_nSubbasins + 1;
	}
	else if(StringMatch(sk, VAR_SUR_NO3_TOCH))
	{
		*data = m_sur_no3ToCh;
		*n = m_nSubbasins + 1;
	}
	else if(StringMatch(sk, VAR_SUR_SOLP_TOCH))
	{
		*data = m_sur_solpToCh;
		*n = m_nSubbasins + 1;
	}
	else if(StringMatch(sk, VAR_SUR_COD_TOCH))
	{
		*data = m_sur_codToCh;
		*n = m_nSubbasins + 1;
	}
    else
        throw ModelException(MID_NUTRMV, "Get1DData", "Parameter " + sk + " does not exist.");                
}
void NutrientRemviaSr::Get2DData(const char *key, int *nRows, int *nCols, float ***data)
{
	initialOutputs();
    string sk(key);
    *nRows = m_nCells;
    *nCols = m_soiLayers;
    if (StringMatch(sk, VAR_SOL_NO3))
    {
        *data = this->m_sol_no3;
    }
    else if (StringMatch(sk, VAR_SOL_SOLP))
    {
        *data = this->m_sol_solp;
    }
    else
        throw ModelException(MID_NUTRMV, "Get2DData", "Output " + sk+" does not exist.");
}
