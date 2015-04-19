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
#include "RevAbstractType.h"
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
#include "RlDiscreteCharacterData.h"

/* Tree types (in folder "datatypes/evolution/trees") */
#include "RlClade.h"
#include "RlRootedTripletDistribution.h"


/* Taxon types (in folder "datatypes/evolution") */
#include "RlTaxon.h"

/* Inference types (in folder "analysis") */
#include "RlBurninEstimationConvergenceAssessment.h"
#include "RlModel.h"
#include "RlPathSampler.h"
#include "RlPowerPosteriorAnalysis.h"
#include "RlSteppingStoneSampler.h"
#include "RlAncestralStateTrace.h"

/// Stopping Rules ///
#include "RlMaxIterationStoppingRule.h"
#include "RlMaxTimeStoppingRule.h"
#include "RlMinEssStoppingRule.h"
#include "RlGelmanRubinStoppingRule.h"
#include "RlGewekeStoppingRule.h"
#include "RlStationarityStoppingRule.h"

/// Monitors ///

/* Monitor types (in folder "monitors) */
#include "RlMonitor.h"
#include "Mntr_AncestralState.h"
#include "Mntr_JointConditionalAncestralState.h"
#include "Mntr_File.h"
#include "Mntr_ExtendedNewickFile.h"
#include "Mntr_Model.h"
#include "Mntr_PosteriorPredictive.h"
#include "Mntr_Screen.h"
#include "Mntr_CharacterHistoryNewickFile.h"
#include "Mntr_CharacterHistoryNhxFile.h"

/// Moves ///

/* Move types (in folder "datatypes/inference/moves") (grouped by parameter type) */
#include "RlMove.h"

/* Moves on real values */
#include "Move_SliceSampling.h"
#include "Move_Scale.h"
#include "Move_Slide.h"

/* Compound Moves on Real Values */
#include "Move_ScalerUpDown.h"
#include "Move_SliderUpDown.h"

/* Moves on integer values */
#include "Move_RandomGeometricWalk.h"
#include "Move_RandomIntegerWalk.h"

/* Moves on simplices */
#include "Move_Simplex.h"
#include "Move_SimplexSingleElementScale.h"

/* Moves on real valued vectors */
#include "Move_SingleElementScale.h"
#include "Move_VectorSingleElementScale.h"
#include "Move_VectorSingleElementSlide.h"
#include "Move_VectorScale.h"

/* Moves on real valued matrices */
#include "Move_MatrixSingleElementSlide.h"
#include "Move_ConjugateInverseWishartBrownian.h"


///* Moves on covariance matrices */
#include "Move_MatrixRealSymmetricSlide.h"


/* Moves on mixtures (in folder "datatypes/inference/moves/mixture") */
#include "Move_DPPScaleCatValsMove.h"
#include "Move_DPPAllocateAuxGibbsMove.h"
#include "Move_DPPGibbsConcentration.h"
#include "Move_DPPScaleCatAllocateAux.h"
#include "Move_MixtureAllocation.h"
#include "Move_ReversibleJumpSwitchMove.h"

/* Moves on character histories/data augmentation */
#include "Move_NodeCharacterHistoryRejectionSample.h"
#include "Move_PathCharacterHistoryRejectionSample.h"
#include "Move_CharacterHistory.h"


/* Moves on continuous phyloprocesses (Brownian, multivariate Brownian, etc) */

/* Tree proposals (in folder "datatypes/inference/moves/tree") */
#include "Move_EmpiricalTree.h"
#include "Move_FNPR.h"
#include "Move_GibbsPruneAndRegraft.h"
#include "Move_NarrowExchange.h"
#include "Move_NNIClock.h"
#include "Move_NNINonclock.h"
#include "Move_NodeTimeSlideUniform.h"
#include "Move_NodeTimeSlideBeta.h"
#include "Move_OriginTimeSlide.h"
#include "Move_RateAgeBetaShift.h"
#include "Move_RootTimeSlide.h"
#include "Move_SubtreeScale.h"
#include "Move_SPRNonclock.h"
#include "Move_TreeScale.h"
#include "Move_WeightedNodeTimeSlide.h"
#include "Move_FossilSafeSlide.h"
#include "Move_FossilSafeScale.h"

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


/* Rate matrix functions (in folder "functions/evolution/ratematrix") */
#include "Func_blosum62.h"
#include "Func_chromosomes.h"
#include "Func_cpRev.h"
#include "Func_dayhoff.h"
#include "Func_DECRateMatrix.h"
#include "Func_epoch.h"
#include "Func_f81.h"
#include "Func_FreeBinary.h"
#include "Func_FreeK.h"
#include "Func_gtr.h"
#include "Func_hky.h"
#include "Func_jc.h"
#include "Func_jones.h"
#include "Func_k80.h"
#include "Func_mtRev.h"
#include "Func_mtMam.h"
#include "Func_pomo.h"
#include "Func_rtRev.h"
#include "Func_vt.h"
#include "Func_t92.h"
#include "Func_wag.h"


