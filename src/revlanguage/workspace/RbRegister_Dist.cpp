/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * Instructions
 *
 * This is the central registry of Rev objects. It is a large file and needs
 * to be properly organized to facilitate maintenance. Follow these simple
 * guidelines to ensure that your additions follow the existing structure.
 * 
 * 1. All headers are added in groups corresponding to directories in the
 *    revlanguage code base.
 * 2. All objects (types, distributions, and functions) are registered in
 *    groups corresponding to directories in the revlanguage code base.
 * 3. All entries in each group are listed in alphabetical order.
 *
 * Some explanation of the directory structure is provided in the comments
 * in this file. Consult these comments if you are uncertain about where
 * to add your objects in the code.
 */

#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

/* Files including helper classes */
#include "AddContinuousDistribution.h"
#include "AddDistribution.h"
#include "AddWorkspaceVectorType.h"
#include "AddVectorizedWorkspaceType.h"
#include "RbException.h"
#include "RlUserInterface.h"
#include "Workspace.h"

/// Miscellaneous types ///

/* Base types (in folder "datatypes") */
#include "RevObject.h"

/* Primitive types (in folder "datatypes/basic") */
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

/* Container types (in folder "datatypes/container") */
#include "ModelVector.h"
#include "WorkspaceVector.h"

/* Evolution types (in folder "datatypes/evolution") */

/* Character state types (in folder "datatypes/evolution/character") */
#include "RlAminoAcidState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlStandardState.h"

/* Character data types (in folder "datatypes/evolution/datamatrix") */
#include "RlAbstractCharacterData.h"
#include "RlHomologousDiscreteCharacterData.h"

/* Tree types (in folder "datatypes/evolution/trees") */
#include "RlClade.h"
#include "RlRootedTripletDistribution.h"



/// Types ///

/* These types are needed as template types for the moves */
#include "RlBranchLengthTree.h"
#include "RlRateGenerator.h"




/* Math types (in folder "datatypes/math") */
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "RlRateMap.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"

/// Distributions ///

/* Distribution types (in folder "distributions") */

/* Character evolution models (in folder "distributions/evolution/character") */
#include "Dist_phyloCTMC.h"
#include "Dist_phyloDACTMC.h"
#include "Dist_phyloCTMCClado.h"

/* Branch rate priors (in folder "distributions/evolution/tree") */

/* Trait evolution models (in folder "distributions/evolution/branchrates") */
#include "Dist_PhyloBrownian.h"
#include "Dist_PhyloBrownianMVN.h"
#include "Dist_PhyloBrownianREML.h"
#include "Dist_PhyloOrnsteinUhlenbeck.h"
#include "Dist_PhyloMvtBrownian.h"
#include "Dist_PhyloWhiteNoise.h"

/* Tree priors (in folder "distributions/phylogenetics/tree") */
#include "Dist_bdp.h"
#include "Dist_bdpTopology.h"
#include "Dist_BirthDeathMultiRate.h"
#include "Dist_Coalescent.h"
#include "Dist_CoalescentSkyline.h"
#include "Dist_constFBDP.h"
#include "Dist_constPopMultispCoal.h"
#include "Dist_divDepYuleProcess.h"
#include "Dist_empiricalTree.h"
#include "Dist_phyloDistanceGamma.h"
#include "Dist_uniformTimeTree.h"
#include "Dist_uniformTopology.h"

