#include <cassert>
#include <vector>

//#include <boost/math/distributions.hpp>
//#include <boost\bind.hpp>
//#include <ql\math\distributions\normaldistribution.hpp>

#include <LMM/pricer/LmmApproxVanillaSwaptionPricer.h>

void LmmApproxVanillaSwaptionPricer::calculateZC()
{
	const std::vector<double>& deltaT = lmm_->get_LMMTenorStructure().get_deltaT();
	const std::vector<double>& liborsInitValue = lmm_->get_liborsInitValue();
	ZC_[0] = 1.0;
	for(size_t i=1; i<ZC_.size(); ++i)
	{
		size_t indexLibor = i-1;
		ZC_[i] = ZC_[i-1]/(1+deltaT[indexLibor]*liborsInitValue[indexLibor]);
	}
}





double LmmApproxVanillaSwaptionPricer::volBlack(const VanillaSwaption& vanillaSwaption,
												const std::vector<double>& liborsInitValue) const
{
	const LMM::VanillaSwap& vanillaSwap = vanillaSwaption.getUnderlyingSwap();

	assert(horizon_ >= vanillaSwap.get_indexEnd()); //! if not cannot price it.
	//! YY TODO: implement the == operator and active this test!
	//assert(lmm_->get_LMMTenorStructure().get_tenorType() == vanillaSwaption.getUnderlyingSwap().get_lmmTenorStructureTenorType());
	precalculate(vanillaSwap);
	                                                                                  
	//! Annuity at time 0
	Name::indexInLMMTenorStructure indexValuationDate = 0;
	std::vector<double> numeraire(ZC_.size());
	for(size_t i=0; i<numeraire.size(); ++i)
	{
		numeraire[i] = 1.0/ZC_[i];
	}
	double annuityValue = annuity(indexValuationDate, vanillaSwap, numeraire);


	//! Omega Vector
	const std::vector<Name::indexInLMMTenorStructure>& floatingLegPaymentIndexSchedule = vanillaSwap.get_floatingLegPaymentIndexSchedule();
	std::vector<double> omega(floatingLegPaymentIndexSchedule.size());
	for(size_t itr=0; itr<floatingLegPaymentIndexSchedule.size(); ++itr)
	{
		//! At time T_{i+1}, pay: delta_t*L_i(T_i)
		size_t floatingLegPaymentIndex = floatingLegPaymentIndexSchedule[itr]; // = i+1
		//size_t indexLibor = floatingLegPaymentIndex-1; // =i, because : floatingTenor = lmmTenor  
		//size_t indexT     = indexLibor;                                        // = i
		double delta_T    = deltaTFloatingLeg_[itr];  // lmmTenorStructure.get_deltaT()[indexLibor]
		omega[itr] = delta_T*ZC_[floatingLegPaymentIndex] / annuityValue;
	}

	//! Robonato Formula: YY TODO: can be simplified: use the symmetric ! 
	Name::indexInLMMTenorStructure swaptionIndexMaturity = vanillaSwaption.get_indexMaturity();
	double volSquare = 0.0;
	for(size_t i=0; i<floatingLegPaymentIndexSchedule.size(); ++i)
	{
		size_t floatingLegPaymentIndex_i = floatingLegPaymentIndexSchedule[i]; // = i+1
		size_t indexLibor_i = floatingLegPaymentIndex_i - 1;
		//size_t indexT_i = indexLibor_i;
		for(size_t j=0; j<floatingLegPaymentIndexSchedule.size(); ++j)
		{
			size_t floatingLegPaymentIndex_j = floatingLegPaymentIndexSchedule[j]; // = i+1
			size_t indexLibor_j = floatingLegPaymentIndex_j - 1;
			//size_t indexT_j     = indexLibor_j;

			// tensor_(k,i,j): L_i, L_j 's integral of vol in [T_{k-1},T_k]

			volSquare += omega[i]*omega[j]*liborsInitValue[indexLibor_i]*liborsInitValue[indexLibor_j]
			             *lmm_->get_cumulatedcovarianceTensor(swaptionIndexMaturity, indexLibor_i, indexLibor_j);
		}
	}

	double swapRate = swapRate_Analytical(vanillaSwap, liborsInitValue); // swapRate at time 0
	double volBlack = std::sqrt(volSquare) * swapRate;

	return volBlack;
}


