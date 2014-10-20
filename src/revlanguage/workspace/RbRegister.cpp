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

/* Inference types (in folder "datatypes/inference") */
#include "RlBurninEstimationConvergenceAssessment.h"
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlParallelMcmcmc.h"
#include "RlPathSampler.h"
#include "RlPowerPosterior.h"
#include "RlSteppingStoneSampler.h"

/// Monitors ///

/* Monitor types (in folder "datatypes/inference/monitors) */
#include "RlMonitor.h"
#include "Mntr_File.h"
#include "Mntr_ExtendedNewickFile.h"
#include "Mntr_Model.h"
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

/* Moves on integer values */
#include "Move_RandomGeometricWalk.h"
#include "Move_RandomIntegerWalk.h"

/* Moves on simplices */
#include "Move_Simplex.h"
#include "Move_SimplexSingleElementScale.h"

/* Moves on real valued vectors */
#include "Move_RLCRateScale.h"
#include "Move_SingleElementScale.h"
#include "Move_SwitchRateJump.h"
#include "Move_VectorSingleElementScale.h"
#include "Move_VectorSingleElementSlide.h"
#include "Move_VectorScale.h"

/* Moves on real valued matrices */
#include "Move_MatrixSingleElementSlide.h"


///* Moves on covariance matrices */
#include "Move_RealSymmetricMatrixSimple.h"


/* Moves on mixtures (in folder "datatypes/inference/moves/mixture") */
#include "Move_DPPScaleCatValsMove.h"
#include "Move_DPPAllocateAuxGibbsMove.h"
#include "Move_DPPGibbsConcentration.h"
#include "Move_DPPScaleCatAllocateAux.h"
#include "Move_MixtureAllocation.h"

/* Moves on character histories/data augmentation */
#include "Move_NodeCharacterHistoryRejectionSample.h"
#include "Move_PathCharacterHistoryRejectionSample.h"


/* Moves on continuous phyloprocesses (Brownian, multivariate Brownian, etc) */
#include "Move_MultivariateRealNodeValTreeTranslation.h"
#include "Move_MultivariateRealNodeValTreeSliding.h"
#include "Move_ConjugateInverseWishartBrownian.h"
#include "Move_RealNodeValTreeSliding.h"
#include "Move_RealNodeValTreeTranslation.h"
#include "Move_ScaleSingleACLNRates.h"
#include "Move_ACLNMixingStep.h"

/* Tree proposals (in folder "datatypes/inference/moves/tree") */
#include "Move_FNPR.h"
#include "Move_NarrowExchange.h"
#include "Move_NNIClock.h"
#include "Move_NNINonclock.h"
#include "Move_NodeTimeSlideUniform.h"
#include "Move_OriginTimeSlide.h"
#include "Move_RateAgeBetaShift.h"
#include "Move_RootTimeSlide.h"
#include "Move_SubtreeScale.h"
#include "Move_SPRNonclock.h"
#include "Move_TreeScale.h"
#include "Move_WeightedNodeTimeSlide.h"

/* Math types (in folder "datatypes/math") */
#include "RealMatrix.h"
#include "RealSymmetricMatrix.h"
#include "RlRateMap.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"

/// Distributions ///

/* Distribution types (in folder "distributions") */

/* Character evolution models (in folder "distributions/evolution/character") */
#include "Dist_phyloCTMC.h"
#include "Dist_phyloDACTMC.h"

/* Branch rate priors (in folder "distributions/evolution/tree") */
#include "Dist_autocorrelatedLnBranchRates.h"
#include "Dist_branchRateJumpProcess.h"
#include "Dist_whiteNoise.h"

/* Trait evolution models (in folder "distributions/evolution/tree") */
#include "Dist_brownian.h"
#include "Dist_ornsteinUhlenbeck.h"
#include "Dist_mvtBrownian.h"

/* Tree priors (in folder "distributions/evolution/tree") */
#include "Dist_bdp.h"
#include "Dist_constPopMultispCoal.h"
#include "Dist_divDepYuleProcess.h"
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
#include "Dist_dirichlet.h"
#include "Dist_exponential.h"
#include "Dist_gamma.h"
#include "Dist_geom.h"
#include "Dist_poisson.h"
#include "Dist_lnorm.h"
#include "Dist_norm.h"
#include "Dist_offsetExponential.h"
#include "Dist_offsetLnorm.h"
#include "Dist_oneOverX.h"
#include "Dist_positiveUnif.h"
#include "Dist_unif.h"
#include "Dist_wishart.h"
#include "Dist_inverseWishart.h"

/* Mixture distributions (in folder "distributions/mixture") */
#include "Dist_dpp.h"
#include "Dist_mixture.h"

/// Functions ///

/* Helper functions for creating functions (in folder "functions") */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"


/* Argument rules (in folder "functions/argumentrules") */
#include "ArgumentRule.h"


/* Basic functions (in folder "functions/basic"). */

/* These are core functions for the Rev environment, providing user help
   and other essential services. */

#include "Func_citation.h"
#include "Func_clear.h"
#include "Func_contributors.h"
#include "Func_exists.h"
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
#include "Func_setwd.h"
#include "Func_structure.h"
#include "Func_system.h"
#include "Func_type.h"
#include "Func_workspaceVector.h"


