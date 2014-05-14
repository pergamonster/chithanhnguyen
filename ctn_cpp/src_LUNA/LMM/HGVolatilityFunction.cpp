#pragma once
#include "HGVolatilityFunction.h"
#include "Printer.h"
#include <cassert>


//! Inner class: h vol
//! Attention: a very slow function.
//! indexLibor = i, indexTime = j
//! libor i, in time [T_{j-1}, T_j]
double HGVolatilityFunction::AbcdPWConstFunction::operator()(size_t indexLibor, size_t indexTime) const// operator(i,j) --return vol--> hij, i>=j
{
	//! check index relation
	if(indexLibor<indexTime)
		throw("Error, index relation error!");

	if(indexLibor<1 || indexLibor>lmmTenorStructure_.get_horizon())  // size1 = horizon
		throw("Error, invalid indexLibor!");

	if(indexTime <1)  // size1 = horizon
		throw("Error, invalid indexTime!");

	//! 
	//double t_begin = lmmTenorStructure_.get_tenorDate(indexTime-1); 
	//double t_end   = lmmTenorStructure_.get_tenorDate(indexTime); 
	//double t       = (t_begin + t_end)/2.0; 
	double t       = lmmTenorStructure_.get_tenorDate(indexTime); 

	double T_i     = lmmTenorStructure_.get_tenorDate(indexLibor);        
	double tau     = T_i-t; 

	return abcdFunction_(tau);
}




// ----------------------------------------------------------------------------------------------------------------
//
//                                           hgVolatilityFunction
//
// ----------------------------------------------------------------------------------------------------------------
//! Constructor
HGVolatilityFunction::HGVolatilityFunction(const AbcdParams& abcdParams,  // h FunctionParam
	                                       const LMMTenorStructure& lmmTenorStructure)       // horizon = N, total number of libor: L_k, k = [0,N]
	:
	 VolatilityFunction(lmmTenorStructure),
     //horizon_(lmmTenorStructure_.get_horizon()),
	 //lmmTenorStructure_(lmmTenorStructure),
	 abcdPWConstFunction_(abcdParams, lmmTenorStructure),
	 horizon_(lmmTenorStructure_.get_horizon()),

	 // YY: A good thing from Adrien, add artificial first row and column (not used!), to make the manipulation of index easier.
	 // piecewise constant vol for: L_k, K = [1,N]
	 hPWConstFunc_(lmmTenorStructure.get_horizon()+1,lmmTenorStructure.get_horizon()+1),   
	 gPWConstFunc_(lmmTenorStructure.get_horizon()+1,lmmTenorStructure.get_horizon()+1)
	 //volCumulated_(lmmTenorStructure.get_horizon()+1,lmmTenorStructure.get_horizon()+1, -9.9e10), 
	 //ifVolisUpToDate_(false)
{
	construct_hPWConstFunc();
	construct_gPWConstFunc();
	//precalculateVolCumulated();
	//ifVolisUpToDate_ = true; //! YY whenever reset h or g function, need to set this param to false, to enforce the recalculation of VolCumulated, 
	//                         //     or other pre-calculated item.
}


void HGVolatilityFunction::construct_hPWConstFunc()
{
	//! forget first row/column
	for(size_t indexLibor=1; indexLibor<hPWConstFunc_.size1(); ++indexLibor)
	{
		for(size_t indexTime=1; indexTime<=indexLibor; ++indexTime)  // triangular matrix
		{
			hPWConstFunc_(indexLibor,indexTime) = abcdPWConstFunction_(indexLibor,indexTime);   
		}
	}
}


void HGVolatilityFunction::construct_gPWConstFunc()
{	
	//! forget first row/column
	for(size_t indexLibor=1; indexLibor<hPWConstFunc_.size1(); ++indexLibor)
	{
		for(size_t indexTime=1; indexTime<=indexLibor; ++indexTime)  // triangular matrix
		{
			gPWConstFunc_(indexLibor,indexTime) = 1.0; //YY TODO: to do the change latter.
		}
	}
}

