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
#include "Dist_phyloCTMCEpoch.h"
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
#include "Dist_serialBDP.h"
#include "Dist_skySerialBDP.h"
#include "Dist_skyFossilBDP.h"
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
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "exists",                   new Func_exists()                   );
        addFunction( "getwd",                    new Func_getwd()                    );
        addFunction( "getOption",                new Func_getOption()                );
        addFunction( "ifelse",                   new Func_ifelse<Real>()             );
        addFunction( "ifelse",                   new Func_ifelse<RealPos>()          );
        addFunction( "license",                  new Func_license()                  );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "printSeed",                new Func_printSeed()                );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "range",                    new Func_range()                    );
        addFunction( "rep",                      new Func_rep<Integer>()             );
        addFunction( "rep",                      new Func_rep<Real>()                );
        addFunction( "rep",                      new Func_rep<Natural>()             );
        addFunction( "rep",                      new Func_rep<RealPos>()             );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "seq",                      new Func_seq<Integer>()             );
        addFunction( "seq",                      new Func_seq<Real>()                );
        addFunction( "setOption",                new Func_setOption()                );
        addFunction( "setwd",                    new Func_setwd()                    );
        addFunction( "str",                      new Func_structure()                );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "system",                   new Func_system()                   );
        addFunction( "type",                     new Func_type()                     );
        
        /* Evolution-related functions (in folder "functions/evolution") */
        addFunction( "branchScoreDistance",         new Func_branchScoreDistance()      );
        addFunction( "clade",                       new Func_clade()                    );
        addFunction( "concatenate",                 new Func_concatenate()              );
        addFunction( "concat",                      new Func_concatenate()              );
        addFunction( "rootedTripletDist",           new Func_constructRootedTripletDistribution()            );
        addFunction( "maximumTree",                 new Func_maximumTree()              );
        addFunction( "mrcaIndex",                   new Func_mrcaIndex()                );
        addFunction( "pomoStateConvert",            new Func_pomoStateConverter() );
        addFunction( "pomoRF",                      new Func_pomoRootFrequencies() );
        addFunction( "simTree",                     new Func_simTree()                  );
        addFunction( "symDiff",                     new Func_symmetricDifference()      );
        addFunction( "tmrca",                       new Func_tmrca()                    );
        addFunction( "treeAssembly",                new Func_treeAssembly()             );
        addFunction( "treeScale",                   new Func_treeScale()                );

        // type conversion
//        addFunction( "_Natural2Integer",            new Func__conversion<Natural, Integer>()        );
        addFunction( "_Natural2Real",               new Func__conversion<Natural, Real>()           );
        addFunction( "_Natural2RealPos",            new Func__conversion<Natural, RealPos>()        );
        addFunction( "_Integer2Real",               new Func__conversion<Integer, Real>()           );