/* Functions related to evolution (in folder "functions/evolution") */
#include "Func_averageRateOnBranch.h"
#include "Func_clade.h"
#include "Func_constructRootedTripletDistribution.h"
#include "Func_expBranchTree.h"
#include "Func_tanhBranchTree.h"
#include "Func_t92GCBranchTree.h"
#include "Func_maximumTree.h"
#include "Func_mrcaIndex.h"
#include "Func_phyloRateMultiplier.h"
#include "Func_pomoStateConverter.h"
#include "Func_pomoRootFrequencies.h"
#include "Func_symmetricDifference.h"
#include "Func_tmrca.h"
#include "Func_treeHeight.h"
#include "Func_treeAssembly.h"


/* Rate matrix functions (in folder "functions/evolution/ratematrix") */
#include "Func_blosum62.h"
#include "Func_cpRev.h"
#include "Func_dayhoff.h"
#include "Func_f81.h"
#include "Func_gtr.h"
#include "Func_hky.h"
#include "Func_t92.h"
#include "Func_jc.h"
#include "Func_jones.h"
#include "Func_mtRev.h"
#include "Func_mtMam.h"
#include "Func_pomo.h"
#include "Func_rtRev.h"
#include "Func_vt.h"
#include "Func_wag.h"

/* Rate map functions (in folder "functions/evolution/ratemap") */
#include "Func_biogeo_de.h"
#include "Func_biogeo_grm.h"


/* Inference functions (in folder "functions/inference") */

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
#include "Func__rladd.h"
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
#include "Func_mapTree.h"
#include "Func_readAtlas.h"
#include "Func_readCharacterData.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_source.h"
#include "Func_TaxonReader.h"
#include "Func_write.h"
#include "Func_writeFasta.h"
#include "Func_writeNexus.h"


/* Math functions (in folder "functions/math") */
#include "Func_abs.h"
#include "Func_ceil.h"
#include "Func_exp.h"
#include "Func_floor.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_mean.h"
#include "Func_min.h"
#include "Func_normalize.h"
#include "Func_power.h"
#include "Func_powermix.h"
#include "Func_powerVector.h"
#include "Func_round.h"
#include "Func_simplex.h"
#include "Func_sum.h"
#include "Func_sqrt.h"
#include "Func_trunc.h"


/* Statistics functions (in folder "functions/statistics") */
/* These are functions related to statistical distributions */
#include "Func_discretizeGamma.h"
#include "Func_dppConcFromMean.h"
#include "Func_dppMeanFromConc.h"
#include "Func_fnNormalizedQuantile.h"
#include "Func_numUniqueInVector.h"