/* Rate map functions (in folder "functions/evolution/ratemap") */
#include "Func_biogeo_de.h"
#include "Func_biogeo_grm.h"


/* Cladogeneic state prob function */
#include "Func_cladoProbs.h"
#include "Func_DECRates.h"
#include "Func_DECRoot.h"

/* Inference functions (in folder "functions/inference") */
#include "Func_Mcmc.h"
#include "Func_Mcmcmc.h"


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
        addDistribution( "dnPhyloWhiteNoise",                new Dist_PhyloWhiteNoise() );
        
        /* trait evolution (in folder "distributions/evolution/branchrate") */

        // brownian motion
        addDistribution( "dnPhyloBrownian",                  new Dist_PhyloBrownian() );
        addDistribution( "dnPhyloBrownianREML",              new Dist_PhyloBrownianREML<TimeTree>() );
        addDistribution( "dnPhyloBrownianMVN",               new Dist_PhyloBrownianMVN<TimeTree>() );
        addDistribution( "dnPhyloOUP",                       new Dist_PhyloOrnsteinUhlenbeck() );
        addDistribution( "dnPhyloOrnsteinUhlenbeck",         new Dist_PhyloOrnsteinUhlenbeck() );
        
        // multivariate brownian motion
        addDistribution( "dnPhyloBrownianMultiVariate",      new Dist_PhyloMvtBrownian() );
  
        /* Character state evolution processes (in folder "distributions/evolution/character") */
        
        // simple phylogenetic CTMC on fixed number of discrete states
        addDistribution( "dnPhyloCTMC",                 new Dist_phyloCTMC<TimeTree>() );
        addDistribution( "dnPhyloCTMC",                 new Dist_phyloCTMC<BranchLengthTree>() );
        addDistribution( "dnPhyloDACTMC",               new Dist_phyloDACTMC<TimeTree>() );
        addDistribution( "dnPhyloDACTMC",               new Dist_phyloDACTMC<BranchLengthTree>() );
//        addDistribution( "dnPhyloCTMCEpoch",            new Dist_phyloCTMCEpoch() );
        addDistribution( "dnPhyloCTMCClado",            new Dist_phyloCTMCClado<TimeTree>() );
        addDistribution( "dnPhyloCTMCClado",            new Dist_phyloCTMCClado<BranchLengthTree>() );
        
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
//        addDistribution( "dnBDP",                       new Dist_bdp() );
        AddDistribution<TimeTree>("BDP", new Dist_bdp());
        addDistribution( "dnBDPConst",                  new Dist_bdp() );
        addDistribution( "dnBirthDeath",                new Dist_bdp() );
        addDistribution( "dnBirthDeathConstant",        new Dist_bdp() );
        
        addDistribution( "dnBirthDeathMulti",           new Dist_BirthDeathMultiRate() );
        
        // constant rate birth-death process with serially sampled tips
        addDistribution( "dnBDPSerial",                 new Dist_serialBDP() );
        addDistribution( "dnBDPConstSerial",            new Dist_serialBDP() );
        addDistribution( "dnBirthDeathConstantSerial",  new Dist_serialBDP() );

        // piecewise constant rate birth-death process with serially sampled tips
        addDistribution( "dnBDPSkySerial",              new Dist_skySerialBDP() );
        addDistribution( "dnBirthDeathSkySerial",       new Dist_skySerialBDP() );

        // diversity-dependent pure-birth process
        addDistribution( "dnYuleDivDep",                new Dist_divDepYuleProcess() );
        addDistribution( "dnYuleDiversityDependent",    new Dist_divDepYuleProcess() );
        
        // coalescent (constant population sizes)
        addDistribution( "dnCoalescent",                new Dist_Coalescent() );

        // multispecies coalescent (per branch constant population sizes)
        addDistribution( "dnCoalMultiSpeciesConst",     new Dist_constPopMultispCoal() );
        addDistribution( "dnCoalMSConst",               new Dist_constPopMultispCoal() );

        // uniform time tree distribution
        addDistribution( "dnUniformTimeTree",           new Dist_uniformTimeTree() );
        
        // uniform topology distribution
        addDistribution( "dnUniformTopology",           new Dist_uniformTopology() );
        
		// empirical tree distributions
		addDistribution( "dnEmpiricalTree",             new Dist_empiricalTree<BranchLengthTree>() );
        addDistribution( "dnEmpiricalTree",             new Dist_empiricalTree<TimeTree>() );
		
		
        
        /* Statistical distributions on simple variables (in folder "distributions/math") */
        
        // bernoulli distribution
        addDistribution( "dnBernoulli",     new Dist_bernoulli() );

        // binomial distribution
        addDistribution( "dnBinomial",     new Dist_binomial() );
        
        // beta distribution
        addDistribution( "dnBeta",          new Dist_beta() );
        
        // bimodal normal distribution
        addDistribution( "dnBimodalNormal",     new Dist_bimodalNorm() );
        
        // bimodal lognormal distribution
        addDistribution( "dnBimodalLognormal",  new Dist_bimodalLnorm() );
        
        // categorical distribution
        addDistribution( "dnCat",           new Dist_categorical() );
        addDistribution( "dnCategorical",   new Dist_categorical() );

        // compound Poisson w/ normal kernel
        addDistribution( "dnCppNormal",     new Dist_cppNormal() );
        
        // dirichlet distribution
        addDistribution( "dnDirichlet",     new Dist_dirichlet() );
		
        // gamma distribution
        addDistribution( "dnGamma",         new Dist_gamma() );
        
        // geometric distribution
        addDistribution( "dnGeom",          new Dist_geom() );
        
        // poisson distribution
        addDistribution( "dnPoisson",       new Dist_poisson() );
        
        // exponential distribution
        addDistribution( "dnExp",           new Dist_exponential() );
        addDistribution( "dnExp",           new Dist_exponentialOffset() );
        addDistribution( "dnExp",           new Dist_exponentialOffsetPositive() );
        addDistribution( "dnExponential",   new Dist_exponential() );
        addDistribution( "dnExponential",   new Dist_exponentialOffset() );
        addDistribution( "dnExponential",   new Dist_exponentialOffsetPositive() );
        
        // lognormal distribution
        addDistribution( "dnLnorm",         new Dist_lnorm() );
        addDistribution( "dnLnorm",         new Dist_lnormOffset() );
        addDistribution( "dnLnorm",         new Dist_lnormOffsetPositive() );
        addDistribution( "dnLognormal",     new Dist_lnorm() );
        addDistribution( "dnLognormal",     new Dist_lnormOffset() );
        addDistribution( "dnLognormal",     new Dist_lnormOffsetPositive() );
        
        // multinomial distribution
        addDistribution( "dnMultinomial",     new Dist_multinomial() );
        
        // multivariate normal distribution
        addDistribution("dnMultivariateNormal", new Dist_multivariateNorm());
        addDistribution("dnMVNormal",           new Dist_multivariateNorm());
        
        // normal distribution
        AddContinuousDistribution<Real>("Normal", new Dist_norm());