//        addFunction( "_Probability2RealPos",        new Func__conversion<Probability, RealPos>()    );
//        addFunction( "_Probability2Real",           new Func__conversion<Probability, Real>()       );
//        addFunction( "_RealPos2Real",               new Func__conversion<RealPos, Real>()           );
        
        
        addFunction( "_Natural[]2Integer[]",        new Func__conversion<ModelVector<Natural>, ModelVector<Integer> >()         );
        addFunction( "_Natural[]2Real[]",           new Func__conversion<ModelVector<Natural>, ModelVector<Real> >()            );
        addFunction( "_Natural[]2RealPos[]",        new Func__conversion<ModelVector<Natural>, ModelVector<RealPos> >()         );
        addFunction( "_Integer[]2Real[]",           new Func__conversion<ModelVector<Integer>, ModelVector<Real> >()            );
        addFunction( "_RealPos[]2Real[]",           new Func__conversion<ModelVector<RealPos>, ModelVector<Real> >()            );
        addFunction( "_Probability[]2RealPos[]",    new Func__conversion<ModelVector<Probability>, ModelVector<RealPos> >()     );
        addFunction( "_Probability[]2Real[]",       new Func__conversion<ModelVector<Probability>, ModelVector<Real> >()        );
        addFunction( "_Simplex2Real[]",             new Func__conversion<Simplex, ModelVector<Real> >()        );
        
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( "ancestralStateTree",          new Func_ancestralStateTree<BranchLengthTree>() );
		addFunction( "ancestralStateTree",          new Func_ancestralStateTree<TimeTree>() );
		addFunction( "annotateHPDAges",             new Func_annotateHPDAges<TimeTree>()    );
		addFunction( "consensusTree",				new Func_consensusTree<BranchLengthTree>() );
		addFunction( "consensusTree",               new Func_consensusTree<TimeTree>()      );
        addFunction( "convertToPhylowood",          new Func_convertToPhylowood<TimeTree>() );
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()    );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()            );
        addFunction( "module",                      new Func_module()                       );
        addFunction( "print",                       new Func_write()                        );
        addFunction( "readAncestralStateTreeTrace", new Func_readAncestralStateTreeTrace()  );		
		addFunction( "readAncestralStateTrace",     new Func_readAncestralStateTrace()	    );
        addFunction( "readAtlas",                   new Func_readAtlas()                    );
		addFunction( "readBranchLengthTrees",       new Func_readBranchLengthTrees()        );
        addFunction( "readContinuousCharacterData", new Func_readContinuousCharacterData()  );
        addFunction( "readDiscreteCharacterData",   new Func_readDiscreteCharacterData()    );
        addFunction( "readCharacterData",           new Func_readCharacterDataUniversal()   );
        addFunction( "readTaxonData",               new Func_TaxonReader()                  );
        addFunction( "readTrace",                   new Func_readTrace()                    );
        addFunction( "readTrees",                   new Func_readTrees()                    );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()                );
		addFunction( "readCharacterDataDelimited",  new Func_readCharacterDataDelimited()   );
        addFunction( "source",                      new Func_source()                       );
        addFunction( "write",                       new Func_write()                        );
        addFunction( "writeFasta",                  new Func_writeFasta()                   );
        addFunction( "writeNexus",                  new Func_writeNexus()                   );

        ///////////////////////////////////////////////////////////////////////////
        /* Add distribution functions (using help classes in folder "functions") */
        ///////////////////////////////////////////////////////////////////////////
        
        // bernoulli distribution
        addFunction("dbernoulli", new DistributionFunctionPdf<Natural>( new Dist_bernoulli() ) );
        addFunction("rbernoulli", new DistributionFunctionRv<Natural>( new Dist_bernoulli() ) );
        
        // binomial distribution
        addFunction("dbinomial", new DistributionFunctionPdf<Natural>( new Dist_binomial() ) );
        addFunction("rbinomial", new DistributionFunctionRv<Natural>( new Dist_binomial() ) );

        // beta distribution
        addFunction("dbeta", new DistributionFunctionPdf<Probability>( new Dist_beta() ) );
        //        addFunction("pbeta", new DistributionFunctionCdf( new Dist_beta() ) );
        //        addFunction("qbeta", new DistributionFunctionQuantile( new Dist_beta() ) );
        addFunction("rbeta", new DistributionFunctionRv<Probability>( new Dist_beta() ) );
        
        // categorical distribution
        addFunction("dcat", new DistributionFunctionPdf<Natural>( new Dist_categorical() ) );
        addFunction("rcat", new DistributionFunctionRv<Natural>( new Dist_categorical() ) );
        
        
        // CPP normal distribution
        addFunction("dCppNormal", new DistributionFunctionPdf<Real>( new Dist_cppNormal() ) );
        
        // dirichlet distribution
        addFunction("ddirichlet", new DistributionFunctionPdf<Simplex>( new Dist_dirichlet() ) );
        addFunction("rdirichlet", new DistributionFunctionRv<Simplex>( new Dist_dirichlet() ) );
        
        // exponential distribution
        addFunction("dexp", new DistributionFunctionPdf<RealPos>( new Dist_exponential() ) );
        addFunction("pexp", new DistributionFunctionCdf( new Dist_exponential() ) );
        addFunction("qexp", new DistributionFunctionQuantilePositiveContinuous( new Dist_exponential() ) );
        addFunction("rexp", new DistributionFunctionRv<RealPos>( new Dist_exponential() ) );
        
        // gamma distribution
        addFunction("dgamma", new DistributionFunctionPdf<RealPos>( new Dist_gamma() ) );
        addFunction("pgamma", new DistributionFunctionCdf( new Dist_gamma() ) );
        addFunction("qgamma", new DistributionFunctionQuantilePositiveContinuous( new Dist_gamma() ) );
        addFunction("rgamma", new DistributionFunctionRv<RealPos>( new Dist_gamma() ) );
        
        // lognormal distribution
        addFunction("dlnorm", new DistributionFunctionPdf<RealPos>( new Dist_lnorm() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_lnorm() ) );
        addFunction("qlnorm", new DistributionFunctionQuantilePositiveContinuous( new Dist_lnorm() ) );
        addFunction("rlnorm", new DistributionFunctionRv<RealPos>( new Dist_lnorm() ) );
        addFunction("dlnorm", new DistributionFunctionPdf<Real>( new Dist_lnormOffset() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_lnormOffset() ) );
        addFunction("qlnorm", new DistributionFunctionQuantileContinuous( new Dist_lnormOffset() ) );
        addFunction("rlnorm", new DistributionFunctionRv<Real>( new Dist_lnormOffset() ) );
        
        // normal distribution
        addFunction("dnorm", new DistributionFunctionPdf<Real>( new Dist_norm() ) );
        addFunction("pnorm", new DistributionFunctionCdf( new Dist_norm() ) );
        addFunction("qnorm", new DistributionFunctionQuantileContinuous( new Dist_norm() ) );
        addFunction("rnorm", new DistributionFunctionRv<Real>( new Dist_norm() ) );
        
        //
        addFunction("dpoisson", new DistributionFunctionPdf<Natural>( new Dist_poisson() ) );
        addFunction("rpoisson", new DistributionFunctionRv<Natural>( new Dist_poisson() ) );
        
        // uniform distribution
        addFunction("dunif", new DistributionFunctionPdf<Real>( new Dist_unif() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_unif() ) );
        addFunction("qunif", new DistributionFunctionQuantileContinuous( new Dist_unif() ) );
        addFunction("runif", new DistributionFunctionRv<Real>( new Dist_unif() ) );
        addFunction("dunif", new DistributionFunctionPdf<RealPos>( new Dist_unifPositive() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_unifPositive() ) );
        addFunction("qunif", new DistributionFunctionQuantilePositiveContinuous( new Dist_unifPositive() ) );
        addFunction("runif", new DistributionFunctionRv<RealPos>( new Dist_unifPositive() ) );
        
        
        addFunction("rPhyloCTMC", new DistributionFunctionRv< AbstractHomologousDiscreteCharacterData >( new Dist_phyloCTMC<TimeTree>() ) );
        addFunction("rPhyloCTMC", new DistributionFunctionRv< AbstractHomologousDiscreteCharacterData >( new Dist_phyloCTMC<BranchLengthTree>() ) );


    }
    catch(RbException& rbException) {
        
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


