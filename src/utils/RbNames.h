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
    
    namespace AaAlignment {
    const std::string       name    = "aaAlignment";
    }

	namespace AlignmentType {
    const std::string       dna    = "DNA";
    const std::string       aa    = "Amino Acid";
    const std::string       morph    = "Morphology";
	}

	namespace AlignmentFileType {
    const std::string       nexus    = "Nexus";
    const std::string       phylip   = "Phylip";
    const std::string       fasta    = "Fasta";
	}

#pragma mark Beta Distribution

	namespace Beta {
	const std::string   	name 	= "distBeta";
	const std::string		alpha 	= "alpha";
	const std::string		beta	= "beta";
	}
	
    namespace CharacterMatrix {
    const std::string       name    = "characterMatrix";
    }
    
    namespace DnaAlignment {
    const std::string       name    = "dnaAlignment";
    }
    
	namespace Double {
	const std::string   	name 	= "double";
	}
	
#pragma mark Chi-Square Distribution

	namespace ChiSquare {
	}

	namespace Distribution {
	const std::string    name    = "distribution";
	}
	
	namespace DistUnifUnrootedTopology {
	const std::string    name    = "distUniformUnrootedTopology";
	}

#pragma mark Dirichlet Distribution

	namespace Dirichlet {
	}

#pragma mark Exponential Distribution

	namespace Exponential {
	const std::string   	name 	= "distExponential";
	const std::string		lambda	= "rate";
	}

#pragma mark Gamma Distribution

	namespace Gamma {
	}

	namespace Int {
	const std::string      name    = "int";
	    }

#pragma mark LogNormal Distribution

	namespace LogNormal {
	}

	namespace MCMC {
	const std::string    name    = "MCMC";
	}

	namespace Model {
	const std::string    name    = "model";
	}

#pragma mark Poisson Distribution

	namespace Poisson {
	}

#pragma mark Normal Distribution

	namespace Normal {
	const std::string   	name;
	const std::string		mu;
	const std::string		sigma;
	}

#pragma mark Uniform(0,1) Distribution

	namespace Uniform01 {
	const std::string   	name 	= "distUniform01";
	}
	
	namespace Uniform {
	const std::string   	name 	= "distUniform";
	const std::string		lower	= "lower";
	const std::string		upper	= "upper";
	}

	namespace MoveScale {
	const std::string   	name 	= "moveScale";
	}
	
	namespace MoveSlidingWindow {
	const std::string		name	= "moveSlidingWindow";
	}

	namespace Monitor {
	const std::string   	name 	= "monitor";
	}

    namespace DAGNode {
    const std::string       name    = "DAGNode";
    }
    
#pragma mark Undefined Data Type

    namespace Undefined {
    const std::string       name    = "undefined";
    }
    
    namespace UnrootedTopology {
    const std::string       name    = "unrootedTopology";
    }
    
    namespace ReadAlignment {
    const std::string       name    = "readAlignment";
    const std::string       fileType    = "fileType";
    const std::string       alignmentType    = "alignmentType";
    const std::string       fileName    = "alignmentFileName";
    }
    
    namespace String {
    const std::string       name    = "string";
    }

}

#endif
