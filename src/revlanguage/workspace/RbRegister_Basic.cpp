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


/* Taxon types (in folder "datatypes/evolution") */
#include "RlTaxon.h"


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

/* Tree priors (in folder "distributions/evolution/tree") */
#include "Dist_bdp.h"
#include "Dist_BirthDeathMultiRate.h"
#include "Dist_Coalescent.h"
#include "Dist_constPopMultispCoal.h"
#include "Dist_divDepYuleProcess.h"
#include "Dist_empiricalTree.h"
#include "Dist_uniformTimeTree.h"
#include "Dist_uniformTopology.h"

/* Distributions on simple variables (in folder "distributions/math") */
#include "Dist_bernoulli.h"
#include "Dist_beta.h"
#include "Dist_bimodalLnorm.h"
#include "Dist_bimodalNorm.h"
#include "Dist_binomial.h"
#include "Dist_categorical.h"
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


/* Basic functions (in folder "functions/basic"). */

/* These are core functions for the Rev environment, providing user help
   and other essential services. */

#include "Func_clear.h"
#include "Func_exists.h"
#include "Func_getOption.h"
#include "Func_getwd.h"
#include "Func_ifelse.h"
#include "Func_license.h"
#include "Func_ls.h"
#include "Func_modelVector.h"
#include "Func_printSeed.h"
#include "Func_quit.h"
#include "Func_range.h"
#include "Func_rep.h"
#include "Func_seed.h"
#include "Func_seq.h"
#include "Func_setOption.h"
#include "Func_setwd.h"
#include "Func_structure.h"
#include "Func_system.h"
#include "Func_type.h"
#include "Func_workspaceVector.h"


/* Functions related to evolution (in folder "functions/evolution") */
#include "Func_branchScoreDistance.h"
#include "Func_clade.h"
#include "Func_concatenate.h"
#include "Func_constructRootedTripletDistribution.h"
#include "Func_maximumTree.h"
#include "Func_mrcaIndex.h"
#include "Func_pomoStateConverter.h"
#include "Func_pomoRootFrequencies.h"
#include "Func_simTree.h"
#include "Func_symmetricDifference.h"
#include "Func_tmrca.h"
#include "Func_treeAssembly.h"
#include "Func_treeScale.h"


/* Internal functions (in folder ("functions/internal") */

/* These are functions that are typically not called explicitly but implicitly
   through parsing of a Rev statement. Examples include a statement like '1 + 2',
   which results in the builtin '_add' function being called.
 
   Exceptions include Func_range and Func_vector, which are both used for implicit
   and explicit calls. They are therefore considered basic functions instead of
   internal functions.
 
   All internal functions have function calls that start with an underscore character,
   and therefore their class names have two underscore characters. They are typically
   templated. */

#include "Func__add.h"
#include "Func__and.h"
#include "Func__div.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__mult.h"
#include "Func__mod.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__unot.h"
#include "Func__scalarVectorAdd.h"
#include "Func__scalarVectorDiv.h"
#include "Func__scalarVectorMult.h"
#include "Func__scalarVectorSub.h"
#include "Func__sub.h"
#include "Func__uminus.h"
#include "Func__vectorIndexOperator.h"
#include "Func__vectorScalarDiv.h"
#include "Func__vectorScalarSub.h"


/* Input/output functions (in folder "functions/io") */
#include "Func_ancestralStateTree.h"
#include "Func_annotateHPDAges.h"
#include "Func_consensusTree.h"
#include "Func_convertToPhylowood.h"
#include "Func_mapTree.h"
#include "Func_module.h"
#include "Func_readAtlas.h"
#include "Func_readCharacterDataDelimited.h"
#include "Func_readContinuousCharacterData.h"
#include "Func_readDiscreteCharacterData.h"
#include "Func_readDistanceMatrix.h"
#include "Func_readCharacterDataUniversal.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readBranchLengthTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_readAncestralStateTreeTrace.h"
#include "Func_readAncestralStateTrace.h"
#include "Func_source.h"
#include "Func_TaxonReader.h"
#include "Func_write.h"
#include "Func_writeFasta.h"
#include "Func_writeNexus.h"


/* Math functions (in folder "functions/math") */
#include "Func_abs.h"
#include "Func_ceil.h"
#include "Func_diagonalMatrix.h"
#include "Func_exp.h"
#include "Func_floor.h"
#include "Func_lnProbability.h"
#include "Func_hyperbolicTangent.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_max.h"
#include "Func_mean.h"
#include "Func_min.h"
#include "Func_normalize.h"
#include "Func_power.h"
#include "Func_powerVector.h"
#include "Func_probability.h"
#include "Func_round.h"
#include "Func_simplex.h"
#include "Func_simplexFromVector.h"
#include "Func_sum.h"
#include "Func_sumPositive.h"
#include "Func_standardDeviation.h"
#include "Func_sqrt.h"
#include "Func_trunc.h"
#include "Func_variance.h"