/** Initialize global workspace */
void RevLanguage::Workspace::initializeGlobalWorkspace(void)
{
    
    try
    {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */
        
        /* Add base types (in folder "datatypes") */
        addType( new RevAbstractType( RevObject::getClassTypeSpec(), new Integer( 0 ) ) );

        /* Add primitive types (in folder "datatypes/basic") (alphabetic order) */
        AddWorkspaceVectorType<Integer,5>::addTypeToWorkspace( *this, new Integer() );
        AddWorkspaceVectorType<Natural,5>::addTypeToWorkspace( *this, new Natural() );
        AddWorkspaceVectorType<Probability,5>::addTypeToWorkspace( *this, new Probability() );
        AddWorkspaceVectorType<Real,5>::addTypeToWorkspace( *this, new Real() );
        AddWorkspaceVectorType<RealPos,5>::addTypeToWorkspace( *this, new RealPos() );
        AddWorkspaceVectorType<RlBoolean,5>::addTypeToWorkspace( *this, new RlBoolean() );
        AddWorkspaceVectorType<RlString,5>::addTypeToWorkspace( *this, new RlString() );
        AddWorkspaceVectorType<Simplex,5>::addTypeToWorkspace( *this, new Simplex() );
        
        AddWorkspaceVectorType<RateMatrix,5>::addTypeToWorkspace( *this, new RateMatrix() );
        AddWorkspaceVectorType<AbstractDiscreteCharacterData,5>::addTypeToWorkspace( *this, new AbstractDiscreteCharacterData() );
        
        AddWorkspaceVectorType<TimeTree,3>::addTypeToWorkspace( *this, new TimeTree() );
        
//        AddVectorizedWorkspaceType<Monitor,3>::addTypeToWorkspace( *this, new Monitor() );
        addFunction("v", new Func_workspaceVector<Monitor>() );
        
        //        AddVectorizedWorkspaceType<Move,3>::addTypeToWorkspace( *this, new Move() );
        addFunction("v", new Func_workspaceVector<Move>() );
        
        /* Add evolution types (in folder "datatypes/evolution") (alphabetic order) */
        
        /* Add character types (in folder "datatypes/evolution/character") (alphabetic order) */
        
        /* Add data matrix types (in folder "datatypes/evolution/datamatrix") (alphabetic order) */

        /* Add tree types (in folder "datatypes/evolution/trees") (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );
       // addTypeWithConstructor( "rootedTripletDist", new RootedTripletDistribution() );

        
        /* Add Taxon (in folder "datatypes/evolution/") (alphabetic order) */
        addTypeWithConstructor( "taxon",            new Taxon() );
        
        /* Add math types (in folder "datatypes/math") */
        addType( new RateMap()              );
        addType( new RealMatrix()           );



        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "beca",                   new BurninEstimationConvergenceAssessment()   );
        addTypeWithConstructor( "mcmc",                   new Mcmc()                                    );
        addTypeWithConstructor( "model",                  new Model()                                   );
        addTypeWithConstructor( "pmcmcmc",                new ParallelMcmcmc()                          );
        addTypeWithConstructor( "pathSampler",            new PathSampler()                             );
        addTypeWithConstructor( "powerPosterior",         new PowerPosterior()                          );
        addTypeWithConstructor( "steppingStoneSampler",   new SteppingStoneSampler()                    );


        ////////////////////////////////////////////////////////////////////////////////
        /* Add monitors (in folder "datatypes/inference/monitors") (alphabetic order) */
        ////////////////////////////////////////////////////////////////////////////////

        addTypeWithConstructor("mnExtNewick",           new Mntr_ExtendedNewickFile());
        addTypeWithConstructor("mnFile",                new Mntr_File());
        addTypeWithConstructor("mnModel",               new Mntr_Model());
        addTypeWithConstructor("mnScreen",              new Mntr_Screen());
        addTypeWithConstructor("mnCharHistoryNewick",   new Mntr_CharacterHistoryNewickFile());
        addTypeWithConstructor("mnCharHistoryNhx",      new Mntr_CharacterHistoryNhxFile());

        
        ///////////////////////////////////////////////////////////////////////////////////
        /* Add moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
        ///////////////////////////////////////////////////////////////////////////////////
        
        /* Regular moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
        
        /* Moves on real values */
        addTypeWithConstructor("mvScale",               new Move_Scale() );
        addTypeWithConstructor("mvSlide",               new Move_Slide() );
        addTypeWithConstructor("mvSlice",               new Move_SliceSampling() );
		
		// compound moves on real values
        addTypeWithConstructor("mvScalerUpDown",        new Move_ScalerUpDown() );
                
        /* Moves on integer values */
        addTypeWithConstructor("mvRandomIntegerWalk",   new Move_RandomIntegerWalk() );
        addTypeWithConstructor("mvRandomGeometricWalk", new Move_RandomGeometricWalk() );


        /* Moves on simplices */
        addTypeWithConstructor("mvSimplex",             new Move_Simplex() );
        addTypeWithConstructor("mvSimplexElementScale", new Move_SimplexSingleElementScale() );        

        /* Moves on vectors of real values */
        addTypeWithConstructor("mvRlcRateScale",                new Move_RLCRateScale() );
        addTypeWithConstructor("mvSingleElementScale",          new Move_SingleElementScale() );
        addTypeWithConstructor("mvSwitchRateJump",              new Move_SwitchRateJump() );
        addTypeWithConstructor("mvVectorScale",                 new Move_VectorScale() );
        addTypeWithConstructor("mvVectorSingleElementScale",    new Move_VectorSingleElementScale() );
        addTypeWithConstructor("mvVectorSingleElementSliding",  new Move_VectorSingleElementSlide() );
        
        /* Moves on matrices of real values */
        addTypeWithConstructor("mvMatrixSingleElementSliding",  new Move_MatrixSingleElementSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor("mvSymmetricMatrixSimple",       new Move_RealSymmetricMatrixSimple() );
        addTypeWithConstructor("mvCovarianceMatrixSimple",       new Move_RealSymmetricMatrixSimple() );

        /* Moves on mixtures (in folder "datatypes/inference/moves/mixture") */
        addTypeWithConstructor("mvDPPScaleCatVals",                new Move_DPPScaleCatValsMove() );
        addTypeWithConstructor("mvDPPScaleCatAllocateAux",         new Move_DPPScaleCatAllocateAux() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<Real>() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<RealPos>() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<Probability>() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<Integer>() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<Natural>() );
        addTypeWithConstructor("mvDPPAllocateAuxGibbs",            new Move_DPPAllocateAuxGibbsMove<Simplex>() );
        addTypeWithConstructor("mvDPPGibbsConcentration",          new Move_DPPGibbsConcentration( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<Real>( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<RealPos>( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<Natural>( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<Integer>( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<Probability>( ) );
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<RateMatrix>( ) );

        /* Tree proposals (in folder "datatypes/inference/moves/tree") */
        addTypeWithConstructor("mvFNPR",                    new Move_FNPR() );
        addTypeWithConstructor("mvNarrow",                  new Move_NarrowExchange() );
        addTypeWithConstructor("mvNNI",                     new Move_NNIClock() );
        addTypeWithConstructor("mvNNI",                     new Move_NNINonclock() );
        addTypeWithConstructor("mvNNIClock",                new Move_NNIClock() );
        addTypeWithConstructor("mvNNINonclock",             new Move_NNINonclock() );
        addTypeWithConstructor("mvNodeTimeSlideUniform",    new Move_NodeTimeSlideUniform() );
        addTypeWithConstructor("mvOriginTimeSlide",         new Move_OriginTimeSlide() );
        addTypeWithConstructor("mvRateAgeBetaShift",        new Move_RateAgeBetaShift() );
        addTypeWithConstructor("mvRootTimeSlide",           new Move_RootTimeSlide() );
        addTypeWithConstructor("mvSubtreeScale",            new Move_SubtreeScale() );
        addTypeWithConstructor("mvSPR",                     new Move_SPRNonclock() );
        addTypeWithConstructor("mvSubtreePruneRegraft",     new Move_SPRNonclock() );
        addTypeWithConstructor("mvTreeScale",               new Move_TreeScale() );
        
        /* Moves on character histories / data augmentation */
        addTypeWithConstructor("mvNodeCharacterHistoryRejectionSample", new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvNodeCHRS",                            new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCharacterHistoryRejectionSample", new Move_PathCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCHRS",                            new Move_PathCharacterHistoryRejectionSample() );

        /* Moves on continuous phylo processes (Brownian, multivariate Brownian, etc) */
        addTypeWithConstructor("mvMultivariatePhyloProcessTranslation",    new Move_MultivariateRealNodeValTreeTranslation() );
        addTypeWithConstructor("mvMultivariatePhyloProcessSliding",    new Move_MultivariateRealNodeValTreeSliding() );
        addTypeWithConstructor("mvMultivariateRealNodeValTreeTranslation",    new Move_MultivariateRealNodeValTreeTranslation() );
        addTypeWithConstructor("mvMultivariateRealNodeValTreeSliding",    new Move_MultivariateRealNodeValTreeSliding() );
        addTypeWithConstructor("mvConjugateInverseWishartBrownian",    new Move_ConjugateInverseWishartBrownian() );
        addTypeWithConstructor("mvRealNodeValTreeSliding",    new Move_RealNodeValTreeSliding() );
        addTypeWithConstructor("mvRealNodeValTreeTranslation",    new Move_RealNodeValTreeTranslation() );
        addTypeWithConstructor("mvRealPhyloProcessSliding",    new Move_RealNodeValTreeSliding() );
        addTypeWithConstructor("mvScaleSingleACLNRates",    new Move_ScaleSingleACLNRates() );
        addTypeWithConstructor("mvACLNMixingStep",    new Move_ACLNMixingStep() );

        

        ///////////////////////////////////////////////////
        /* Add distributions (in folder "distributions") */
        ///////////////////////////////////////////////////
        
        /* Evolutionary processes (in folder "distributions/evolution") */

        /* Branch rate processes (in folder "distributions/evolution/branchrate") */
        
        // autocorrelated log-normal branch rates relaxed clock model
        addDistribution( "dnAutocorrrelatedLNBranchRates", new Dist_autocorrelatedLnBranchRates() );
        addDistribution( "dnACLN", new Dist_autocorrelatedLnBranchRates() );
		
		
		// branch-rate jump process
        addDistribution( "dnBranchRateJumpProcess", new Dist_branchRateJumpProcess() );
        
        // white noise process
        addDistribution( "dnWhiteNoise",    new Dist_whiteNoise() );
        
        /* trait evolution (in folder "distributions/evolution/branchrate") */

        // brownian motion
        addDistribution( "dnBrownian",              new Dist_brownian() );
        addDistribution( "dnOUP",                   new Dist_ornsteinUhlenbeck() );
        addDistribution( "dnOrnsteinUhlenbeck",     new Dist_ornsteinUhlenbeck() );
        
        // multivariate brownian motion
        addDistribution( "dnBrownianMultiVariate",  new Dist_mvtBrownian() );
  
        /* Character state evolution processes (in folder "distributions/evolution/character") */
        
        // simple phylogenetic CTMC on fixed number of discrete states
        addDistribution( "dnPhyloCTMC", new Dist_phyloCTMC<TimeTree>() );
        addDistribution( "dnPhyloCTMC", new Dist_phyloCTMC<BranchLengthTree>() );
        addDistribution( "dnPhyloDACTMC", new Dist_phyloDACTMC<TimeTree>() );
        addDistribution( "dnPhyloDACTMC", new Dist_phyloDACTMC<BranchLengthTree>() );
                
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
        addDistribution( "dnBDP"                        , new Dist_bdp() );
        addDistribution( "dnBDPConst"                   , new Dist_bdp() );
        addDistribution( "dnBirthDeathConstant"         , new Dist_bdp() );
        
        // constant rate birth-death process with serially sampled tips
        addDistribution( "dnBDPSerial"                  , new Dist_serialBDP() );
        addDistribution( "dnBDPConstSerial"             , new Dist_serialBDP() );
        addDistribution( "dnBirthDeathConstantSerial"   , new Dist_serialBDP() );

        // piecewise constant rate birth-death process with serially sampled tips
        addDistribution( "dnBDPSkySerial"               , new Dist_skySerialBDP() );
        addDistribution( "dnBirthDeathSkySerial"        , new Dist_skySerialBDP() );

        // piecewise constant rate fossilized birth-death process with serially sampled fossils
        addDistribution( "dnBDPSkyFossil"               , new Dist_skyFossilBDP() );
        addDistribution( "dnBirthDeathSkyFossilized"    , new Dist_skyFossilBDP() );

        // diversity-dependent pure-birth process
        addDistribution( "dnYuleDivDep"                 , new Dist_divDepYuleProcess() );
        addDistribution( "dnPBPDivDep"                  , new Dist_divDepYuleProcess() );
        addDistribution( "dnPureBirthDiversityDependent", new Dist_divDepYuleProcess() );
        addDistribution( "dnYuleDiversityDependent"     , new Dist_divDepYuleProcess() );
        
        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnCoalMultiSpeciesConst"      , new Dist_constPopMultispCoal() );
        addDistribution( "dnCoalMSConst"                , new Dist_constPopMultispCoal() );

        // uniform time tree distribution
        addDistribution( "dnUniformTimeTree"            , new Dist_uniformTimeTree() );
        
        // uniform topology distribution
        addDistribution( "dnUniformTopology"            , new Dist_uniformTopology() );
        
        
        /* Statistical distributions on simple variables (in folder "distributions/math") */
        
        // bernoulli distribution
        addDistribution( "dnBernoulli",     new Dist_bernoulli() );

        // binomial distribution
        addDistribution( "dnBinomial",     new Dist_binomial() );
        
        // beta distribution
        addDistribution( "dnBeta",          new Dist_beta() );
        
        // bimodal normal distribution
        addDistribution( "dnBimodalNorm",   new Dist_bimodalNorm() );
        
        // bimodal lognormal distribution
        addDistribution( "dnBimodalLnorm",  new Dist_bimodalLnorm() );
        
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
        addDistribution( "dnExp",           new Dist_offsetExponential() );
        addDistribution( "dnExponential",   new Dist_exponential() );
        addDistribution( "dnExponential",   new Dist_offsetExponential() );
        
        // lognormal distribution
        addDistribution( "dnLnorm",         new Dist_lnorm() );
        addDistribution( "dnLnorm",         new Dist_offsetLnorm() );
        addDistribution( "dnLognormal",     new Dist_lnorm() );
        addDistribution( "dnLognormal",     new Dist_offsetLnorm() );
        
        // normal distribution
        addDistribution( "dnNorm",          new Dist_norm() );
        addDistribution( "dnNormal",        new Dist_norm() );
        
        // LogUniform distribution   
        addDistribution( "dnLogUniform",    new Dist_oneOverX() );
        
        // uniform distribution
        addDistribution( "dnUnif",          new Dist_unif() );
        addDistribution( "dnUnif",          new Dist_positiveUnif() );
        addDistribution( "dnUniform",       new Dist_unif() );
        addDistribution( "dnUniform",       new Dist_positiveUnif() );
        
        // Wishart distribution
        addDistribution( "dnWishart",       new Dist_wishart() );
        
        // inverse Wishart distribution
        addDistribution( "dnInvWishart",       new Dist_inverseWishart() );
        
        
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
		addDistribution( "dnMixture",       new Dist_mixture<RateMatrix>() );
        

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;
        

        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////
        
        /* Basic functions (in folder "functions/basic") */
        
        // regular functions
        addFunction( "citation",                 new Func_citation()                 );
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "contributors",             new Func_contributors()             );
        addFunction( "exists",                   new Func_exists()                   );
        addFunction( "getwd",                    new Func_getwd()                    );
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
        addFunction( "setwd",                    new Func_setwd()                    );
        addFunction( "str",                      new Func_structure()                );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "system",                   new Func_system()                   );
        addFunction( "type",                     new Func_type()                     );
        
        /* Evolution-related functions (in folder "functions/evolution") */
        addFunction( "aveRateOnBranch",             new Func_averageRateOnBranch()         );
        addFunction( "clade",                       new Func_clade()                    );
        addFunction( "rootedTripletDist",           new Func_constructRootedTripletDistribution()            );
        addFunction( "maximumTree",                 new Func_maximumTree()             );
        addFunction( "mrcaIndex",                   new Func_mrcaIndex()                   );
        addFunction( "expBranchTree",               new Func_expBranchTree()            );
        addFunction( "tanhBranchTree",              new Func_tanhBranchTree()            );
        addFunction( "t92GCBranchTree",             new Func_t92GCBranchTree()            );
        addFunction( "phyloRateMultiplier",         new Func_phyloRateMultiplier()      );
        addFunction( "pomoStateConvert",            new Func_pomoStateConverter() );
        addFunction( "pomoRF",                      new Func_pomoRootFrequencies() );
        addFunction( "symDiff",                     new Func_symmetricDifference()      );
        addFunction( "tmrca",                       new Func_tmrca()                    );
        addFunction( "treeAssembly",                new Func_treeAssembly()             );
        addFunction( "treeHeight",                  new Func_treeHeight()               );
        
        
        // nonstandard names (for backward compatibility)
        addFunction( "rateMultiplierPhyloFunction", new Func_phyloRateMultiplier()      );
        addFunction( "expbranchtree",               new Func_expBranchTree()            );

        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( "fnBlosum62", new Func_blosum62());
        addFunction( "fnCpRev",    new Func_cpRev()   );
        addFunction( "fnDayhoff",  new Func_dayhoff() );
        addFunction( "fnF81",      new Func_f81()     );
        addFunction( "fnGTR",      new Func_gtr()     );
        addFunction( "fnHKY",      new Func_hky()     );
        addFunction( "fnJC",       new Func_jc()      );
        addFunction( "fnJones",    new Func_jones()   );
        addFunction( "fnMtMam",    new Func_mtMam()   );
        addFunction( "fnMtRev",    new Func_mtRev()   );
        addFunction( "fnPomo",     new Func_pomo()    );
        addFunction( "fnRtRev",    new Func_rtRev()   );
        addFunction( "fnVT",       new Func_vt()      );
        addFunction( "fnWAG",      new Func_wag()     );
        
        /* rate maps used for data augmentation (in folder "functions/evolution/ratemap") */
        addFunction( "fnBiogeoDE",   new Func_biogeo_de() );
        addFunction( "fnBiogeoGRM",  new Func_biogeo_grm() );

        /* Inference functions (in folder "functions/inference") */

        
        /* Internal functions (in folder "functions/internal") */
        
        /* Note: These are functions that are called implicitly, and the name of which, if
         called explicitly, starts with an underscore character. */
        
        // not templated logical functions
        addFunction( "_and",      new Func__and()   );
        addFunction( "_or",       new Func__or()    );
        addFunction( "_unot",     new Func__unot()  );
        
        // range function (x:y)
        addFunction( "_range",    new Func_range()  );
        
        // logical templated functions
        addFunction( "_eq",       new Func__eq<             Integer,        Integer >()             );
        addFunction( "_eq",       new Func__eq<                Real,           Real >()             );
        addFunction( "_eq",       new Func__eq<             Integer,           Real >()             );
        addFunction( "_eq",       new Func__eq<                Real,        Integer >()             );
        addFunction( "_eq",       new Func__eq<           RlBoolean,      RlBoolean >()             );
        addFunction( "_eq",       new Func__eq<           RlString,       RlString  >()             );
        addFunction( "_ge",       new Func__ge<             Integer,        Integer >()             );
        addFunction( "_ge",       new Func__ge<                Real,           Real >()             );
        addFunction( "_ge",       new Func__ge<             Integer,           Real >()             );
        addFunction( "_ge",       new Func__ge<                Real,        Integer >()             );
        addFunction( "_ge",       new Func__ge<           RlBoolean,      RlBoolean >()             );
        addFunction( "_gt",       new Func__gt<             Integer,        Integer >()             );
        addFunction( "_gt",       new Func__gt<                Real,           Real >()             );
        addFunction( "_gt",       new Func__gt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_le",       new Func__le<             Integer,        Integer >()             );
        addFunction( "_le",       new Func__le<                Real,           Real >()             );
        addFunction( "_le",       new Func__le<             Integer,           Real >()             );
        addFunction( "_le",       new Func__le<                Real,        Integer >()             );
        addFunction( "_le",       new Func__le<           RlBoolean,      RlBoolean >()             );
        addFunction( "_lt",       new Func__lt<             Integer,        Integer >()             );
        addFunction( "_lt",       new Func__lt<                Real,           Real >()             );
        addFunction( "_lt",       new Func__lt<           RlBoolean,      RlBoolean >()             );
        addFunction( "_ne",       new Func__ne<             Integer,        Integer >()             );
        addFunction( "_ne",       new Func__ne<                Real,           Real >()             );
        addFunction( "_ne",       new Func__ne<             Integer,           Real >()             );
        addFunction( "_ne",       new Func__ne<                Real,        Integer >()             );
        addFunction( "_ne",       new Func__ne<           RlBoolean,      RlBoolean >()             );
        
        // unary minus (e.g. -a)
        addFunction( "_uminus",   new Func__uminus<Integer, Integer>()  );
        addFunction( "_uminus",   new Func__uminus<Natural, Integer>()  );
        addFunction( "_uminus",   new Func__uminus<Real, Real>()        );
        addFunction( "_uminus",   new Func__uminus<RealPos, Real>()     );
        
        // addition (e.g. a+b )
        addFunction( "_add",      new Func__add< Natural                , Natural               , Natural               >(  )   );
        addFunction( "_add",      new Func__add< Integer                , Integer               , Integer               >(  )   );
        addFunction( "_add",      new Func__add< Real                   , Real                  , Real                  >(  )   );
        addFunction( "_add",      new Func__add< RealPos                , RealPos               , RealPos               >(  )   );
        addFunction( "_add",      new Func__add< RlString               , RlString              , RlString              >(  )   );
        addFunction( "_add",      new Func__add< RlString               , Real                  , RlString              >(  )   );
        addFunction( "_add",      new Func__add< RlString               , Integer               , RlString              >(  )   );
        addFunction( "_add",      new Func__add< ModelVector<Natural>   , ModelVector<Natural>  , ModelVector<Natural>       >(  )   );
        addFunction( "_add",      new Func__add< ModelVector<Integer>   , ModelVector<Integer>  , ModelVector<Integer>       >(  )   );
        addFunction( "_add",      new Func__add< ModelVector<RealPos>   , ModelVector<RealPos>  , ModelVector<RealPos>       >(  )   );
        addFunction( "_add",      new Func__add< ModelVector<Real>      , ModelVector<Real>     , ModelVector<Real>          >(  )   );
        addFunction( "_add",      new Func__scalarVectorAdd<Natural     , ModelVector<Natural>  , ModelVector<Natural>       >(  )   );
        addFunction( "_add",      new Func__scalarVectorAdd<Integer     , ModelVector<Integer>  , ModelVector<Integer>       >(  )   );
        addFunction( "_add",      new Func__scalarVectorAdd<Real        , ModelVector<Real>     , ModelVector<Real>          >(  )   );
        addFunction( "_add",      new Func__scalarVectorAdd<RealPos     , ModelVector<RealPos>  , ModelVector<RealPos>       >(  )   );
        
        addFunction( "_add",      new Func__rladd< AbstractCharacterData  , AbstractCharacterData , AbstractCharacterData >(  )   );
        
        // division
        addFunction( "_div",      new Func__div< Natural                            , RealPos               , RealPos                   >(  )  );
        addFunction( "_div",      new Func__div< RealPos                            , Natural               , RealPos                   >(  )  );
        addFunction( "_div",      new Func__div< Integer                            , Real                  , Real                      >(  )  );
        addFunction( "_div",      new Func__div< Real                               , Integer               , Real                      >(  )  );
        addFunction( "_div",      new Func__div< Real                               , Real                  , Real                      >(  )  );
        addFunction( "_div",      new Func__div< RealPos                            , RealPos               , RealPos                   >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<Natural>               , ModelVector<RealPos>  , ModelVector<RealPos>      >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<RealPos>               , ModelVector<Natural>  , ModelVector<RealPos>      >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<Integer>               , ModelVector<Real>     , ModelVector<Real>         >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<Real>                  , ModelVector<Integer>  , ModelVector<Real>         >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<RealPos>               , ModelVector<RealPos>  , ModelVector<RealPos>      >(  )  );
        addFunction( "_div",      new Func__div< ModelVector<Real>                  , ModelVector<Real>     , ModelVector<RealPos>      >(  )  );
        addFunction( "_div",      new Func__vectorScalarDiv<Natural                 , RealPos               , RealPos                   >(  )   );
        addFunction( "_div",      new Func__vectorScalarDiv<RealPos                 , Natural               , RealPos                   >(  )   );
        addFunction( "_div",      new Func__vectorScalarDiv<Integer                 , Real                  , Real                      >(  )   );
        addFunction( "_div",      new Func__vectorScalarDiv<Real                    , Integer               , Real                      >(  )   );
        addFunction( "_div",      new Func__vectorScalarDiv<Real                    , Real                  , Real                      >(  )   );
        addFunction( "_div",      new Func__vectorScalarDiv<RealPos                 , RealPos               , RealPos                   >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<RealPos                 , Natural               , RealPos                   >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<Natural                 , RealPos               , RealPos                   >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<Real                    , Integer               , Real                      >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<Integer                 , Real                  , Real                      >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<Real                    , Real                  , Real                      >(  )   );
        addFunction( "_div",      new Func__scalarVectorDiv<RealPos                 , RealPos               , RealPos                   >(  )   );
        
        // multiplication
        addFunction( "_mul",      new Func__mult< Natural               , Natural               , Natural               >(  )  );
        addFunction( "_mul",      new Func__mult< Integer               , Integer               , Integer               >(  )  );
        addFunction( "_mul",      new Func__mult< Real                  , Real                  , Real                  >(  )  );
        addFunction( "_mul",      new Func__mult< RealPos               , RealPos               , RealPos               >(  )  );
        addFunction( "_mul",      new Func__mult< ModelVector<Natural>  , ModelVector<Natural>  , ModelVector<Natural>  >(  )  );
        addFunction( "_mul",      new Func__mult< ModelVector<Integer>  , ModelVector<Integer>  , ModelVector<Integer>  >(  )  );
        addFunction( "_mul",      new Func__mult< ModelVector<RealPos>  , ModelVector<RealPos>  , ModelVector<RealPos>  >(  )  );
        addFunction( "_mul",      new Func__mult< ModelVector<Real>     , ModelVector<Real>     , ModelVector<Real>     >(  )  );
        addFunction( "_mul",      new Func__scalarVectorMult<Natural    , ModelVector<Natural>  , ModelVector<Natural>  >(  )   );
        addFunction( "_mul",      new Func__scalarVectorMult<Integer    , ModelVector<Integer>  , ModelVector<Integer>  >(  )   );
        addFunction( "_mul",      new Func__scalarVectorMult<Real       , ModelVector<Real>     , ModelVector<Real>     >(  )   );
        addFunction( "_mul",      new Func__scalarVectorMult<RealPos    , ModelVector<RealPos>  , ModelVector<RealPos>  >(  )   );
        
        // subtraction
        addFunction( "_sub",      new Func__sub< Integer                            , Integer               , Integer               >(  )  );
        addFunction( "_sub",      new Func__sub< Real                               , Real                  , Real                  >(  )  );
        addFunction( "_sub",      new Func__sub< ModelVector<Integer>               , ModelVector<Integer>  , ModelVector<Integer>  >(  )  );
        addFunction( "_sub",      new Func__sub< ModelVector<Real>                  , ModelVector<Real>     , ModelVector<Real>     >(  )  );
        addFunction( "_sub",      new Func__vectorScalarDiv<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( "_sub",      new Func__vectorScalarDiv<Real                    , Real                  , Real                      >(  )   );
        addFunction( "_sub",      new Func__scalarVectorSub<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( "_sub",      new Func__scalarVectorSub<Real                    , Real                  , Real                      >(  )   );
        
        // modulo
        addFunction( "_mod",      new Func__mod() );
        
        // exponentiation
        addFunction( "_exp",      new Func_power() );
        addFunction( "_exp",      new Func_powerVector() );
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "print",                       new Func_write()                       );
        addFunction( "readAtlas",                   new Func_readAtlas()                   );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
        addFunction( "readTaxonData",               new Func_TaxonReader()                 );
        addFunction( "readTrace",                   new Func_readTrace()                   );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()               );
        addFunction( "source",                      new Func_source()                      );
        addFunction( "write",                       new Func_write()                       );
        addFunction( "writeFasta",                  new Func_writeFasta()                  );
        addFunction( "writeNexus",                  new Func_writeNexus()                  );

        
        /* Math functions (in folder "functions/math") */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
		
		// ceil function
        addFunction( "ceil",        new Func_ceil<Real,Integer>()  );
        addFunction( "ceil",        new Func_ceil<RealPos,Natural>()  );
        addFunction( "ceiling",     new Func_ceil<Real,Integer>()  );
        addFunction( "ceiling",     new Func_ceil<RealPos,Natural>()  );
        
        // exponential function
        addFunction( "exp",         new Func_exp() );
		
		// floor function
        addFunction( "floor",       new Func_floor<Real,Integer>()  );
        addFunction( "floor",       new Func_floor<RealPos,Natural>()  );
        
        // natural log function
        addFunction( "ln",          new Func_ln()  );
        
        // log function
		addFunction( "log",         new Func_log()  );
        
        // mean function
		addFunction( "mean",        new Func_mean()  );
        
        // min function
		addFunction( "min",         new Func_min()  );

        // normalize vector function
		addFunction( "normalize",   new Func_normalize()  );

		// power function
        addFunction( "power",       new Func_power() );
        addFunction( "power",       new Func_powerVector() );

        // powermix function (TODO: remove when user functions work)
        addFunction( "powermix",    new Func_powermix() );
        
		// round function
        addFunction( "round",       new Func_round<Real,Integer>()  );
        addFunction( "round",       new Func_round<RealPos,Natural>()  );
		
        // simplex constructor function (from RealPos ellipsis argument values)
        addFunction( "simplex",   new Func_simplex()                  );

        // simplex constructor function (from vector of RealPos values)
        addFunction( "simplex",   new Func_normalize()                );

		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
        
        // mean function
		addFunction( "sum",       new Func_sum()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );


 		/* Statistics functions (in folder "functions/statistics") */
		
		// some helper statistics for the DPP distribution
        addFunction("fnDPPConcFromMean",  new Func_dppConcFromMean( )     );
        addFunction("fnDPPMeanFromConc",  new Func_dppMeanFromConc( )  );
		
		// count the number of unique elements in vector
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<Real>( )  );
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<RealPos>( )  );
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<Integer>( )  );
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<Natural>( )  );
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<Probability>( )  );
        addFunction("fnNumUniqueInVector",  new Func_numUniqueInVector<Simplex>( )  );

        // return a distcretized (by quantile) and normalized vector from a continuous distribution
        addFunction( "fnNormalizedQuantile",             new Func_fnNormalizedQuantile<Real>()    );
        addFunction( "fnNormalizedQuantile",             new Func_fnNormalizedQuantile<RealPos>()    );
        
        // return a discretized gamma distribution (for gamma-dist rates)
        addFunction( "fnDiscretizeGamma",             new Func_discretizeGamma( )   );

        
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
        
        // dirichlet distribution
        addFunction("ddirichlet", new DistributionFunctionPdf<Simplex>( new Dist_dirichlet() ) );
        addFunction("rdirichlet", new DistributionFunctionRv<Simplex>( new Dist_dirichlet() ) );
        
        // exponential distribution
        addFunction("dexp", new DistributionFunctionPdf<RealPos>( new Dist_exponential() ) );
        addFunction("pexp", new DistributionFunctionCdf( new Dist_exponential() ) );
        addFunction("qexp", new DistributionFunctionQuantile( new Dist_exponential() ) );
        addFunction("rexp", new DistributionFunctionRv<RealPos>( new Dist_exponential() ) );
        
        // gamma distribution
        addFunction("dgamma", new DistributionFunctionPdf<RealPos>( new Dist_gamma() ) );
        addFunction("pgamma", new DistributionFunctionCdf( new Dist_gamma() ) );
        addFunction("qgamma", new DistributionFunctionQuantile( new Dist_gamma() ) );
        addFunction("rgamma", new DistributionFunctionRv<RealPos>( new Dist_gamma() ) );
        
        // lognormal distribution
        addFunction("dlnorm", new DistributionFunctionPdf<RealPos>( new Dist_lnorm() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_lnorm() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new Dist_lnorm() ) );
        addFunction("rlnorm", new DistributionFunctionRv<RealPos>( new Dist_lnorm() ) );
        addFunction("dlnorm", new DistributionFunctionPdf<Real>( new Dist_offsetLnorm() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new Dist_offsetLnorm() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new Dist_offsetLnorm() ) );
        addFunction("rlnorm", new DistributionFunctionRv<Real>( new Dist_offsetLnorm() ) );
        
        // normal distribution
        addFunction("dnorm", new DistributionFunctionPdf<Real>( new Dist_norm() ) );
        addFunction("pnorm", new DistributionFunctionCdf( new Dist_norm() ) );
        addFunction("qnorm", new DistributionFunctionQuantile( new Dist_norm() ) );
        addFunction("rnorm", new DistributionFunctionRv<Real>( new Dist_norm() ) );
        
        //
        addFunction("dpoisson", new DistributionFunctionPdf<Natural>( new Dist_poisson() ) );
        addFunction("rpoisson", new DistributionFunctionRv<Natural>( new Dist_poisson() ) );
        
        // uniform distribution
        addFunction("dunif", new DistributionFunctionPdf<Real>( new Dist_unif() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_unif() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new Dist_unif() ) );
        addFunction("runif", new DistributionFunctionRv<Real>( new Dist_unif() ) );
        addFunction("dunif", new DistributionFunctionPdf<RealPos>( new Dist_positiveUnif() ) );
        addFunction("punif", new DistributionFunctionCdf( new Dist_positiveUnif() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new Dist_positiveUnif() ) );
        addFunction("runif", new DistributionFunctionRv<RealPos>( new Dist_positiveUnif() ) );


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


