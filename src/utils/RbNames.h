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
	extern const std::string   	name 	= "distBeta";
	extern const std::string		alpha 	= "alpha";
	extern const std::string		beta	= "beta";
	}
	
	namespace Double {
	extern const std::string   	name 	= "double";
	}
	
#pragma mark Chi-Square Distribution

	namespace ChiSquare {
	}

	namespace Distribution {
	extern const std::string    name    = "distribution";
	}

#pragma mark Dirichlet Distribution

	namespace Dirichlet {
	}

#pragma mark Exponential Distribution

	namespace Exponential {
	extern const std::string   	name 	= "distExponential";
	extern const std::string		lambda	= "rate";
	}

#pragma mark Gamma Distribution

	namespace Gamma {
	}

#pragma mark LogNormal Distribution

	namespace LogNormal {
	}

	namespace MCMC {
	extern const std::string    name    = "MCMC";
	}

	namespace Model {
	extern const std::string    name    = "model";
	}

#pragma mark Poisson Distribution

	namespace Poisson {
	}

#pragma mark Normal Distribution

	namespace Normal {
	extern const std::string   	name;
	extern const std::string		mu;
	extern const std::string		sigma;
	}

#pragma mark Uniform(0,1) Distribution

	namespace Uniform01 {
	extern const std::string   	name 	= "distUniform01";
	}
	
	namespace Uniform {
	extern const std::string   	name 	= "distUniform";
	extern const std::string		lower	= "lower";
	extern const std::string		upper	= "upper";
	}

	namespace MoveScale {
	extern const std::string   	name 	= "moveScale";
	}
	
	namespace MoveSlidingWindow {
	extern const std::string		name	= "moveSlidingWindow";
	}

}

#endif
