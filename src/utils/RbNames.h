/*!
* \file
* This file contains commonly used statistics functions that are used
* in REvBayes. The probability density (pdf), log of the probability
* density (lnPdf), cumulative probability (cdf), and quantiles of
* common probability distributions.
*
* \brief Namespace containing statistical functions
*
* (c) Copyright 2009- under GPL version 3
* \date Last modified: $Date: 2009-11-24 18:02:14 -0800 (Tue, 24 Nov 2009) $
 * \author Sebastian Hoehna (Department of Mathematics, Stockholm University)
 * \author John Huelsenbeck (Department of Integrative Biology, University of California, Berkeley)
 * \author Fredrik Ronquist (Department of Entomology, Swedish Museum of Natural History)
* \license GPL version 3
*
* $Id: RbStatistics.h 100 2009-11-25 02:02:14Z jhuelsenbeck $
*/
#ifndef RbNames_H
#define RbNames_H

#include <string>

namespace RbNames {

#pragma mark Beta Distribution

	namespace Beta {
	std::string   	name 	= "distBeta";
	std::string		alpha 	= "alpha";
	std::string		beta	= "beta";
	}
	
	namespace Double {
	std::string   	name 	= "double";
	}
	
#pragma mark Chi-Square Distribution

	namespace ChiSquare {
	}

	namespace Distribution {
	std::string    name    = "distribution";
	}

#pragma mark Dirichlet Distribution

	namespace Dirichlet {
	}

#pragma mark Exponential Distribution

	namespace Exponential {
	std::string   	name 	= "distExponential";
	std::string		lambda	= "rate";
	}

#pragma mark Gamma Distribution

	namespace Gamma {
	}

#pragma mark LogNormal Distribution

	namespace LogNormal {
	}

	namespace MCMC {
	std::string    name    = "MCMC";
	}

	namespace Model {
	std::string    name    = "model";
	}

#pragma mark Poisson Distribution

	namespace Poisson {
	}

#pragma mark Normal Distribution

	namespace Normal {
	std::string   	name 	= "distNormal";
	std::string		mu	 	= "mu";
	std::string		sigma	= "sigma";
	}

#pragma mark Uniform(0,1) Distribution

	namespace Uniform01 {
	std::string   	name 	= "distUniform01";
	}
	
	namespace Uniform {
	std::string   	name 	= "distUniform";
	std::string		lower	= "lower";
	std::string		upper	= "upper";
	}

	namespace MoveScale {
	std::string   	name 	= "moveScale";
	}
	
	namespace MoveSlidingWindow {
	std::string		name	= "moveSlidingWindow";
	}

}

#endif