//void hgVolatilityFunction::precalculateVolCumulated()
//{
//	for(size_t indexLibor=1; indexLibor<hPWConstFunc_.size1(); ++indexLibor)
//	{
//		for(size_t indexTime=1; indexTime<=indexLibor; ++indexTime) 
//		{
//			double vol_PWConstIntanteneous     = hPWConstFunc_(indexLibor, indexTime)*gPWConstFunc_(indexLibor, indexTime);
//			double vol_integralCurrentInterval = vol_PWConstIntanteneous * lmmTenorStructure_.get_deltaT(indexTime); //! L_{i+1}'s integral vol in [T_j,T_{j+1}] 
//			if(indexTime==1)
//				volCumulated_(indexLibor,indexTime) = vol_integralCurrentInterval;
//			else
//			    volCumulated_(indexLibor,indexTime) = volCumulated_(indexLibor,indexTime-1) + vol_integralCurrentInterval;
//		}
//	}
//}


//! vol
//! i = indexLibor
//! j = indexTime
//! volatility of L_i at [T_{j-1},T_j]
double HGVolatilityFunction::operator()(size_t indexLibor, size_t indexTime) const// operator(i,j) --return vol--> hij, i>=j
{
	//if(ifVolisUpToDate_ = false)  // YY: This is needed for calibration! 
	//	throw("Error Vol need to update before the calculation.");

	if(indexLibor<indexTime)
		throw("Error, index relation error!");

	if(indexLibor<1 || indexLibor>hPWConstFunc_.size1())  // size1 = horizon
		throw("Error, invalid indexLibor!");

	if(indexTime <1)  // size1 = horizon
		throw("Error, invalid indexTime!");

	return hPWConstFunc_(indexLibor, indexTime)*gPWConstFunc_(indexLibor, indexTime);
}


//! Whne t in [T_i,T_{i+1}], return index= i+1
size_t HGVolatilityFunction::indexSearch(double t)
{

	 if(t<lmmTenorStructure_.get_tenorDate(0) || t>lmmTenorStructure_.get_tenorDate(horizon_))  // check condition: t in [T_0,T_N]
		for(size_t i=0; i<horizon_; ++i)
		{
			double t1 = lmmTenorStructure_.get_tenorDate(i);
			double t2 = lmmTenorStructure_.get_tenorDate(i+1);
			if(t>=t1 && t<t2)
			{
				return i+1;
			}
		}
}


double HGVolatilityFunction::covIntegral( size_t indexTime_i,
										  size_t indexTime_j,
										  size_t indexLibor_i,
										  size_t indexLibor_j)
{
	assert(indexLibor_i <= indexLibor_j);

	size_t indexLiborMin   = indexLibor_i;
	size_t indexTime_begin = std::min(indexTime_i,indexLiborMin);
	size_t indexTime_end   = std::min(indexTime_j,indexLiborMin);

	double covIntegralValue = 0.0;
	for(size_t indexTime=indexTime_begin; indexTime<indexTime_end; ++indexTime)
	{
		double deltaT = lmmTenorStructure_.get_deltaT(indexTime);
		covIntegralValue += this->operator()(indexLibor_i,indexTime+1) * this->operator()(indexLibor_j,indexTime+1) * deltaT;
	}	
	return covIntegralValue;
}