//LmmApproxSwaptionPricer::SwaptionApproximation(const SwaptionApproximation& approximation)
//{
//	throw("Error: copy cosntructor no implememted");
//	//swapStartDateIndex_ = approximation.swapStartDateIndex_;
//	//swapEndDateIndex_ = approximation.swapEndDateIndex_;
//	//vol_ = approximation.vol_;
//	//tenorDates_ = approximation.tenorDates_;
//}
//
//double Approximation::omega0(size_t i, double annuity_T0, const std::vector<double>& bonds_T0) 
//{
//	double per = (tenorDates_[i+1] - tenorDates_[i]);
//	double res = per * bonds_T0[i+1];
//	return res/annuity_T0;
//}

//
////! suppose: T_0 = 0, ZC[i] = P(T_0,T_i) = P(0,T_i), so ZC[0] = 1.0
//double LmmApproxSwaptionPricer::omega0(size_t indexLibor, 
//									   double annuity_T0, 
//									   const std::vector<double>& ZC) 
//{
//	//double deltaT = (tenorDates_[i+1] - tenorDates_[i]);
//	//double res = per * bonds_T0[i+1];
//	//return res/annuity_T0;
//
//	double floatingLeg = lmmTenorStrucutre_.get_deltaT(indexLibor);
//	double fixingleg   = 
//}
//
//
//
//void LmmApproxSwaptionPricer::computeShiftedSwapRateAndStrike(double& out_shifted_swapRate_T0,
//													double& out_shifted_strike,
//													const std::vector<double>& bonds_T0,
//													const std::vector<double>& libor_shifts,
//													const std::vector<size_t>& floatingIndices,
//													double annuity_T0)
//{
//	//-- Compute shifted swap rate and shifted strike 
//		double swap_shift = 0.0;
//		for each (size_t i in floatingIndices)
//			swap_shift +=  omega0(i-1,annuity_T0,bonds_T0)*libor_shifts[i-1]; //-- Check the index
//		
//		out_shifted_swapRate_T0 += swap_shift;
//		out_shifted_strike += swap_shift;
//}
//
//
//double LmmApproxSwaptionPricer::computeRebonatoVolatility(size_t swapStartDateIndex, 		                                        
//	                                            double shifted_swapRate_T0,
//	                                            double annuity_T0, 
//												const std::vector<size_t>& floatingIndices,
//												const std::vector<double>& bonds_T0,
//												const std::vector<double>& libors_T0,
//												const std::vector<double>& libor_shifts)
//{
//	double T_maturity = tenorDates_[swapStartDateIndex];
//	//-- Compute vol under swap measure
//	double vol_squared = 0.;
//
//	for each (size_t i in floatingIndices) 
//	{
//		double Ti = tenorDates_[i-1];
//		for each (size_t j in floatingIndices)
//		{
//			double Tj = tenorDates_[j-1];
//			double volTmp = vol_->covIntegral(i-1,j-1,Ti,Tj,0,T_maturity);
//			vol_squared += omega0(i-1,annuity_T0,bonds_T0) * omega0(j-1,annuity_T0,bonds_T0) * (libors_T0[i-1]+libor_shifts[i-1]) * (libors_T0[j-1] +libor_shifts[j-1])* volTmp;
//		}
//	}
//
//	vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);
//
//	return vol_squared;
//}
//
//
//double LmmApproxSwaptionPricer::computeRebonatoVolatility(const Array& x,
//	                                            size_t swapStartDateIndex, 												
//	                                            double shifted_swapRate_T0,
//	                                            double annuity_T0, 
//	                                            const std::vector<size_t>& floatingIndices,
//	                                            const std::vector<double>& bonds_T0,
//								                const std::vector<double>& libors_T0,
//								                const std::vector<double>& libor_shifts)
//{
//	double T_maturity = tenorDates_[swapStartDateIndex];
//	//-- Compute vol under swap measure
//	double vol_squared = 0.;
//
//	for each (size_t i in floatingIndices) 
//	{
//		double Ti = tenorDates_[i-1];
//		for each (size_t j in floatingIndices)
//		{
//			double Tj = tenorDates_[j-1];
//			double volTmp = vol_->covIntegral(x,i-1,j-1,Ti,Tj,0,T_maturity);
//			vol_squared += omega0(i-1,annuity_T0,bonds_T0) * omega0(j-1,annuity_T0,bonds_T0) * (libors_T0[i-1]+libor_shifts[i-1]) * (libors_T0[j-1] +libor_shifts[j-1])* volTmp;
//		}
//	}
//
//	vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);
//
//	return vol_squared;
//}
//
////-- Squared volatility from Rebonato's approx is an out parameter
//double LmmApproxSwaptionPricer::RebonatoApprox(size_t swapStartDateIndex,
//	                                 double strike,
//									 const std::vector<size_t>& floatingIndices,
//	                                 double& out_vol_squared,
//	                                 double swapRate_T0, 
//									 double annuity_T0, 
//									 const std::vector<double>& bonds_T0,
//									 const std::vector<double>& libors_T0,
//									 const std::vector<double>& libor_shifts)
//{ 
//	
//	//double strike = (vanillaSwaption.getUnderlyingSwap()).get_strike();
//	
//	//-- In case swap starts at time 0
//	if (swapStartDateIndex == 0) 
//	{
//		return annuity_T0*std::max(swapRate_T0-strike,0.);
//	}
//	else 
//	{
//		//-- Compute shifted swap rate and shifted strike 
//		double swap_shift = 0.0;
//		for each (size_t i in floatingIndices)
//			swap_shift +=  omega0(i-1,annuity_T0,bonds_T0)*libor_shifts[i-1]; //-- Check the index
//		
//		double shifted_swapRate_T0 = swapRate_T0 + swap_shift;
//		double shifted_strike = strike + swap_shift;
//
//		//-- Compute vol under swap measure
//		double T_maturity = tenorDates_[swapStartDateIndex];
//		out_vol_squared = 0.;
//		for each (size_t i in floatingIndices) 
//		{
//			double Ti = tenorDates_[i-1];
//			for each (size_t j in floatingIndices)
//			{
//				double Tj = tenorDates_[j-1];
//				double volTmp = vol_->covIntegral(i-1,j-1,Ti,Tj,0,T_maturity);
//				out_vol_squared += omega0(i-1,annuity_T0,bonds_T0) * omega0(j-1,annuity_T0,bonds_T0) * (libors_T0[i-1]+libor_shifts[i-1]) * (libors_T0[j-1] +libor_shifts[j-1])* volTmp;
//			}
//		}
//
//		out_vol_squared /= (shifted_swapRate_T0*shifted_swapRate_T0);
//		
//		//-- Apply Black formula
//		NumericalMethods blackCapletPricer;
//		double price = blackCapletPricer.BlackFormula(shifted_swapRate_T0,shifted_strike,out_vol_squared,annuity_T0);
//
//		return price;
//	}
//}
//
//
//double LmmApproxSwaptionPricer::Rebonato_SwaptionPrice(NumericalMethods& NM,
//	                                         double swapRate_T0, 
//											 double strike, 
//											 double vol_squared, 
//											 double annuity)
//{
//	return NM.BlackFormula(swapRate_T0,strike,vol_squared,annuity);
//}
//
//double LmmApproxSwaptionPricer::AndersenApprox() {return 0.;}
//
//
//const std::vector<double>& LmmApproxSwaptionPricer::getTenorDates() {return tenorDates_;}