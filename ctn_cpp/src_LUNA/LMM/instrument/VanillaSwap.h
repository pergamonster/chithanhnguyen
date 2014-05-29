#ifndef LMM_INSTRUMENT_VANILLA_SWAP_H
#define LMM_INSTRUMENT_VANILLA_SWAP_H
#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/shared_ptr.hpp>

#include <LMM/Name.h>
#include <LMM/instrument/TenorType.h>


namespace LMM
{

/*! \class VanillaSwap manage leg's INDICES in the simulation structures see <LMM\ModelMCLMM\LMMTenorStructure.h>
 * Each simulation index correspond to a simulated Libor
 *
 * Float Leg's indices                 S      fixedTenor        |       fixedTenor    | 
 * Simulation indices    0--st--1--st--2--st--3--st---4---st---*--st---*--st--*--st--*---st---N+1     // st is the SimulationTenor
 * Fixed Leg's indices                 S  floatTenor   |   floatTenor   | floatTenor  | 
 *
 * S and E are start Index and end Index in the simulation base
 * 
 * (endIndex-startIndex)  has to be multiple of (fixedTenor%st) and (floatTenor%st)
 *
 * !!! For instance, floatTenor=simulationTenor
 */
class VanillaSwap
{
private:

	double strike_;

	LMM::Index indexStart_; // S      
	LMM::Index indexEnd_  ; // E       
		
	/*! The TenorType of Simulation (LMMTenorStructure) has to be a common divisor
	 *  of TenorType for floatingLeg and fixedLeg
	 */
	Tenor floatingLegTenorType_ ; // floatingLeg payment frequency = Libor's tenorType
	Tenor fixedLegTenorType_    ; // fixedLeg payment frequency: each "1M", "3M", "6M", "1Y"
	Tenor simulationTenorType_  ; // simulation tenor step : exmple "1M", "3M", "6M", "1Y"

	size_t floatingVsLiborTenorTypeRatio_; // = floatingLegTenorType_ % simulationTenorType_
	size_t fixedVsLiborTenorTypeRatio_;    // = fixedLegTenorType_    % simulationTenorType_

	std::vector<LMM::Index> floatingLegPaymentIndexSchedule_; // give index in LMMTenorStructure 
	std::vector<LMM::Index> fixedLegPaymentIndexSchedule_   ;    

public:
	
	VanillaSwap(double strike,
		LMM::Index  indexStart, 
		LMM::Index  indexEnd, 
		const Tenor& floatingLegTenorType,		
		const Tenor& fixedLegTenorType,
		const Tenor& lmmTenorStructureTenorType);

	void  set_strike(const double& strike) ;
	const double& get_strike() const       ;
	
	const Tenor& get_fixedLegTenorType()	const;// { return fixedLegTenorType_     ; }
	const Tenor& get_floatingLegTenorType()	const;// { return floatingLegTenorType_  ; }	
	const Tenor& get_simulationTenorType()  const;// { return simulationTenorType_   ; }

	//! get the euclidean quotien of legs's TenorType to simulation's TenorType
	size_t    get_fixedLegTenorLmmTenorRatio() const;// { return    fixedVsLiborTenorTypeRatio_ ; }
	size_t get_floatingLegTenorLmmTenorRatio() const;// { return floatingVsLiborTenorTypeRatio_ ; }		
	
	const std::vector<LMM::Index>& get_floatingLegPaymentIndexSchedule() const;// { return floatingLegPaymentIndexSchedule_; }
	const std::vector<LMM::Index>& get_fixedLegPaymentIndexSchedule()    const;// { return fixedLegPaymentIndexSchedule_   ; }

	LMM::Index get_indexStart() const ;// {return indexStart_;}
	LMM::Index get_indexEnd()   const ;// {return indexEnd_  ;}


public:

	virtual void write_to_stream(std::ostream& outputstream)const ;
};
typedef boost::shared_ptr<VanillaSwap> VanillaSwap_PTR;


} //end namespace LMM

std::ostream& operator<<(std::ostream& os, const LMM::VanillaSwap& swap);

#endif /* LMM_INSTRUMENT_VANILLA_SWAP_H */