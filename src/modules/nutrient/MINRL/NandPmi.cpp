/*!
 * \ingroup MINRL
 * \author Huiran Gao
 * \date April 2016
 */

#include <iostream>
#include "NandPmi.h"
#include "MetadataInfo.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "ModelException.h"
#include "util.h"
#include <omp.h>

using namespace std;

NandPim::NandPim(void) :
		//input
        m_nCells(-1), m_cellWidth(-1.f), m_soilLayers(-1), m_cmn(-1.f), m_cdn(-1.f), m_sdnco(-1.f),m_nactfr(-1.f), m_psp(-1.f), m_ssp(-1.f),
        m_nSoilLayers(NULL), m_sol_z(NULL), m_sol_thick(NULL), m_sol_clay(NULL), m_sol_bd(NULL),
        m_landcover(NULL), m_rsdco_pl(NULL), m_sol_cbn(NULL), m_a_days(NULL), m_b_days(NULL),
        m_sol_wpmm(NULL),  m_sol_awc(NULL),
        m_sol_solp(NULL), m_sol_orgp(NULL),m_sol_actp(NULL), m_sol_stap(NULL),  m_sol_fop(NULL), 
        m_sol_no3(NULL),m_sol_nh4(NULL),m_sol_orgn(NULL), m_sol_aorgn(NULL), m_sol_fon(NULL),
		m_sol_cov(NULL),m_sol_rsd(NULL), 
		/// from other modules
		m_soilStorage(NULL),m_sote(NULL),
		/// cell scale output
		m_hmntl(NULL), m_hmptl(NULL), m_rmn2tl(NULL),
		m_rmptl(NULL), m_rwntl(NULL), m_wdntl(NULL), m_rmp1tl(NULL), m_roctl(NULL),
		/// watershed scale statistics
		m_wshd_dnit(-1.f), m_wshd_hmn(-1.f), m_wshd_hmp(-1.f), m_wshd_rmn(-1.f), m_wshd_rmp(-1.f), 
        m_wshd_rwn(-1.f), m_wshd_nitn(-1.f), m_wshd_voln(-1.f), m_wshd_pal(-1.f), m_wshd_pas(-1.f),
		m_solP_model(0),m_CbnModel(0),
		/// CENTURY model related variables
		m_sol_WOC(NULL), m_sol_WON(NULL), m_sol_BM(NULL), m_sol_BMC(NULL), m_sol_BMN(NULL), 
		m_sol_HP(NULL), m_sol_HS(NULL), m_sol_HSC(NULL), m_sol_HSN(NULL), m_sol_HPC(NULL), 
		m_sol_HPN(NULL), m_sol_LM(NULL), m_sol_LMC(NULL), m_sol_LMN(NULL), m_sol_LSC(NULL), 
		m_sol_LSN(NULL), m_sol_LS(NULL), m_sol_LSL(NULL), m_sol_LSLC(NULL), m_sol_LSLNC(NULL)
		//m_sol_RNMN(NULL), m_sol_RSPC(NULL)
{
}

NandPim::~NandPim(void)
{
	if(m_hmntl != NULL) Release1DArray(m_hmntl);
	if(m_hmptl != NULL) Release1DArray(m_hmptl);
	if(m_rmn2tl != NULL) Release1DArray(m_rmn2tl);
	if(m_rmptl != NULL) Release1DArray(m_rmptl);
	if(m_rwntl != NULL) Release1DArray(m_rwntl);
	if(m_wdntl != NULL) Release1DArray(m_wdntl);
	if(m_rmp1tl != NULL) Release1DArray(m_rmp1tl);
	if(m_roctl != NULL) Release1DArray(m_roctl);
	if (m_a_days != NULL) Release1DArray(m_a_days);
	if (m_b_days != NULL) Release1DArray(m_b_days);
	/// release CENTURY related variables
	if (m_sol_WOC != NULL) Release2DArray(m_nCells, m_sol_WOC);
	if (m_sol_WON != NULL) Release2DArray(m_nCells, m_sol_WON);
	if (m_sol_BM != NULL) Release2DArray(m_nCells, m_sol_BM);
	if (m_sol_BMC != NULL) Release2DArray(m_nCells, m_sol_BMC);
	if (m_sol_BMN != NULL) Release2DArray(m_nCells, m_sol_BMN);
	if (m_sol_HP != NULL) Release2DArray(m_nCells, m_sol_HP);
	if (m_sol_HS != NULL) Release2DArray(m_nCells, m_sol_HS);
	if (m_sol_HSC != NULL) Release2DArray(m_nCells, m_sol_HSC);
	if (m_sol_HSN != NULL) Release2DArray(m_nCells, m_sol_HSN);
	if (m_sol_HPC != NULL) Release2DArray(m_nCells, m_sol_HPC);
	if (m_sol_HPN != NULL) Release2DArray(m_nCells, m_sol_HPN);
	if (m_sol_LM != NULL) Release2DArray(m_nCells, m_sol_LM);
	if (m_sol_LMC != NULL) Release2DArray(m_nCells, m_sol_LMC);
	if (m_sol_LMN != NULL) Release2DArray(m_nCells, m_sol_LMN);
	if (m_sol_LSC != NULL) Release2DArray(m_nCells, m_sol_LSC);
	if (m_sol_LSN != NULL) Release2DArray(m_nCells, m_sol_LSN);
	if (m_sol_LS != NULL) Release2DArray(m_nCells, m_sol_LS);
	if (m_sol_LSL != NULL) Release2DArray(m_nCells, m_sol_LSL);
	if (m_sol_LSLC != NULL) Release2DArray(m_nCells, m_sol_LSLC);
	if (m_sol_LSLNC != NULL) Release2DArray(m_nCells, m_sol_LSLNC);
	//if (m_sol_RNMN != NULL) Release2DArray(m_nCells, m_sol_RNMN);
	//if (m_sol_RSPC != NULL) Release2DArray(m_nCells, m_sol_RSPC);


}

bool NandPim::CheckInputSize(const char *key, int n)
{
    if (n <= 0)
        throw ModelException(MID_MINRL, "CheckInputSize",
                             "Input data for " + string(key) + " is invalid. The size could not be less than zero.");
    if (m_nCells != n)
    {
        if (m_nCells <= 0)
            m_nCells = n;
        else
        {
			throw ModelException(MID_MINRL, "CheckInputSize", "Input data for " + string(key) + " is invalid with size: "+ ValueToString(n) +
				". The origin size is " + ValueToString(m_nCells) + ".\n");
        }
    }
    return true;
}