//        addDistribution( "dnNorm",          new Dist_norm() );
//        addDistribution( "dnNormal",        new Dist_norm() );
        
        // LogUniform distribution   
        addDistribution( "dnLogUniform",    new Dist_logUniform() );
        
        // Uniform distribution with normal distributed bounds
        addDistribution( "dnSoftBoundUniformNormal",    new Dist_SoftBoundUniformNormal() );
        
        // uniform distribution
        addDistribution( "dnUnif",          new Dist_unif() );
        addDistribution( "dnUnif",          new Dist_unifPositive() );
        addDistribution( "dnUnif",          new Dist_unifProbability() );
        addDistribution( "dnUniform",       new Dist_unif() );
        addDistribution( "dnUniform",       new Dist_unifPositive() );
        addDistribution( "dnUniform",       new Dist_unifProbability() );
        
        // Wishart distribution
        addDistribution( "dnWishart",       new Dist_wishart() );
        
        // inverse Wishart distribution
        addDistribution( "dnInvWishart",       new Dist_inverseWishart() );
        addDistribution( "dnInverseWishart",   new Dist_inverseWishart() );

        // and the so-called "decomposed" Inverse Wishart
        addDistribution( "dnDecomposedInvWishart",       new Dist_decomposedInverseWishart() );
        
        /* Mixture distributions (in folder "distributions/mixture") */
        
        // dirichlet process prior distribution
        addDistribution( "dnDPP",           new Dist_dpp<Real>() );
		addDistribution( "dnDPP",           new Dist_dpp<RealPos>() );
		addDistribution( "dnDPP",           new Dist_dpp<Natural>() );
		addDistribution( "dnDPP",           new Dist_dpp<Integer>() );
		addDistribution( "dnDPP",           new Dist_dpp<Probability>() );
        addDistribution( "dnDPP",           new Dist_dpp<Simplex>() );

        // mixture distribution
        addDistribution( "dnMixture",       new Dist_mixture<Real>() );
		addDistribution( "dnMixture",       new Dist_mixture<RealPos>() );
		addDistribution( "dnMixture",       new Dist_mixture<Natural>() );
		addDistribution( "dnMixture",       new Dist_mixture<Integer>() );
		addDistribution( "dnMixture",       new Dist_mixture<Probability>() );
        addDistribution( "dnMixture",       new Dist_mixture<RateGenerator>() );
        
        // Ornstein-Uhlenbeck process
        addDistribution( "dnOrnsteinUhlenbeck", new OrnsteinUhlenbeckProcess() );
        addDistribution( "dnOU",                new OrnsteinUhlenbeckProcess() );
        
        // mixture distribution
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Real>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<RealPos>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Natural>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Integer>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Probability>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Simplex>() );
        //addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<RateGenerator>() );
        // aliases
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Real>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<RealPos>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Natural>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Integer>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Probability>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Simplex>() );
        //addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<RateGenerator>() );
        

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;

    }
    catch(RbException& rbException) {
        
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