void HGVolatilityFunction::print() const  // OK: test passed.
{
	std::string fileName = "HGVolatilityFunction.csv";
	std::string path = printPath + fileName;
	std::vector<PrintElement_PTR> elements_print;

	//! Abcd function for h
	PrintElement_PTR abcdParamVector     = PrintElement_PTR(new VectorPrintElement<std::vector<double>>("abcdParamVector",  abcdPWConstFunction_.get_AbcdParams().toVector()));
	PrintElement_PTR abcdFunctionFormula = PrintElement_PTR(new ScalarPrintElement<std::string>("abcdFunctionFormula",  "abcdFunc(tau) = (a+ b*tau)*exp(-c*tau) + d   tau = T_i-t"));
	elements_print.push_back(abcdParamVector);
	elements_print.push_back(abcdFunctionFormula);

	//! h,g,hgMatrix
	typedef boost::numeric::ublas::matrix<double> matrix;
	matrix hMatrix(horizon_+1, horizon_+1);
	matrix gMatrix(horizon_+1, horizon_+1);
	matrix hgMatrix(horizon_+1, horizon_+1);

	for(size_t indexLibor = 1; indexLibor<=horizon_; ++indexLibor)
	{
		for(size_t indexT = 1; indexT<=indexLibor; ++indexT)
		{
			hMatrix(indexLibor,indexT)  = hPWConstFunc_(indexLibor,indexT);
			gMatrix(indexLibor,indexT)  = gPWConstFunc_(indexLibor,indexT);
			hgMatrix(indexLibor,indexT) = this->operator()(indexLibor,indexT);
		}
	}

	//seems a problem of shared ptr polymorphisms ... 
	//PrintElement_PTR tenorType_print    = PrintElement_PTR(new ScalarPrintElement<std::string>("tenorType", tenorType_));
	//PrintElement_PTR horizon_print      = PrintElement_PTR(new ScalarPrintElement<Name::indexInLMMTenorStructure>("horizon", horizon_));
	PrintElement_PTR hMatrix_print   = PrintElement_PTR(new MatrixPrintElement<matrix>("hMatrix",  hMatrix));
	PrintElement_PTR gMatrix_print   = PrintElement_PTR(new MatrixPrintElement<matrix>("gMatrix",  gMatrix));
	PrintElement_PTR hgMatrix_print  = PrintElement_PTR(new MatrixPrintElement<matrix>("hgMatrix", hgMatrix));


	//elements_print.push_back(tenorType_print);
	elements_print.push_back(hMatrix_print);
	elements_print.push_back(gMatrix_print);
	elements_print.push_back(hgMatrix_print);

	Printer printer(path, elements_print);
	printer.print();
}



////! Covariance for different Libor 
////! YY: TODO: need to improve the speed of this function, esp: the CHECK unnecessary.
////            esp when S,T is exactly the grid
//double hgVolatilityFunction::covIntegral(size_t indexLibor_i,
//										 size_t indexLibor_j,
//										 double S, 
//										 double T)
//{
//	if(ifVolisUpToDate_ = false)  // YY: This is needed for calibration! 
//		throw("Error Vol need to update before the calculation.");
//
//	if(S>T)
//		throw("Error: S should less than T");
//
//	if(indexLibor_i>indexLibor_j)  // to make sure: T_i <= T_j
//	{
//		std::swap(indexLibor_i,indexLibor_j);
//	}
//
//	double T_i = lmmTenorStructure_.get_tenorDate(indexLibor_i);
//	double T_j = lmmTenorStructure_.get_tenorDate(indexLibor_j);
//
//	//! int_{t1}^{t2} vol_i(u), vol_j(u) du
//	double min_Tij = std::min(T_i,T_j);
//	double t1 = std::min(S,min_Tij);
//	double t2 = std::min(T,min_Tij);
//	
//	if(std::abs(t1-t2) < 1.0e-16) // YY need a function to treat the diff together ... 
//	{
//		return 0.0;
//	}
//	else
//	{
//		size_t indexSearchT_i = indexSearch(t1);
//		size_t indexSearchT_j = indexSearch(t2);
//
//		for(size_t i=indexSearchT_i; i<indexSearchT_j-1; ++i)
//		{
//
//		}
//
//		double vol_front  = (lmmTenorStructure_.get_tenorDate(indexSearchT_i) - t1);
//		double vol_back   = (lmmTenorStructure_.get_tenorDate(indexSearchT_j) - t2);
//		double vol_middle =  (volCumulated_(indexLibor_i,indexSearchT_j) - volCumulated_(indexLibor_i,indexSearchT_i))
//			                *(volCumulated_(indexLibor_j,indexSearchT_j) - volCumulated_(indexLibor_j,indexSearchT_i))* ;
//	
//		return vol_front + vol_back + vol_middle;
//	}
//}