bool NandPim::CheckInputData()
{
    if (this->m_nCells <= 0)
        throw ModelException(MID_MINRL, "CheckInputData", "The input data can not be less than zero.");
    if (this->m_soilLayers < 0)
        throw ModelException(MID_MINRL, "CheckInputData", "The maximum soil layers number can not be NULL.");
    if (this->m_cellWidth < 0)
        throw ModelException(MID_MINRL, "CheckInputData", "The cell width can not be less than 0.");
    if (this->m_nSoilLayers == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The soil layers can not be NULL.");
    if (this->m_cmn <0)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_cmn can not be less than 0.");
    if (this->m_cdn <0)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_cdn can not be less than 0.");
    if (this->m_landcover == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_landcover can not be NULL.");
    if (this->m_nactfr <0)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_nactfr can not be less than 0.");
	if (this->m_sdnco <0)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_sdnco can not be less than 0.");
    if (this->m_psp <0)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_psp can not be less than 0.");
	if (this->m_ssp <0)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_ssp can not be less than 0.");
	if (this->m_sol_clay == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_clay can not be NULL.");
    if (this->m_sol_z == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_z can not be NULL.");
	if (this->m_sol_rsdin == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_rsdin can not be NULL.");
	//if (m_sol_cov == NULL)
	//	throw ModelException(MID_MINRL, "CheckInputData", "The residue on soil surface can not be NULL.");
	//if (m_sol_rsd == NULL)
	//	throw ModelException(MID_MINRL, "CheckInputData", "The organic matter in soil classified as residue can not be NULL.");
	if (this->m_a_days == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_a_days can not be NULL.");
	if (this->m_b_days == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_b_days can not be NULL.");
	if (this->m_sol_thick == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_thick can not be NULL.");
	if (this->m_sol_bd == NULL)
		throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_bd can not be NULL.");
    if (this->m_rsdco_pl == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_rsdco_pl can not be NULL.");
    if (this->m_sol_cbn == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_cbn can not be NULL.");
    if (this->m_sol_awc == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_awc can not be NULL.");
    if (this->m_sol_wpmm == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_wpmm can not be NULL.");
    if (this->m_sol_no3 == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_no3 can not be NULL.");
    if (this->m_sol_nh4 == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_nh4 can not be NULL.");
    if (this->m_sol_orgn == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_orgn can not be NULL.");
    if (this->m_sol_orgp == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_orgp can not be NULL.");
    if (this->m_sol_solp == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sol_solp can not be NULL.");
    if (this->m_soilStorage == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_soilStorage can not be NULL.");
    if (this->m_sote == NULL)
        throw ModelException(MID_MINRL, "CheckInputData", "The m_sote can not be NULL.");
    return true;
}

void NandPim::SetValue(const char *key, float value)
{
    string sk(key);
    if (StringMatch(sk, VAR_OMP_THREADNUM)) omp_set_num_threads((int) value); 
    else if (StringMatch(sk, Tag_CellWidth)) { this->m_cellWidth = value; }
	else if (StringMatch(sk, VAR_NACTFR)) { this->m_nactfr = value; }
	else if (StringMatch(sk, VAR_SDNCO)) { this->m_sdnco = value; }
    else if (StringMatch(sk, VAR_CMN)) { this->m_cmn = value; }
    else if (StringMatch(sk, VAR_CDN)) { this->m_cdn = value; }
	else if (StringMatch(sk, VAR_PSP)) { this->m_psp = value; }
	else if (StringMatch(sk, VAR_SSP)) { this->m_ssp = value; }
	else if (StringMatch(sk, VAR_CSWAT)) { this->m_CbnModel = value; }
    else
        throw ModelException(MID_MINRL, "SetValue", "Parameter " + sk + " does not exist.");
}

void NandPim::Set1DData(const char *key, int n, float *data)
{
    if (!this->CheckInputSize(key, n)) return;
    string sk(key);
    if (StringMatch(sk, VAR_LCC)) { this->m_landcover = data; }
	else if (StringMatch(sk, VAR_PL_RSDCO)) { this->m_rsdco_pl = data; }
	else if (StringMatch(sk, VAR_SOL_RSDIN)) { this->m_sol_rsdin = data; }
	else if (StringMatch(sk, VAR_SOL_COV)) { this->m_sol_cov = data; }
	else if (StringMatch(sk, VAR_SOILLAYERS)) { this->m_nSoilLayers = data; }
	else if (StringMatch(sk, VAR_SOTE)) { this->m_sote = data; }
	else if (StringMatch(sk, VAR_A_DAYS)) { this->m_a_days = data; }
	else if (StringMatch(sk, VAR_B_DAYS)) { this->m_b_days = data; }
    else
        throw ModelException(MID_MINRL, "Set1DData", "Parameter " + sk +
                                                    " does not exist. Please contact the module developer.");
}

void NandPim::Set2DData(const char *key, int nRows, int nCols, float **data)
{
    if (!this->CheckInputSize(key, nRows)) return;
    string sk(key);
    m_soilLayers = nCols;
	if (StringMatch(sk, VAR_SOL_CBN)) { this->m_sol_cbn = data; }
	else if (StringMatch(sk, VAR_SOL_BD)) { this->m_sol_bd = data; }
	else if (StringMatch(sk, VAR_CLAY)) { this->m_sol_clay = data; }
	else if (StringMatch(sk, VAR_ROCK)) { this->m_sol_rock = data; }
    else if (StringMatch(sk, VAR_SOL_ST)) { this->m_soilStorage = data; }
    else if (StringMatch(sk, VAR_SOL_AWC)) { this->m_sol_awc = data; }
	else if (StringMatch(sk, VAR_SOL_NO3)) { this->m_sol_no3 = data; }
	else if (StringMatch(sk, VAR_SOL_NH4)) { this->m_sol_nh4 = data; }
    else if (StringMatch(sk, VAR_SOL_SORGN)) { this->m_sol_orgn = data; }
    else if (StringMatch(sk, VAR_SOL_HORGP)) { this->m_sol_orgp = data; }
    else if (StringMatch(sk, VAR_SOL_SOLP)) { this->m_sol_solp = data; }
    else if (StringMatch(sk, VAR_SOL_WPMM)) { this->m_sol_wpmm = data; }
	else if (StringMatch(sk, VAR_SOILDEPTH)) { this->m_sol_z = data; }
	else if (StringMatch(sk, VAR_SOILTHICK)) { this->m_sol_thick = data; }
	else if (StringMatch(sk, VAR_SOL_RSD)) this->m_sol_rsd = data;
    else
        throw ModelException(MID_MINRL, "Set2DData", "Parameter " + sk + " does not exist.");
}

void NandPim::initialOutputs()
{
	if(m_hmntl == NULL) Initialize1DArray(m_nCells, m_hmntl, 0.f);
	if(m_hmptl == NULL) Initialize1DArray(m_nCells, m_hmptl, 0.f);
	if(m_rmn2tl == NULL) Initialize1DArray(m_nCells, m_rmn2tl, 0.f);
	if(m_rmptl == NULL) Initialize1DArray(m_nCells, m_rmptl, 0.f);
	if(m_rwntl == NULL) Initialize1DArray(m_nCells, m_rwntl, 0.f);
	if(m_wdntl == NULL) Initialize1DArray(m_nCells, m_wdntl, 0.f);
	if(m_rmp1tl == NULL) Initialize1DArray(m_nCells, m_rmp1tl, 0.f);
	if(m_roctl == NULL) Initialize1DArray(m_nCells, m_roctl, 0.f);
	if(m_a_days == NULL) Initialize1DArray(m_nCells, m_a_days, 0.f);
	if(m_b_days == NULL) Initialize1DArray(m_nCells, m_b_days, 0.f);
	if(m_sol_cov == NULL || m_sol_rsd == NULL)
	{
		Initialize1DArray(m_nCells, m_sol_cov, m_sol_rsdin);
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_rsd, 0.f);
#pragma omp parallel for
		for (int i = 0; i < m_nCells; i++)
			m_sol_rsd[i][0] = m_sol_cov[i];
	}
	// initial input soil chemical in first run
	if(m_sol_no3 == NULL) Initialize2DArray(m_nCells, m_soilLayers, m_sol_no3, 0.f);
	if(m_sol_fon == NULL || m_sol_fop == NULL || m_sol_aorgn == NULL || 
		m_sol_actp == NULL || m_sol_stap == NULL) 
	{
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_fon, 0.f);
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_fop, 0.f);
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_aorgn, 0.f);
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_actp, 0.f);
		Initialize2DArray(m_nCells, m_soilLayers, m_sol_stap, 0.f);

#pragma omp parallel for
		 for (int i = 0; i < m_nCells; i++)
		 {
			 // fresh organic P / N
			 m_sol_fop[i][0] = m_sol_cov[i] * .0010f;
			 m_sol_fon[i][0] = m_sol_cov[i] * .0055f;

			 for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
			 {
				float wt1 = 0.f;
				float conv_wt = 0.f;
				// mg/kg => kg/ha
				wt1 = m_sol_bd[i][k] * m_sol_thick[i][k] / 100.f;
				// kg/kg => kg/ha
				conv_wt = 1.e6f * wt1;
			
				/// if m_sol_no3 is not provided, then initialize it.
				if (m_sol_no3[i][k] <= 0.f) 
				{
					m_sol_no3[i][k] = 0.f;
					float zdst = 0.f;
					zdst = exp(-m_sol_z[i][k] / 1000.f);
					m_sol_no3[i][k] = 10.f * zdst * 0.7f;
					m_sol_no3[i][k] *= wt1;	// mg/kg => kg/ha
					//if(k == 0) outfile << m_sol_no3[i][k];
				}
				/// if m_sol_orgn is not provided, then initialize it.
				if (m_sol_orgn[i][k] <=0.f)
				{
					// CN ratio changed back to 14
					m_sol_orgn[i][k] = 10000.f * (m_sol_cbn[i][k] / 14.f) * wt1;
				}
				// assume C:N ratio of 10:1
				// nitrogen active pool fraction (nactfr)
				float nactfr = .02f;
				m_sol_aorgn[i][k] = m_sol_orgn[i][k] * nactfr;
				m_sol_orgn[i][k] *= (1.f - nactfr);

				// currently not used
				//sumorgn = sumorgn + m_sol_aorgn[i][k] + m_sol_orgn[i][k] + m_sol_fon[i][k];

				if (m_sol_orgp[i][k] <= 0.f)
				{
					// assume N:P ratio of 8:1
					m_sol_orgp[i][k] = 0.125f * m_sol_orgn[i][k];
				}
			
				if (m_sol_solp[i][k] <= 0.f) 
				{
					// assume initial concentration of 5 mg/kg
					m_sol_solp[i][k] = 5.f * wt1;
				}

				float solp = 0.f;
				float actp = 0.f;
				float psp = m_psp;
				if (m_solP_model == 0)// Set active pool based on dynamic PSP MJW
				{
					// Allow Dynamic PSP Ratio
					if (conv_wt != 0) solp = (m_sol_solp[i][k] / conv_wt) * 1000000.f;
					else
						throw ModelException(MID_MINRL, "initialOutputs", "Please check the bulk density and soil thickness data.");
					if (m_sol_clay[i][k] > 0.f)
					{
						psp = -0.045f * log(m_sol_clay[i][k]) + (0.001f * solp);
						psp = psp - (0.035f * m_sol_cbn[i][k]) + 0.43f;
					} else
					{
						psp = 0.4f;
					}
					// Limit PSP range
					if (psp < .05f) psp = 0.05f; 
					else if(psp > 0.9f) psp = 0.9f;
				}
			
				m_sol_actp[i][k] = m_sol_solp[i][k] * (1.f - psp) / psp;

				if (m_solP_model == 0) // Set Stable pool based on dynamic coefficient
				{                      // From White et al 2009 
					// convert to concentration for ssp calculation
					actp = m_sol_actp[i][k] / conv_wt * 1000000.f;
					solp = m_sol_solp[i][k] / conv_wt * 1000000.f;
					// estimate Total Mineral P in this soil based on data from sharpley 2004
					float ssp = 0.;
					ssp = 25.044f * pow((actp + solp), -0.3833f);
					// limit SSP Range
					if (ssp > 7.f) ssp = 7.f;
					if (ssp < 1.f) ssp = 1.f;
					// define stableP
					m_sol_stap[i][k] = ssp * (m_sol_actp[i][k] + m_sol_solp[i][k]);
				} else
				{
					// The original code
					m_sol_stap[i][k] = 4.f * m_sol_actp[i][k];
				}
				//m_sol_hum[i][k] = m_sol_cbn[i][k] * wt1 * 17200.f;
				//summinp = summinp + m_sol_solp[i][k] + m_sol_actp[i][k] + m_sol_stap[i][k];
				//sumorgp = sumorgp + m_sol_orgp[i][k] + m_sol_fop[i][k];
			 }
		 }
	}
	/**** initialization of CENTURY model related variables *****/
	/****                soil_chem.f of SWAT                *****/
	if (m_CbnModel == 2)
	{
		/// definition of temporary parameters
		float sol_mass = 0.f, FBM = 0.f, FHP = 0.f, FHS = 0.f;
		float X1 = 0.f, RTO = 0.f, sol_min_n = 0.f;
		if(m_sol_WOC == NULL) 
		{
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_WOC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_WON, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_BM, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_BMC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_BMN, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HP, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HS, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HSC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HSN, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HPC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_HPN, 0.f);

			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LM, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LMC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LMN, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LSC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LSN, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LS, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LSL, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LSLC, 0.f);
			Initialize2DArray(m_nCells, m_soilLayers, m_sol_LSLNC, 0.f);


			//Initialize2DArray(m_nCells, m_soilLayers, m_sol_RNMN, 0.f);
			//Initialize2DArray(m_nCells, m_soilLayers, m_sol_RSPC, 0.f);

#pragma omp parallel for
			for (int i = 0; i < m_nCells; i++)
			{
				for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
				{
					/// soil mass in each layer, kg/ha
					sol_mass = 10000.f * m_sol_thick[i][k] * m_sol_bd[i][k] * (1.f - m_sol_rock[i][k] / 100.f);
					/// mineral nitrogen, kg/ha
					sol_min_n = m_sol_no3[i][k] + m_sol_nh4[i][k];
					m_sol_WOC[i][k] = sol_mass * m_sol_cbn[i][k] / 100.f;
					m_sol_WON[i][k] = m_sol_aorgn[i][k] + m_sol_orgn[i][k];
					/// fraction of Mirobial biomass, humus passive C pools
					if (FBM < 1.e-10f) FBM = 0.04f;
					if (FHP < 1.e-10f) FHP = 0.7f - 0.4f * exp(-0.277f * 100.f);
					FHS = 1.f - FBM - FHP;
					m_sol_BM[i][k] = FBM * m_sol_WOC[i][k];
					m_sol_BMC[i][k] = m_sol_BM[i][k];
					RTO = m_sol_WON[i][k] / m_sol_WOC[i][k];
					m_sol_BMN[i][k] = RTO * m_sol_BMC[i][k];
					m_sol_HP[i][k] = FHP * (m_sol_WOC[i][k] - m_sol_BM[i][k]);
					m_sol_HS[i][k] = m_sol_WOC[i][k] - m_sol_BM[i][k] - m_sol_HP[i][k];
					m_sol_HSC[i][k] = m_sol_HS[i][k];
					m_sol_HSN[i][k] = RTO * m_sol_HSC[i][k];
					m_sol_HPC[i][k] = m_sol_HP[i][k]; /// same as sol_aorgn
					m_sol_HPN[i][k] = RTO * m_sol_HPC[i][k]; // same as sol_orgn
					X1 = m_sol_rsd[i][k] / 1000.f;
					m_sol_LM[i][k] = 500.f * X1;
					m_sol_LS[i][k] = m_sol_LM[i][k];
					m_sol_LSL[i][k] = 0.8f * m_sol_LS[i][k];
					m_sol_LMC[i][k] = 0.42f * m_sol_LM[i][k];

					m_sol_LMN[i][k] = 0.1f * m_sol_LMC[i][k];
					m_sol_LSC[i][k] = 0.42f * m_sol_LS[i][k];
					m_sol_LSLC[i][k] = 0.8f * m_sol_LSC[i][k];
					m_sol_LSLNC[i][k] = 0.2f * m_sol_LSC[i][k];
					m_sol_LSN[i][k] = m_sol_LSC[i][k] / 150.f;

					m_sol_WOC[i][k] += m_sol_LSC[i][k] + m_sol_LMC[i][k];
					m_sol_WON[i][k] += m_sol_LSN[i][k] + m_sol_LMN[i][k];

					m_sol_orgn[i][k] = m_sol_HPN[i][k];
					m_sol_aorgn[i][k] = m_sol_HSN[i][k];
					m_sol_fon[i][k] = m_sol_LMN[i][k] + m_sol_LSN[i][k];
				}
			}
		}
	}
    // allocate the output variables
	if(!FloatEqual(m_wshd_dnit, 0.f))
    {
        m_wshd_dnit = 0.f;
        m_wshd_hmn = 0.f;
        m_wshd_hmp = 0.f;
        m_wshd_rmn = 0.f;
        m_wshd_rmp = 0.f;
        m_wshd_rwn = 0.f;
        m_wshd_nitn = 0.f;
        m_wshd_voln = 0.f;
        m_wshd_pal = 0.f;
        m_wshd_pas = 0.f;
    }
}

int NandPim::Execute()
{
    CheckInputData();
    initialOutputs();
#pragma omp parallel for
    for (int i = 0; i < m_nCells; i++)
    {
        //Calculate daily nitrogen and phosphorus mineralization and immobilization
        CalculateMinerandImmobi(i);
        //Calculate daily mineralization (NH3 to NO3) and volatilization of NH3
        CalculateMinerandVolati(i);
        //Calculate P flux between the labile, active mineral and stable mineral P pools
        CalculatePflux(i);
    }
	//cout<<"MINRL, cell id 5878, sol_no3[0]: "<<m_sol_no3[5878][0]<<endl;
    return 0;
}

void NandPim::CalculateMinerandImmobi(int i)
{
    //soil layer (k)
    for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
    {
        //soil layer used to compute soil water and soil temperature factors
        int kk = 0;
        if (k == 0)
        {
            kk = 1;
        } else
        {
            kk = k;
        }
        float sut = 0.f;
        // mineralization can occur only if temp above 0 deg
        if (m_sote[i] > 0)
        {
            // compute soil water factor (sut)
            sut = 0.f;
            if (m_soilStorage[i][kk] < 0)
            {
                m_soilStorage[i][kk] = 0.0000001f;
            }
            sut = 0.1f + 0.9f * sqrt(m_soilStorage[i][kk] / m_sol_awc[i][kk]);
            sut = max(0.05f, sut);

            //compute soil temperature factor
            //variable to hold intermediate calculation result (xx)
            float xx = 0.f;
            //soil temperature factor (cdg)
            float cdg = 0.f;
            xx = m_sote[i];
            cdg = 0.9f * xx / (xx + exp(9.93f - 0.312f * xx)) + 0.1f;
            cdg = max(0.1f, cdg);

            // compute combined factor
            xx = 0.f;
            //combined temperature/soil water factor (csf)
            float csf = 0.f;
            xx = cdg * sut;
            if (xx < 0)
            {
                xx = 0.f;
            }
            if (xx > 1000000)
            {
                xx = 1000000.f;
            }
            csf = sqrt(xx);

            // compute flow from active to stable pools
            //amount of nitrogen moving from active organic to stable organic pool in layer (rwn)
            float rwn = 0.f;
            rwn = 0.1e-4f * (m_sol_aorgn[i][k] * (1.f / m_nactfr - 1.f) - m_sol_orgn[i][k]);
            if (rwn > 0.f)
            {
                rwn = min(rwn, m_sol_aorgn[i][k]);
            } else
            {
                rwn = -(min(abs(rwn), m_sol_orgn[i][k]));
            }
            m_sol_orgn[i][k] = max(1.e-6f, m_sol_orgn[i][k] + rwn);
            m_sol_aorgn[i][k] = max(1.e-6f, m_sol_aorgn[i][k] - rwn);

            // compute humus mineralization on active organic n
            //amount of nitrogen moving from active organic nitrogen pool to nitrate pool in layer (hmn)
            float hmn = 0.f;
            hmn = m_cmn * csf * m_sol_aorgn[i][k];
            hmn = min(hmn, m_sol_aorgn[i][k]);
            // compute humus mineralization on active organic p
            xx = 0.f;
            //amount of phosphorus moving from the organic pool to the labile pool in layer (hmp)
            float hmp = 0.f;
            xx = m_sol_orgn[i][k] + m_sol_aorgn[i][k];
            if (xx > 1.e-6f)
            {
                hmp = 1.4f * hmn * m_sol_orgp[i][k] / xx;
            } else
            {
                hmp = 0.f;
            }
            hmp = min(hmp, m_sol_orgp[i][k]);

            // move mineralized nutrients between pools;
            m_sol_aorgn[i][k] = max(1.e-6f, m_sol_aorgn[i][k] - hmn);
            m_sol_no3[i][k] = m_sol_no3[i][k] + hmn;
            m_sol_orgp[i][k] = m_sol_orgp[i][k] - hmp;
            m_sol_solp[i][k] = m_sol_solp[i][k] + hmp;

            // compute residue decomp and mineralization of
            // fresh organic n and p (upper two layers only)
            //amount of nitrogen moving from fresh organic to nitrate(80%) and active organic(20%) pools in layer (rmn1)
            float rmn1 = 0.f;
            //amount of phosphorus moving from fresh organic to labile(80%) and organic(20%) pools in layer (rmp)
            float rmp = 0.f;
            if (k <= 2)
            {
                //the C:N ratio (cnr)
                float cnr = 0.f;
                //the C:P ratio (cnr)
                float cpr = 0.f;
                //the nutrient cycling water factor for layer (ca)
                float ca = 0.f;
                float cnrf = 0.f;
                float cprf = 0.f;
                if (m_sol_fon[i][k] + m_sol_no3[i][k] > 1e-4f)
                {
                    //Calculate cnr, equation 3:1.2.5 in SWAT Theory 2009, p189
                    cnr = 0.58f * m_sol_rsd[i][k] / (m_sol_fon[i][k] + m_sol_no3[i][k]);
                    if (cnr > 500)
                    {
                        cnr = 500.f;
                    }
                    cnrf = exp(-0.693f * (cnr - 25.f) / 25.f);
                } else
                {
                    cnrf = 1.f;
                }
                if (m_sol_fop[i][k] + m_sol_solp[i][k] > 1e-4f)
                {
                    cpr = 0.58f * m_sol_rsd[i][k] / (m_sol_fop[i][k] + m_sol_solp[i][k]);
                    if (cpr > 5000)
                    {
                        cpr = 5000.f;
                    }
                    cprf = 0.f;
                    cprf = exp(-0.693f * (cpr - 200.f) / 200.f);
                } else
                {
                    cprf = 1.f;
                }
                //decay rate constant (decr)
                float decr = 0.f;

                float rdc = 0.f;
                //Calculate ca, equation 3:1.2.8 in SWAT Theory 2009, p190
                ca = min(min(cnrf, cprf), 1.f);
                //if (m_landcover[i] > 0)
                //{
                //    decr = m_rsdco_pl[(int) m_landcover[i]] * ca * csf;
                //} else
                //{
                //    decr = 0.05f;
                //}
                if (m_rsdco_pl[i] < 0.f)
					decr = 0.05f;
				else
					decr = m_rsdco_pl[i] * ca * csf;
                decr = min(decr, 1.f);
                m_sol_rsd[i][k] = max(1.e-6f, m_sol_rsd[i][k]);
                rdc = decr * m_sol_rsd[i][k];
                m_sol_rsd[i][k] = m_sol_rsd[i][k] - rdc;
                if (m_sol_rsd[i][k] < 0)m_sol_rsd[i][k] = 0.f;
                rmn1 = decr * m_sol_fon[i][k];
                m_sol_fop[i][k] = max(1.e-6f, m_sol_fop[i][k]);
                rmp = decr * m_sol_fop[i][k];

                m_sol_fop[i][k] = m_sol_fop[i][k] - rmp;
                m_sol_fon[i][k] = max(1.e-6f, m_sol_fon[i][k]) - rmn1;;
                //Calculate no3, aorgn, solp, orgp, equation 3:1.2.9 in SWAT Theory 2009, p190
                m_sol_no3[i][k] = m_sol_no3[i][k] + 0.8f * rmn1;
                m_sol_aorgn[i][k] = m_sol_aorgn[i][k] + 0.2f * rmn1;
                m_sol_solp[i][k] = m_sol_solp[i][k] + 0.8f * rmp;
                m_sol_orgp[i][k] = m_sol_orgp[i][k] + 0.2f * rmp;
            }
            //  compute denitrification
            //amount of nitrogen lost from nitrate pool in layer due to denitrification
            float wdn = 0.f;
            //Calculate wdn, equation 3:1.4.1 and 3:1.4.2 in SWAT Theory 2009, p194
            if (sut >= m_sdnco)
            {
                wdn = m_sol_no3[i][k] * (1.f - exp(-m_cdn * cdg * m_sol_cbn[i][k]));
            } else
            {
                wdn = 0.f;
            }
            m_sol_no3[i][k] = m_sol_no3[i][k] - wdn;

            // Summary calculations
            m_wshd_hmn = m_wshd_hmn + hmn * (1.f / m_nCells);
            m_wshd_rwn = m_wshd_rwn + rwn * (1.f / m_nCells);
            m_wshd_hmp = m_wshd_hmp + hmp * (1.f / m_nCells);
            m_wshd_rmn = m_wshd_rmn + rmn1 * (1.f / m_nCells);
            m_wshd_rmp = m_wshd_rmp + rmp * (1.f / m_nCells);
            m_wshd_dnit = m_wshd_dnit + wdn * (1.f / m_nCells);
            //m_hmntl = m_hmntl + hmn;
            //m_rwntl = m_rwntl + rwn;
            //m_hmptl = m_hmptl + hmp;
            //m_rmn2tl = m_rmn2tl + rmn1;
            //m_rmptl = m_rmptl + rmp;
            //m_wdntl = m_wdntl + wdn;
			m_hmntl[i] = hmn;
			m_rwntl[i] = rwn;
			m_hmptl[i] = hmp;
			m_rmn2tl[i] = rmn1;
			m_rmptl[i] = rmp;
			m_wdntl[i] = wdn;
        }
    }
}

void NandPim::CalculateMinerandVolati(int i)
{
    //soil layer (k)
    float kk = 0.f;
    for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
    {
        //nitrification/volatilization temperature factor (nvtf)
        float nvtf = 0.f;
        //Calculate nvtf, equation 3:1.3.1 in SWAT Theory 2009, p192
        nvtf = 0.41f * (m_sote[i] - 5.f) / 10.f;
        if (m_sol_nh4[i][k] > 0.f && nvtf >= 0.001f)
        {
            float sw25 = 0.f;
            float swwp = 0.f;
            //nitrification soil water factor (swf)
            float swf = 0.f;
            //Calculate nvtf, equation 3:1.3.2 and 3:1.3.3 in SWAT Theory 2009, p192
            sw25 = m_sol_wpmm[i][k] + 0.25f * m_sol_awc[i][k];
            swwp = m_sol_wpmm[i][k] + m_soilStorage[i][k];
            if (swwp < sw25)
            {
                swf = (swwp - m_sol_wpmm[i][k]) / (sw25 - m_sol_wpmm[i][k]);
            } else
            {
                swf = 1.f;
            }

            if (k == 0)
            {
                kk = 0.f;
            } else
            {
                kk = m_sol_z[i][k - 1];
            }
            //depth from the soil surface to the middle of the layer (dmidl/mm)
            float dmidl = 0.f;
            //volatilization depth factor (dpf)
            float dpf = 0.f;
            //nitrification regulator (akn)
            float akn = 0.f;
            //volatilization regulator (akn)
            float akv = 0.f;
            //amount of ammonium converted via nitrification and volatilization in layer (rnv)
            float rnv = 0.f;
            //amount of nitrogen moving from the NH3 to the NO3 pool (nitrification) in the layer (rnit)
            float rnit = 0.f;
            //amount of nitrogen lost from the NH3 pool due to volatilization (rvol)
            float rvol = 0.f;
            //volatilization CEC factor (cecf)
            float cecf = 0.15f;
            //Calculate nvtf, equation 3:1.3.2 and 3:1.3.3 in SWAT Theory 2009, p192
            dmidl = (m_sol_z[i][k] + kk) / 2.f;
            dpf = 1.f - dmidl / (dmidl + exp(4.706f - 0.0305f * dmidl));
            //Calculate rnv, equation 3:1.3.6, 3:1.3.7 and 3:1.3.8 in SWAT Theory 2009, p193
            akn = nvtf * swf;
            akv = nvtf * dpf * cecf;
            rnv = m_sol_nh4[i][k] * (1.f - exp(-akn - akv));
            //Calculate rnit, equation 3:1.3.9 in SWAT Theory 2009, p193
            rnit = 1.f - exp(-akn);
            //Calculate rvol, equation 3:1.3.10 in SWAT Theory 2009, p193
            rvol = 1.f - exp(-akv);

            //calculate nitrification (NH3 => NO3)
            if ((rvol + rnit) > 1.e-6f)
            {
                //Calculate the amount of nitrogen removed from the ammonium pool by nitrification,
                //equation 3:1.3.11 in SWAT Theory 2009, p193
                rvol = rnv * rvol / (rvol + rnit);
                //Calculate the amount of nitrogen removed from the ammonium pool by volatilization,
                //equation 3:1.3.12 in SWAT Theory 2009, p194
                rnit = rnv - rvol;
                if (rnit < 0)rnit = 0.f;
                m_sol_nh4[i][k] = max(1e-6f, m_sol_nh4[i][k] - rnit);
            }
            if (m_sol_nh4[i][k] < 0)
            {
                rnit = rnit + m_sol_nh4[i][k];
                m_sol_nh4[i][k] = 0.f;
            }
            m_sol_no3[i][k] = m_sol_no3[i][k] + rnit;
            //calculate ammonia volatilization
            m_sol_nh4[i][k] = max(1e-6f, m_sol_nh4[i][k] - rvol);
            if (m_sol_nh4[i][k] < 0)
            {
                rvol = rvol + m_sol_nh4[i][k];
                m_sol_nh4[i][k] = 0.f;
            }
            //summary calculations
            m_wshd_voln += rvol * (1.f / m_nCells);
            m_wshd_nitn += rnit * (1.f / m_nCells);
        }
    }
}

void NandPim::CalculatePflux(int i)
{
	float wt1 = 0.f;
	float conv_wt = 0.f;
    for (int k = 0; k < (int)m_nSoilLayers[i]; k++)
    {
		// mg/kg => kg/ha
		wt1 = m_sol_bd[i][k] * m_sol_thick[i][k] / 100.f;
		// kg/kg => kg/ha
		conv_wt = 1.e6f * wt1;

		// make sure that no zero or negative pool values come in
		if (m_sol_solp[i][k] <= 1.e-6) m_sol_solp[i][k] = 1.e-6f;
		if (m_sol_actp[i][k] <= 1.e-6) m_sol_actp[i][k] = 1.e-6f;
		if (m_sol_stap[i][k] <= 1.e-6) m_sol_stap[i][k] = 1.e-6f;

		// Convert kg/ha to ppm so that it is more meaningful to compare between soil layers
		float solp = 0.f;
		float actp = 0.f;
		float stap = 0.f;
		solp = m_sol_solp[i][k] / conv_wt * 1000000.f;
		actp = m_sol_actp[i][k] / conv_wt * 1000000.f;
		stap = m_sol_stap[i][k]/ conv_wt * 1000000.f;

		// ***************Soluble - Active Transformations***************	
		// Dynamic PSP Ratio
		float psp = 0.f;
		//PSP = -0.045*log (% clay) + 0.001*(Solution P, mg kg-1) - 0.035*(% Organic C) + 0.43
		if (m_sol_clay[i][k] > 0.f)
		{
			psp = -0.045f * log(m_sol_clay[i][k])+ (0.001f * solp) ;
			psp = psp - (0.035f  * m_sol_cbn[i][k]) + 0.43f;
		} else 
		{
			psp = 0.4f;
		} 		
		// Limit PSP range
		if (psp <.1f)  psp = 0.1f;
		if (psp > 0.7f)  psp = 0.7f;

		// Calculate smoothed PSP average 
		if (m_psp > 0.f) psp = (m_psp * 29.f + psp * 1.f) / 30;
		// Store PSP for tomarrows smoothing calculation
		m_psp = psp;

		//***************Dynamic Active/Soluble Transformation Coeff******************
		// Calculate P balance
		float rto = psp / (1.f - psp);
		float rmn1 = 0.f;
		rmn1 = m_sol_solp[i][k] - m_sol_actp[i][k] * rto; // P imbalance
		// Move P between the soluble and active pools based on vadas et al., 2006
		if (rmn1 >= 0.f) // Net movement from soluble to active	
		{
			rmn1 = max(rmn1, (-1 * m_sol_solp[i][k]));
			// Calculate dynamic coefficient		
			float vara = 0.918f * (exp(-4.603f * psp));          
			float varb = (-0.238f * log(vara)) - 1.126f;
			float arate = 0.f;
			if (m_a_days[i] >0)
			{
				arate = vara * pow(m_a_days[i], varb);
			} 
			else
			{
				arate = vara ; // ? arate = vara * (1) ** varb; ?
			}
			// limit rate coefficient from 0.05 to .5 helps on day 1 when a_days is zero
			if (arate > 0.5f) arate  = 0.5f;
			if (arate < 0.1f) arate  = 0.1f;
			rmn1 = arate * rmn1;
			m_a_days[i] = m_a_days[i]  + 1.f; // add a day to the imbalance counter
			m_b_days[i] = 0;
		}

		if (rmn1 < 0.f) // Net movement from Active to Soluble
		{
			rmn1 = min(rmn1, m_sol_actp[i][k]);
			// Calculate dynamic coefficient
			float base = 0.f;
			float varc = 0.f;
			base = (-1.08f * psp) + 0.79f;
			varc = base * (exp (-0.29f));
			// limit varc from 0.1 to 1
			if (varc > 1.f) varc  = 1.f;
			if (varc < .1f) varc  = .1f;
			rmn1 = rmn1 * varc;
			m_a_days[i] = 0.f;
			m_b_days[i] = m_b_days[i]  + 1.f; // add a day to the imbalance counter
		}

		//*************** Active - Stable Transformations ******************
		// Estimate active stable transformation rate coeff
		// original value was .0006
		// based on linear regression rate coeff = 0.005 @ 0% CaCo3 0.05 @ 20% CaCo3
		float as_p_coeff = 0.f;
		float sol_cal = 2.8f;
		as_p_coeff = 0.0023f * sol_cal + 0.005f;
		if (as_p_coeff > 0.05f) as_p_coeff = 0.05f;
		if (as_p_coeff < 0.002f) as_p_coeff = 0.002f;
		// Estimate active/stable pool ratio
		// Generated from sharpley 2003
		float xx = 0.f;
		float ssp = 0.f;
		xx = actp + (actp * rto);
		if (xx > 1.e-6f) ssp = 25.044f * pow(xx, -0.3833f);
		// limit ssp to range in measured data
		if (ssp > 10.f) ssp = 10.f;
		if (ssp < 0.7f) ssp = 0.7f;
		// Smooth ssp, no rapid changes
		if (m_ssp > 0.f) ssp = (ssp + m_ssp * 99.f) / 100.f;
		float roc = 0.f;
		roc = ssp * (m_sol_actp[i][k] + m_sol_actp[i][k] * rto);
		roc = roc - m_sol_stap[i][k];
		roc = as_p_coeff * roc;
		// Store todays ssp for tomarrow's calculation
		m_ssp = ssp;

		// **************** Account for Soil Water content, do not allow movement in dry soil************
		float wetness = 0.f;
		wetness = (m_soilStorage[i][k] / m_sol_awc[i][k]); // range from 0-1 1 = field cap
		if (wetness >1.f)  wetness = 1.f;
		if (wetness <0.25f)  wetness = 0.25f;
		rmn1 = rmn1 * wetness;
		roc  = roc  * wetness;

		// If total P is greater than 10,000 mg/kg do not allow transformations at all
		if ((solp + actp + stap) < 10000.f) 
		{
			// Allow P Transformations
			m_sol_stap[i][k] = m_sol_stap[i][k] + roc;
			if (m_sol_stap[i][k] < 0.f) m_sol_stap[i][k] = 0.f;
			m_sol_actp[i][k] = m_sol_actp[i][k] - roc + rmn1;
			if (m_sol_actp[i][k] < 0.f) m_sol_actp[i][k] = 0.f;
			m_sol_solp[i][k] = m_sol_solp[i][k] - rmn1;
			if (m_sol_solp[i][k] < 0.f) m_sol_solp[i][k] = 0.f;

			// Add water soluble P pool assume 1:5 ratio based on sharpley 2005 et al
			m_wshd_pas += roc * (1.f / m_nCells);
			m_wshd_pal += rmn1 * (1.f / m_nCells);
			m_roctl[i] += roc;
			m_rmp1tl[i] += rmn1;
		}
		
		//////////////////////// pminrl.f ///////////////////////////
// 		float bk = .0006f;
//         float rmn1 = 0.f;
//         rmn1 = (m_sol_solp[i][k] - m_sol_actp[i][k] * rto);
//         if (rmn1 > 0.f) rmn1 *= 0.1f;
//         if (rmn1 < 0.f)rmn1 *= 0.6f;
//         rmn1 = min(rmn1, m_sol_solp[i][k]);
//         //amount of phosphorus moving from the active mineral to the stable mineral pool in the soil layer (roc)
//         float roc = 0;
//         //Calculate roc, equation 3:2.3.4 and 3:2.3.5 in SWAT Theory 2009, p215
//         roc = bk * (4.f * m_sol_actp[i][k] - m_sol_stap[i][k]);
//         if (roc < 0)roc *= 0.1f;
//         roc = min(roc, m_sol_actp[i][k]);
//         m_sol_stap[i][k] = m_sol_stap[i][k] + roc;
//         if (m_sol_stap[i][k] < 0)m_sol_stap[i][k] = 0.f;
//         m_sol_actp[i][k] = m_sol_actp[i][k] - roc + rmn1;
//         if (m_sol_actp[i][k] < 0)m_sol_actp[i][k] = 0.f;
//         m_sol_solp[i][k] = m_sol_solp[i][k] - rmn1;
//         if (m_sol_solp[i][k] < 0)m_sol_solp[i][k] = 0.f;
//         m_wshd_pas += roc * (1.f / m_nCells);
//         m_wshd_pal += rmn1 * (1.f / m_nCells);
//         m_roctl[i] += roc;
//         m_rmp1tl[i] += rmn1;
    }
}

void NandPim::CalculateCflux(int i)
{//TODO

}

void NandPim::GetValue(const char *key, float *value)
{
    string sk(key);
    if (StringMatch(sk, VAR_WSHD_DNIT)) { *value = this->m_wshd_dnit; }
    else if (StringMatch(sk, VAR_WSHD_HMN)) { *value = this->m_wshd_hmn; }
    else if (StringMatch(sk, VAR_WSHD_HMP)) { *value = this->m_wshd_hmp; }
    else if (StringMatch(sk, VAR_WSHD_RMN)) { *value = this->m_wshd_rmn; }
    else if (StringMatch(sk, VAR_WSHD_RMP)) { *value = this->m_wshd_rmp; }
    else if (StringMatch(sk, VAR_WSHD_RWN)) { *value = this->m_wshd_rwn; }
    else if (StringMatch(sk, VAR_WSHD_NITN)) { *value = this->m_wshd_nitn; }
    else if (StringMatch(sk, VAR_WSHD_VOLN)) { *value = this->m_wshd_voln; }
    else if (StringMatch(sk, VAR_WSHD_PAL)) { *value = this->m_wshd_pal; }
    else if (StringMatch(sk, VAR_WSHD_PAS)) { *value = this->m_wshd_pas; }
    else
        throw ModelException(MID_MINRL, "GetValue", "Parameter " + sk + " does not exist.");
}

void NandPim::Get1DData(const char *key, int *n, float **data)
{
	initialOutputs();
	string sk(key);
	if (StringMatch(sk, VAR_HMNTL)) { *data = this->m_hmntl; }
	else if (StringMatch(sk, VAR_HMPTL)) { *data = this->m_hmptl; }
	else if (StringMatch(sk, VAR_RMN2TL)) { *data = this->m_rmn2tl; }
	else if (StringMatch(sk, VAR_RMPTL)) { *data = this->m_rmptl; }
	else if (StringMatch(sk, VAR_RWNTL)) { *data = this->m_rwntl; }
	else if (StringMatch(sk, VAR_WDNTL)) { *data = this->m_wdntl; }
	else if (StringMatch(sk, VAR_RMP1TL)) { *data = this->m_rmp1tl; }
	else if (StringMatch(sk, VAR_ROCTL)) { *data = this->m_roctl; }
	else if (StringMatch(sk, VAR_SOL_COV)){*data = this->m_sol_cov;}
	else if (StringMatch(sk, VAR_A_DAYS)){*data = this->m_a_days;}
	else if (StringMatch(sk, VAR_B_DAYS)){*data = this->m_b_days;}
	else
		throw ModelException(MID_MINRL, "Get1DData", "Parameter " + sk + " does not exist.");
	*n = this->m_nCells;
}

void NandPim::Get2DData(const char *key, int *nRows, int *nCols, float ***data)
{
	initialOutputs();
    string sk(key);
    *nRows = m_nCells;
    *nCols = m_soilLayers;
    if (StringMatch(sk, VAR_SOL_AORGN)) { *data = this->m_sol_aorgn; }
    else if (StringMatch(sk, VAR_SOL_FORGN)) { *data = this->m_sol_fon; }
    else if (StringMatch(sk, VAR_SOL_FORGP)) { *data = this->m_sol_fop; }
    else if (StringMatch(sk, VAR_SOL_NO3)) { *data = this->m_sol_no3; }
    else if (StringMatch(sk, VAR_SOL_NH4)) { *data = this->m_sol_nh4; }
    else if (StringMatch(sk, VAR_SOL_SORGN)) { *data = this->m_sol_orgn; }
    else if (StringMatch(sk, VAR_SOL_HORGP)) { *data = this->m_sol_orgp; }
    else if (StringMatch(sk, VAR_SOL_SOLP)) { *data = this->m_sol_solp; }
    else if (StringMatch(sk, VAR_SOL_ACTP)) { *data = this->m_sol_actp; }
    else if (StringMatch(sk, VAR_SOL_STAP)) { *data = this->m_sol_stap; }
	else if (StringMatch(sk, VAR_SOL_RSD)) {*data = this->m_sol_rsd;}
	/// 2-CENTURY C/N cycling related variables
	else if (StringMatch(sk, VAR_SOL_WOC)) {*data = this->m_sol_WOC;}
	else if (StringMatch(sk, VAR_SOL_WON)) {*data = this->m_sol_WON;}
	else if (StringMatch(sk, VAR_SOL_BM)) {*data = this->m_sol_BM;}
	else if (StringMatch(sk, VAR_SOL_BMC)) {*data = this->m_sol_BMC;}
	else if (StringMatch(sk, VAR_SOL_BMN)) {*data = this->m_sol_BMN;}
	else if (StringMatch(sk, VAR_SOL_HP)) {*data = this->m_sol_HP;}
	else if (StringMatch(sk, VAR_SOL_HS)) {*data = this->m_sol_HS;}
	else if (StringMatch(sk, VAR_SOL_HSC)) {*data = this->m_sol_HSC;}
	else if (StringMatch(sk, VAR_SOL_HSN)) {*data = this->m_sol_HSN;}
	else if (StringMatch(sk, VAR_SOL_HPC)) {*data = this->m_sol_HPC;}
	else if (StringMatch(sk, VAR_SOL_HPN)) {*data = this->m_sol_HPN;}
	else if (StringMatch(sk, VAR_SOL_LM)) {*data = this->m_sol_LM;}
	else if (StringMatch(sk, VAR_SOL_LMC)) {*data = this->m_sol_LMC;}
	else if (StringMatch(sk, VAR_SOL_LMN)) {*data = this->m_sol_LMN;}
	else if (StringMatch(sk, VAR_SOL_LSC)) {*data = this->m_sol_LSC;}
	else if (StringMatch(sk, VAR_SOL_LSN)) {*data = this->m_sol_LSN;}
	else if (StringMatch(sk, VAR_SOL_LS)) {*data = this->m_sol_LS;}
	else if (StringMatch(sk, VAR_SOL_LSL)) {*data = this->m_sol_LSL;}
	else if (StringMatch(sk, VAR_SOL_LSLC)) {*data = this->m_sol_LSLC;}
	else if (StringMatch(sk, VAR_SOL_LSLNC)) {*data = this->m_sol_LSLNC;}
	// need to be confirmed by huiran.
	//else if (StringMatch(sk, VAR_SOL_RNMN)) {*data = this->m_sol_RNMN;}
	//else if (StringMatch(sk, VAR_SOL_RSPC)) {*data = this->m_sol_RSPC;}
    else
        throw ModelException(MID_MINRL, "Get2DData", "Parameter " + sk + " does not exist.");
}