/* Distributions on simple variables (in folder "distributions/math") */
#include "Dist_bernoulli.h"
#include "Dist_beta.h"
#include "Dist_bimodalLnorm.h"
#include "Dist_bimodalNorm.h"
#include "Dist_binomial.h"
#include "Dist_categorical.h"
#include "Dist_chisq.h"
#include "Dist_cppNormal.h"
#include "Dist_dirichlet.h"
#include "Dist_exponential.h"
#include "Dist_exponentialOffset.h"
#include "Dist_exponentialOffsetPositive.h"
#include "Dist_gamma.h"
#include "Dist_geom.h"
#include "Dist_poisson.h"
#include "Dist_lnorm.h"
#include "Dist_lnormOffset.h"
#include "Dist_lnormOffsetPositive.h"
#include "Dist_logUniform.h"
#include "Dist_multinomial.h"
#include "Dist_multivariateNorm.h"
#include "Dist_norm.h"
#include "Dist_softBoundUniformNormal.h"
#include "Dist_unif.h"
#include "Dist_unifPositive.h"
#include "Dist_unifProbability.h"
#include "Dist_wishart.h"
#include "Dist_inverseWishart.h"
#include "Dist_decomposedInverseWishart.h"
#include "Process_OrnsteinUhlenbeck.h"

/* Mixture distributions (in folder "distributions/mixture") */
#include "Dist_dpp.h"
#include "Dist_mixture.h"
#include "Dist_reversibleJumpMixtureConstant.h"

/// Functions ///

/* Helper functions for creating functions (in folder "functions") */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantileContinuous.h"
#include "DistributionFunctionQuantilePositiveContinuous.h"
#include "DistributionFunctionRv.h"


/* Argument rules (in folder "functions/argumentrules") */
#include "ArgumentRule.h"



