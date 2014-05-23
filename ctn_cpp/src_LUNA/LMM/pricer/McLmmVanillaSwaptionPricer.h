
//! MClmm price Swap, don't need to be too general !!!  
#pragma once
#include <boost/shared_ptr.hpp>

#include <LMM/ModelMCLMM/McLmm.h>        // Model
#include <LMM/instrument/VanillaSwaption.h>  // Instrument
#include <LMM/pricer/McLmmVanillaSwapPricer.h>



class MCLmmVanillaSwaptionPricer : public MCLmmVanillaSwapPricer // to reuse the alreday implemented code in VanillaSwapPricer.
{
public:
	MCLmmVanillaSwaptionPricer(const MCLmm_PTR& mcLmm)
		: MCLmmVanillaSwapPricer(mcLmm)
	{};

	//! Pricing at time T0 = 0
	double price (const VanillaSwaption& vanillaSwaption, size_t nbSimulation)  const;
};