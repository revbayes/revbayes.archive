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

#define AlignmentFileType_nexus                       "nexus"
#define AlignmentFileType_phylip                      "phylip"
#define AlignmentFileType_fasta                       "fasta"
#define AlignmentType_dna                             "dna"
#define AlignmentType_aa                              "amino acid"
#define AlignmentType_morph                           "morphology"
#define Argument_name                                 "argument"
#define ArgumentRule_name                             "argument rule"
#define ConstantNode_name                             "constant node"
#define DAGNode_name                                  "dag node"
#define DAGNodeContainer_name                         "dag node container"
#define DeterministicNode_name                        "deterministic node"
#define IntVector_name                                "int vector"
#define MoveScale_name                                "move(scale)"
#define MoveSlidingWindow_name                        "move(sliding window)"
#define RbBool_name                                   "bool"
#define RbComplex_name                                "complex"
#define RbDouble_name                                 "double"
#define RbException_name                              "exception"
#define RbFunction_name                               "function"
#define RbFunction_dchisq_name                        "chi-square density"
#define RbFunction_dbeta_name                         "beta density"
#define RbFunction_dexp_name                          "exponential density"
#define RbFunction_dgamma_name                        "gamma density"
#define RbFunction_dnorm_name                         "normal density"
#define RbFunction_pbeta_name                         "beta cdf"
#define RbFunction_pchisq_name                        "chi-square cdf"
#define RbFunction_pexp_name                          "exponential cdf"
#define RbFunction_pnorm_name                         "normal cdf"
#define RbFunction_qbeta_name                         "beta quantile"
#define RbFunction_qchisq_name                        "chi-square quantile"
#define RbFunction_qexp_name                          "exponential quantile"
#define RbFunction_qnorm_name                         "normal quantile"
#define RbFunction_readCharacterMatrix_name           "read alignment"
#define RbFunction_readCharacterMatrix_fileType       "file type"
#define RbFunction_readCharacterMatrix_alignmentType  "alignment type"
#define RbFunction_readCharacterMatrix_fileName       "file name"
#define RbInt_name                                    "int"
#define RbMcmc_name                                   "mcmc"
#define RbMove_name                                   "move"
#define RbMoveSchedule_name                           "move(schedule)"
#define RbObject_name                                 "object"
#define RbPrimitive_name                              "primitive"
#define RbString_name                                 "string"
#define RbTopology_name                               "topology"
#define RbUndefined_name                              "undefined"
#define RbUnrootedTopology_name                       "unrooted topology"
#define StochasticNode_name                           "stochastic node"
#define StringVector_name                             "string vector"
#define RandomNumberGenerator_name                    "random number generator"
#define UserFunction_name                             "user function"



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
	

    namespace Branch {
        const std::string       name    = "branch";
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

	namespace Node {
	    const std::string    name    = "node";
	    }

#pragma mark Poisson Distribution

	namespace Poisson {
	}

#pragma mark Normal Distribution

	namespace Normal {
	const std::string   	name;
	const std::string       dname = "dnorm";
	const std::string       pname = "pnorm";
	const std::string       rname = "rvnorm";
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

    namespace DAGNode {
    const std::string       name    = "dagNode";
    }

    namespace ConstantNode {
    const std::string       name    = "constantNode";
    }

    namespace StochasticNode {
    const std::string       name    = "stochasticNode";
    }

    namespace DeterministicNode {
    const std::string       name    = "deterministcNode";
    }

    namespace DAGNodeContainer {
    const std::string       name    = "dagNodeContainer";
    }

    namespace RbObject {
    const std::string       name    = "object";
    }
    
    namespace RbFunction {
    const std::string       name    = "function";
    }

    namespace UserFunction {
    const std::string       name    = "userFunction";
    }

    namespace Argument {
    const std::string       name    = "argument";
    }

    namespace ArgumentRule {
    const std::string       name    = "argumentRule";
    }

    namespace RbMonitor {
    const std::string       name    = "monitor";
    }
    
    namespace RbMove {
    const std::string       name    = "move";
    }
    
    namespace RbMoveSchedule {
    const std::string       name    = "moveSchedule";
    }
    
    namespace MoveScale {
    const std::string       name    = "moveScale";
    }
    
    namespace MoveSlidingWindow {
    const std::string       name    = "moveSlidingWindow";
    }
    
    namespace RbMcmc {
    const std::string       name    = "mcmc";
    }
    
    namespace RbString {
    const std::string       name    = "string";
    }
    
    namespace RbInt {
    const std::string       name    = "int";
    }
    
    namespace RbDouble {
    const std::string       name    = "double";
    }
    
    namespace RbBool {
    const std::string       name    = "bool";
    }
    
    namespace Topology {
        const std::string       name    = "topology";
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
    
    namespace StringVector {
    const std::string       name    = "stringVector";
    }

    namespace IntVector {
    const std::string       name    = "intVector";
    }

    namespace RbComplex {
    const std::string       name    = "complex";
    }

    namespace RbException {
    const std::string       name    = "exception";
    }

    namespace Primitive {
    const std::string       name    = "primitive";
    }

    namespace Move {
    const std::string       name    = "move";
    }

    namespace MoveSchedule {
    const std::string       name    = "moveSchedule";
    }

    namespace RandomNumberGenerator {
    const std::string       name    = "randomNumberGenerator";
    }

	

}

#endif