/** Initialize global workspace */
void RevLanguage::Workspace::initializeDistGlobalWorkspace(void)
{
    
    try
    {
        ///////////////////////////////////////////////////
        /* Add distributions (in folder "distributions") */
        ///////////////////////////////////////////////////
        
        /* Evolutionary processes (in folder "distributions/evolution") */

        /* Branch rate processes (in folder "distributions/evolution/branchrate") */
        
        // white noise process
        addDistribution(  new Dist_PhyloWhiteNoise() );
        
        /* trait evolution (in folder "distributions/evolution/branchrate") */

        // brownian motion
        addDistribution( new Dist_PhyloBrownian() );
        addDistribution( new Dist_PhyloBrownianREML() );
        addDistribution( new Dist_PhyloBrownianMVN() );
        addDistribution( new Dist_PhyloOrnsteinUhlenbeck() );
        
        // multivariate brownian motion
        addDistribution( new Dist_PhyloMvtBrownian() );
  
        /* Character state evolution processes (in folder "distributions/evolution/character") */
        
        // simple phylogenetic CTMC on fixed number of discrete states
        addDistribution( new Dist_phyloCTMC() );
        addDistribution( new Dist_phyloDACTMC() );
        addDistribution( new Dist_phyloCTMCClado() );
        
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
//        addDistribution( "dnBDP",                       new Dist_bdp() );
        AddDistribution<TimeTree>( new Dist_bdp());
        addDistribution( new Dist_bdpTopology() );
		
        addDistribution( new Dist_BirthDeathMultiRate() );
        
        
        // constant rate birth-death process
        AddDistribution<TimeTree>( new Dist_constFBDP());
        
        // diversity-dependent pure-birth process
        addDistribution( new Dist_divDepYuleProcess() );
        
        // coalescent (constant population sizes)
        addDistribution( new Dist_Coalescent() );
        
        // coalescent (skyline population sizes)
        addDistribution( new Dist_CoalescentSkyline() );

        // multispecies coalescent (per branch constant population sizes)
        addDistribution( new Dist_constPopMultispCoal() );

        // uniform time tree distribution
        addDistribution( new Dist_uniformTimeTree() );
        
        // uniform topology distribution
        addDistribution( new Dist_uniformTopology() );
        
		// empirical tree distributions
		addDistribution( new Dist_empiricalTree() );
		
		// Distance Matrix Gamma distribution
		addDistribution( new Dist_phyloDistanceGamma() );

		
        /* Statistical distributions on simple variables (in folder "distributions/math") */
        
        // bernoulli distribution
        addDistribution( new Dist_bernoulli() );

        // binomial distribution
        addDistribution( new Dist_binomial() );
        
        // beta distribution
        addDistribution( new Dist_beta() );
        
        // bimodal normal distribution
        addDistribution( new Dist_bimodalNorm() );
        
        // bimodal lognormal distribution
        addDistribution( new Dist_bimodalLnorm() );
        
        // categorical distribution
        addDistribution( new Dist_categorical() );
        
        // chi-square distribution
        AddContinuousDistribution<RealPos>( new Dist_chisq() );
//        addDistribution( "dnChisq",         new Dist_chisq() );

        // compound Poisson w/ normal kernel
        addDistribution( new Dist_cppNormal() );
        
        // dirichlet distribution
        addDistribution( new Dist_dirichlet() );
		
        // gamma distribution
        addDistribution( new Dist_gamma() );
        
        // geometric distribution
        addDistribution( new Dist_geom() );
        
        // poisson distribution
        addDistribution( new Dist_poisson() );
        
        // exponential distribution
        addDistribution( new Dist_exponential() );
        addDistribution( new Dist_exponentialOffset() );
        addDistribution( new Dist_exponentialOffsetPositive() );
        
        // lognormal distribution
        addDistribution( new Dist_lnorm() );
        addDistribution( new Dist_lnormOffset() );
        addDistribution( new Dist_lnormOffsetPositive() );
        
        // multinomial distribution
        addDistribution( new Dist_multinomial() );
        
        // multivariate normal distribution
        addDistribution( new Dist_multivariateNorm());
        
        // normal distribution
        AddContinuousDistribution<Real>( new Dist_norm() );
//        addDistribution( "dnNorm",          new Dist_norm() );
//        addDistribution( "dnNormal",        new Dist_norm() );
        
        // LogUniform distribution   
        addDistribution( new Dist_logUniform() );
        
        // Uniform distribution with normal distributed bounds
        addDistribution( new Dist_SoftBoundUniformNormal() );
        
        // uniform distribution
        addDistribution( new Dist_unif() );
        addDistribution( new Dist_unifPositive() );
        addDistribution( new Dist_unifProbability() );
        
        // Wishart distribution
        addDistribution( new Dist_wishart() );
        
        // inverse Wishart distribution
        addDistribution( new Dist_inverseWishart() );

        // and the so-called "decomposed" Inverse Wishart
        addDistribution( new Dist_decomposedInverseWishart() );
        
        /* Mixture distributions (in folder "distributions/mixture") */
        
        // dirichlet process prior distribution
        addDistribution( new Dist_dpp<Real>() );
		addDistribution( new Dist_dpp<RealPos>() );
		addDistribution( new Dist_dpp<Natural>() );
		addDistribution( new Dist_dpp<Integer>() );
		addDistribution( new Dist_dpp<Probability>() );
        addDistribution( new Dist_dpp<Simplex>() );

        // mixture distribution
        addDistribution( new Dist_mixture<Real>() );
		addDistribution( new Dist_mixture<RealPos>() );
		addDistribution( new Dist_mixture<Natural>() );
		addDistribution( new Dist_mixture<Integer>() );
		addDistribution( new Dist_mixture<Probability>() );
        addDistribution( new Dist_mixture<RateGenerator>() );
        
        // Ornstein-Uhlenbeck process
        addDistribution( new OrnsteinUhlenbeckProcess() );
        
        // mixture distribution
        addDistribution( new Dist_reversibleJumpMixtureConstant<Real>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<RealPos>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<Natural>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<Integer>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<Probability>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<Simplex>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<ModelVector<Natural> >() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<TimeTree>() );
        addDistribution( new Dist_reversibleJumpMixtureConstant<BranchLengthTree>() );
        

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;

    }
    catch(RbException& rbException)
    {
        
        RBOUT("Caught an exception while initializing distributions in the workspace\n");
        std::ostringstream msg;
        rbException.print(msg);
        msg << std::endl;
        RBOUT(msg.str());
        
        RBOUT("Please report this bug to the RevBayes Development Core Team");
        
        RBOUT("Press any character to exit the program.");
        getchar();
        exit(1);
    }
    
}