/* Statistics functions (in folder "functions/statistics") */
/* These are functions related to statistical distributions */
#include "Func_discretizeGamma.h"
#include "Func_discretizeDistribution.h"
#include "Func_discretizePositiveDistribution.h"
#include "Func_dppConcFromMean.h"
#include "Func_dppMeanFromConc.h"
#include "Func_fnNormalizedQuantile.h"
#include "Func_numUniqueInVector.h"
#include "Func_stirling.h"
#include "Func_varianceCovarianceMatrix.h"
#include "Func_decomposedVarianceCovarianceMatrix.h"


/** Initialize global workspace */
void RevLanguage::Workspace::initializeBasicGlobalWorkspace(void)
{
    
    try
    {
        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////
        
        /* Basic functions (in folder "functions/basic") */
        
        // regular functions
        addFunction( new Func_clear()                    );
        addFunction( new Func_exists()                   );
        addFunction( new Func_getwd()                    );
        addFunction( new Func_getOption()                );
        addFunction( new Func_ifelse<Real>()             );
        addFunction( new Func_ifelse<RealPos>()          );
        addFunction( new Func_license()                  );
        addFunction( new Func_ls()                       );
        addFunction( new Func_printSeed()                );
        addFunction( new Func_quit()                     );
        addFunction( new Func_quit()                     );
        addFunction( new Func_range()                    );
        addFunction( new Func_rep<Integer>()             );
        addFunction( new Func_rep<Real>()                );
        addFunction( new Func_rep<Natural>()             );
        addFunction( new Func_rep<RealPos>()             );
        addFunction( new Func_seed()                     );
        addFunction( new Func_seq<Integer>()             );
        addFunction( new Func_seq<Real>()                );
        addFunction( new Func_setOption()                );
        addFunction( new Func_setwd()                    );
        addFunction( new Func_structure()                );
        addFunction( new Func_structure()                );
        addFunction( new Func_system()                   );
        addFunction( new Func_type()                     );
        
        /* Evolution-related functions (in folder "functions/evolution") */
        addFunction( new Func_branchScoreDistance()      );
        addFunction( new Func_clade()                    );
        addFunction( new Func_concatenate()              );
        addFunction( new Func_concatenate()              );
        addFunction( new Func_constructRootedTripletDistribution()            );
        addFunction( new Func_maximumTree()              );
        addFunction( new Func_mrcaIndex()                );
        addFunction( new Func_pomoStateConverter() );
        addFunction( new Func_pomoRootFrequencies() );
        addFunction( new Func_simTree()                  );
        addFunction( new Func_symmetricDifference()      );
        addFunction( new Func_tmrca()                    );
        addFunction( new Func_treeAssembly()             );
        addFunction( new Func_treeScale()                );
		

        // type conversion
//        addFunction( "_Natural2Integer",            new Func__conversion<Natural, Integer>()        );
        addFunction( new Func__conversion<Natural, Real>()           );
        addFunction( new Func__conversion<Natural, RealPos>()        );
        addFunction( new Func__conversion<Integer, Real>()           );
//        addFunction( "_Probability2RealPos",        new Func__conversion<Probability, RealPos>()    );
//        addFunction( "_Probability2Real",           new Func__conversion<Probability, Real>()       );
//        addFunction( "_RealPos2Real",               new Func__conversion<RealPos, Real>()           );
        
        
        addFunction( new Func__conversion<ModelVector<Natural>, ModelVector<Integer> >()         );
        addFunction( new Func__conversion<ModelVector<Natural>, ModelVector<Real> >()            );
        addFunction( new Func__conversion<ModelVector<Natural>, ModelVector<RealPos> >()         );
        addFunction( new Func__conversion<ModelVector<Integer>, ModelVector<Real> >()            );
        addFunction( new Func__conversion<ModelVector<RealPos>, ModelVector<Real> >()            );
        addFunction( new Func__conversion<ModelVector<Probability>, ModelVector<RealPos> >()     );
        addFunction( new Func__conversion<ModelVector<Probability>, ModelVector<Real> >()        );
        addFunction( new Func__conversion<Simplex, ModelVector<Real> >()        );
        
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( new Func_ancestralStateTree() );
		addFunction( new Func_annotateHPDAges()    );
		addFunction( new Func_consensusTree() );
        addFunction( new Func_convertToPhylowood() );
        addFunction( new Func_mapTree()    );
        addFunction( new Func_module()                       );
        addFunction( new Func_write()                        );
        addFunction( new Func_readAncestralStateTreeTrace()  );		
		addFunction( new Func_readAncestralStateTrace()	    );
        addFunction( new Func_readAtlas()                    );
		addFunction( new Func_readBranchLengthTrees()        );
        addFunction( new Func_readContinuousCharacterData()  );
        addFunction( new Func_readDiscreteCharacterData()    );
		addFunction( new Func_readDistanceMatrix()    );
        addFunction( new Func_readCharacterDataUniversal()   );
        addFunction( new Func_TaxonReader()                  );
        addFunction( new Func_readTrace()                    );
        addFunction( new Func_readTrees()                    );
        addFunction( new Func_readTreeTrace()                );
		addFunction( new Func_readCharacterDataDelimited()   );
        addFunction( new Func_source()                       );
        addFunction( new Func_write()                        );
        addFunction( new Func_writeFasta()                   );
        addFunction( new Func_writeNexus()                   );

        ///////////////////////////////////////////////////////////////////////////
        /* Add distribution functions (using help classes in folder "functions") */
        ///////////////////////////////////////////////////////////////////////////
        
        // bernoulli distribution
        addFunction( new DistributionFunctionPdf<Natural>( new Dist_bernoulli() ) );
        addFunction( new DistributionFunctionRv<Natural>( new Dist_bernoulli() ) );
        
        // binomial distribution
        addFunction( new DistributionFunctionPdf<Natural>( new Dist_binomial() ) );
        addFunction( new DistributionFunctionRv<Natural>( new Dist_binomial() ) );

        // beta distribution
        addFunction( new DistributionFunctionPdf<Probability>( new Dist_beta() ) );
        //        addFunction("pbeta", new DistributionFunctionCdf( new Dist_beta() ) );
        //        addFunction("qbeta", new DistributionFunctionQuantile( new Dist_beta() ) );
        addFunction( new DistributionFunctionRv<Probability>( new Dist_beta() ) );
        
        // categorical distribution
        addFunction( new DistributionFunctionPdf<Natural>( new Dist_categorical() ) );
        addFunction( new DistributionFunctionRv<Natural>( new Dist_categorical() ) );
        
        
        // CPP normal distribution
        addFunction( new DistributionFunctionPdf<Real>( new Dist_cppNormal() ) );
        
        // dirichlet distribution
        addFunction( new DistributionFunctionPdf<Simplex>( new Dist_dirichlet() ) );
        addFunction( new DistributionFunctionRv<Simplex>( new Dist_dirichlet() ) );
        
        // exponential distribution
        addFunction( new DistributionFunctionPdf<RealPos>( new Dist_exponential() ) );
        addFunction( new DistributionFunctionCdf( new Dist_exponential() ) );
        addFunction( new DistributionFunctionQuantilePositiveContinuous( new Dist_exponential() ) );
        addFunction( new DistributionFunctionRv<RealPos>( new Dist_exponential() ) );
        
        // gamma distribution
        addFunction( new DistributionFunctionPdf<RealPos>( new Dist_gamma() ) );
        addFunction( new DistributionFunctionCdf( new Dist_gamma() ) );
        addFunction( new DistributionFunctionQuantilePositiveContinuous( new Dist_gamma() ) );
        addFunction( new DistributionFunctionRv<RealPos>( new Dist_gamma() ) );
        
        // lognormal distribution
        addFunction( new DistributionFunctionPdf<RealPos>( new Dist_lnorm() ) );
        addFunction( new DistributionFunctionCdf( new Dist_lnorm() ) );
        addFunction( new DistributionFunctionQuantilePositiveContinuous( new Dist_lnorm() ) );
        addFunction( new DistributionFunctionRv<RealPos>( new Dist_lnorm() ) );
        addFunction( new DistributionFunctionPdf<Real>( new Dist_lnormOffset() ) );
        addFunction( new DistributionFunctionCdf( new Dist_lnormOffset() ) );
        addFunction( new DistributionFunctionQuantileContinuous( new Dist_lnormOffset() ) );
        addFunction( new DistributionFunctionRv<Real>( new Dist_lnormOffset() ) );
        
        // normal distribution
        addFunction( new DistributionFunctionPdf<Real>( new Dist_norm() ) );
        addFunction( new DistributionFunctionCdf( new Dist_norm() ) );
        addFunction( new DistributionFunctionQuantileContinuous( new Dist_norm() ) );
        addFunction( new DistributionFunctionRv<Real>( new Dist_norm() ) );
        
        //
        addFunction( new DistributionFunctionPdf<Natural>( new Dist_poisson() ) );
        addFunction( new DistributionFunctionRv<Natural>( new Dist_poisson() ) );
        
        // uniform distribution
        addFunction( new DistributionFunctionPdf<Real>( new Dist_unif() ) );
        addFunction( new DistributionFunctionCdf( new Dist_unif() ) );
        addFunction( new DistributionFunctionQuantileContinuous( new Dist_unif() ) );
        addFunction( new DistributionFunctionRv<Real>( new Dist_unif() ) );
        addFunction( new DistributionFunctionPdf<RealPos>( new Dist_unifPositive() ) );
        addFunction( new DistributionFunctionCdf( new Dist_unifPositive() ) );
        addFunction( new DistributionFunctionQuantilePositiveContinuous( new Dist_unifPositive() ) );
        addFunction( new DistributionFunctionRv<RealPos>( new Dist_unifPositive() ) );
        
        
        addFunction( new DistributionFunctionRv< AbstractHomologousDiscreteCharacterData >( new Dist_phyloCTMC() ) );
        

    }
    catch(RbException& rbException)
    {
        
        RBOUT("Caught an exception while initializing the workspace\n");
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


