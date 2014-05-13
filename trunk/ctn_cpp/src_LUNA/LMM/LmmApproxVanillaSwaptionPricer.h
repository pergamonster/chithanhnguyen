#pragma once

#include "Dispersion.h"
#include "VanillaSwaption.h"
#include "NumericalMethods.h"
#include "Lmm.h"
#include "VanillaSwapPricer.h"
#include <cassert>


//-- Class defining approx methods for swaption pricing  
class LmmApproxVanillaSwaptionPricer : public VanillaSwapPricer // to use the precalculation.
{
private:
    Lmm_PTR lmm_;

	//! Working place: pre-calculation: calculate much more things but who cares :) 
	size_t horizon_;
	std::vector<double> ZC_;      // ZC_[i] = P(T_0, T_i), so ZC_[0] = 1.0
	//std::vector<double> omega0_;  // omega_[i] = deltaT_floating[i] P(T_0,T_{i+1}) / annuity

public:
	//! constructor  
	LmmApproxVanillaSwaptionPricer::LmmApproxVanillaSwaptionPricer(const Lmm_PTR& lmmModel)
		:VanillaSwapPricer(lmmModel->get_LMMTenorStructure()),
		 lmm_(lmmModel),
		 horizon_(lmm_->get_horizon()),
		 ZC_(horizon_+2)
	{
		assert(lmm_->get_LMMTenorStructure().get_tenorDate()[0] == 0.0);
		calculateZC();
	}

	double volBlack(const VanillaSwaption& vanillaSwaption, const std::vector<double>& liborsInitValue) const;
private:
	void   calculateZC();
	double calculateOmega(const VanillaSwaption& vanillaSwaption);
	
	//double annuity0(const VanillaSwap& vanillaSwap)const;
	//double omega0(size_t i) const;
};


	//double omega0(size_t i, 
	//	          double annuity_T0, 
	//			  const std::vector<double>& bonds_T0);

	//double computeRebonatoVolatility(size_t swapStartDateIndex, 
	//								 double shifted_swapRate_T0,
	//                                 double annuity_T0, 
	//                                 const std::vector<size_t>& floatingIndices,
	//	                             const std::vector<double>& bonds_T0,
	//							     const std::vector<double>& libors_T0,
	//							     const std::vector<double>& libor_shifts);

	//double computeRebonatoVolatility(const Array& x,
	//	                             size_t swapStartDateIndex, 		                             
	//                                 double shifted_swapRate_T0,
	//                                 double annuity_T0, 
	//                                 const std::vector<size_t>& floatingIndices,
	//	                             const std::vector<double>& bonds_T0,
	//							     const std::vector<double>& libors_T0,
	//							     const std::vector<double>& libor_shifts);

	//void computeShiftedSwapRateAndStrike(double& out_shifted_swapRate_T0,
	//	                                 double& out_shifted_strike,
	//									 const std::vector<double>& bonds_T0,
	//									 const std::vector<double>& libor_shifts,
	//									 const std::vector<size_t>& floatingIndices,
	//									 double annuity_T0);

	//double RebonatoApprox(size_t swapStartDateIndex,
	//	                  double strike,
	//					  const std::vector<size_t>& floatingIndices,
	//					  double& out_vol_squared,
	//	                  double swapRate_T0, 
	//	                  double annuity_T0,
	//					  const std::vector<double>& bonds_T0,
	//					  const std::vector<double>& libors_T0,
	//					  const std::vector<double>& libor_shifts);


	////-- Computes Rebonato's formula for swaptions
	////-- For calibration
	//double Rebonato_SwaptionPrice(NumericalMethods& NM,
	//	                      double shifted_swapRate_T0, 
	//                          double strike, 
	//						  double vol_squared, 
	//						  double annuity);


	//double AndersenApprox();

	//const std::vector<double>& getTenorDates();
//
//};
