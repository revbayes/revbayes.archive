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

/// Monitors ///

/* Monitor types (in folder "monitors) */
#include "RlMonitor.h"
#include "Mntr_AncestralState.h"
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


///* Moves on covariance matrices */
#include "Move_MatrixRealSymmetricSimple.h"


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
#include "Func_symmetricDifference.h"
#include "Func_tmrca.h"
#include "Func_treeAssembly.h"


/* Rate matrix functions (in folder "functions/evolution/ratematrix") */
#include "Func_blosum62.h"
#include "Func_chromosomes.h"
#include "Func_cpRev.h"
#include "Func_dayhoff.h"
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
#include "Func_mapTree.h"
#include "Func_module.h"
#include "Func_readAtlas.h"
#include "Func_readContinuousCharacterData.h"
#include "Func_readDiscreteCharacterData.h"
#include "Func_readTSVCharacterData.h"
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
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_max.h"
#include "Func_mean.h"
#include "Func_min.h"
#include "Func_normalize.h"
#include "Func_power.h"
#include "Func_powerVector.h"
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
#include "Func_varianceCovarianceMatrix.h"


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
        AddWorkspaceVectorType<Taxon,5>::addTypeToWorkspace( *this, new Taxon() );
        
        
        AddWorkspaceVectorType<RateMatrix,5>::addTypeToWorkspace( *this, new RateMatrix() );
        AddWorkspaceVectorType<AbstractDiscreteCharacterData,5>::addTypeToWorkspace( *this, new AbstractDiscreteCharacterData() );
        
        AddWorkspaceVectorType<TimeTree,3>::addTypeToWorkspace( *this, new TimeTree() );
		AddWorkspaceVectorType<BranchLengthTree,3>::addTypeToWorkspace( *this, new BranchLengthTree() );
        AddWorkspaceVectorType<Clade,3>::addTypeToWorkspace( *this, new Clade() );
		
		addFunction("v", new Func_workspaceVector<AncestralStateTrace>() );
        
//        AddVectorizedWorkspaceType<Monitor,3>::addTypeToWorkspace( *this, new Monitor() );
        addFunction("v", new Func_workspaceVector<Monitor>() );
        
        //        AddVectorizedWorkspaceType<Move,3>::addTypeToWorkspace( *this, new Move() );
        addFunction("v", new Func_workspaceVector<Move>() );
        
        addFunction("v", new Func_workspaceVector<StoppingRule>() );
        
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
        addType( new MatrixReal()           );



        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "beca",                   new BurninEstimationConvergenceAssessment()   );
        addTypeWithConstructor( "model",                  new Model()                                   );
        addTypeWithConstructor( "pathSampler",            new PathSampler()                             );
        addTypeWithConstructor( "steppingStoneSampler",   new SteppingStoneSampler()                    );

        /* Add stopping rules (in folder "analysis/stoppingRules") (alphabetic order) */
        addTypeWithConstructor( "srMaxIteration",       new MaxIterationStoppingRule()   );
        addTypeWithConstructor( "srMaxTime",            new MaxTimeStoppingRule()   );
        

        ////////////////////////////////////////////////////////////////////////////////
        /* Add monitors (in folder "datatypes/inference/monitors") (alphabetic order) */
        ////////////////////////////////////////////////////////////////////////////////

		addTypeWithConstructor("mnAncestralState",      new Mntr_AncestralState<TimeTree>());
		addTypeWithConstructor("mnAncestralState",      new Mntr_AncestralState<BranchLengthTree>());
        addTypeWithConstructor("mnExtNewick",           new Mntr_ExtendedNewickFile());
        addTypeWithConstructor("mnFile",                new Mntr_File());
        addTypeWithConstructor("mnModel",               new Mntr_Model());
        addTypeWithConstructor("mnPosteriorPredictive", new Mntr_PosteriorPredictive());
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
        addTypeWithConstructor("mvSingleElementScale",          new Move_SingleElementScale() );
        addTypeWithConstructor("mvVectorScale",                 new Move_VectorScale() );
        addTypeWithConstructor("mvVectorSingleElementScale",    new Move_VectorSingleElementScale() );
        addTypeWithConstructor("mvVectorSingleElementSliding",  new Move_VectorSingleElementSlide() );
        
        /* Moves on matrices of real values */
        addTypeWithConstructor("mvMatrixSingleElementSliding",  new Move_MatrixSingleElementSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor("mvSymmetricMatrixSimple",       new Move_MatrixRealSymmetricSimple() );

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
        
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Real>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<RealPos>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Natural>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Integer>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Probability>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Simplex>( ) );

        /* Tree proposals (in folder "datatypes/inference/moves/tree") */
		addTypeWithConstructor("mvEmpiricalTree",           new Move_EmpiricalTree<BranchLengthTree>() );
		addTypeWithConstructor("mvEmpiricalTree",           new Move_EmpiricalTree<TimeTree>() );
        addTypeWithConstructor("mvFNPR",                    new Move_FNPR() );
        addTypeWithConstructor("mvGPR",                     new Move_GibbsPruneAndRegraft() );
        addTypeWithConstructor("mvNarrow",                  new Move_NarrowExchange() );
        addTypeWithConstructor("mvNNI",                     new Move_NNIClock() );
        addTypeWithConstructor("mvNNI",                     new Move_NNINonclock() );
        addTypeWithConstructor("mvNNIClock",                new Move_NNIClock() );
        addTypeWithConstructor("mvNNINonclock",             new Move_NNINonclock() );
        addTypeWithConstructor("mvNodeTimeSlideUniform",    new Move_NodeTimeSlideUniform() );
        addTypeWithConstructor("mvNodeTimeSlideBeta",       new Move_NodeTimeSlideBeta() );
        addTypeWithConstructor("mvOriginTimeSlide",         new Move_OriginTimeSlide() );
        addTypeWithConstructor("mvRateAgeBetaShift",        new Move_RateAgeBetaShift() );
        addTypeWithConstructor("mvRootTimeSlide",           new Move_RootTimeSlide() );
        addTypeWithConstructor("mvSubtreeScale",            new Move_SubtreeScale() );
        addTypeWithConstructor("mvSPR",                     new Move_SPRNonclock() );
        addTypeWithConstructor("mvSubtreePruneRegraft",     new Move_SPRNonclock() );
        addTypeWithConstructor("mvTreeScale",               new Move_TreeScale() );
        
        /* Moves on character histories / data augmentation */
        addTypeWithConstructor("mvCharacterHistory",                    new Move_CharacterHistory<BranchLengthTree>() );
        addTypeWithConstructor("mvCharacterHistory",                    new Move_CharacterHistory<TimeTree>() );
        addTypeWithConstructor("mvNodeCharacterHistoryRejectionSample", new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvNodeCHRS",                            new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCharacterHistoryRejectionSample", new Move_PathCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCHRS",                            new Move_PathCharacterHistoryRejectionSample() );

        

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
        addDistribution( "dnPhyloCTMCEpoch",            new Dist_phyloCTMCEpoch() );
        addDistribution( "dnPhyloCTMCClado",            new Dist_phyloCTMCClado<TimeTree>() );
        addDistribution( "dnPhyloCTMCClado",            new Dist_phyloCTMCClado<BranchLengthTree>() );
        
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
//        addDistribution( "dnBDP",                       new Dist_bdp() );
        AddDistribution<TimeTree>("BDP", new Dist_bdp());
        addDistribution( "dnBDPConst",                  new Dist_bdp() );
        addDistribution( "dnBirthDeathConstant",        new Dist_bdp() );
        
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
        
        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
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
        addDistribution( "dnBimodalNorm",   new Dist_bimodalNorm() );
        
        // bimodal lognormal distribution
        addDistribution( "dnBimodalLnorm",  new Dist_bimodalLnorm() );
        
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
        
        // normal distribution
        addDistribution("dnMultivariateNormal", new Dist_multivariateNorm());
        addDistribution("dnMVNormal",           new Dist_multivariateNorm());
        
        // normal distribution
        AddContinuousDistribution<Real>("Normal", new Dist_norm());
//        addDistribution( "dnNorm",          new Dist_norm() );
//        addDistribution( "dnNormal",        new Dist_norm() );
        
        // LogUniform distribution   
        addDistribution( "dnLogUniform",    new Dist_logUniform() );
        
        // LogUniform distribution
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
        addDistribution( "dnMixture",       new Dist_mixture<RateMatrix>() );
        
        // mixture distribution
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Real>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<RealPos>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Natural>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Integer>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Probability>() );
        addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<Simplex>() );
        //addDistribution( "dnReversibleJumpMixture",       new Dist_reversibleJumpMixtureConstant<RateMatrix>() );
        // aliases
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Real>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<RealPos>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Natural>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Integer>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Probability>() );
        addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<Simplex>() );
        //addDistribution( "dnRJMixture",       new Dist_reversibleJumpMixtureConstant<RateMatrix>() );
        

        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;
        

        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////
        
        /* Basic functions (in folder "functions/basic") */
        
        // regular functions
        addFunction( "clear",                    new Func_clear()                    );
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
        addFunction( "maximumTree",                 new Func_maximumTree()             );
        addFunction( "mrcaIndex",                   new Func_mrcaIndex()                   );
        addFunction( "pomoStateConvert",            new Func_pomoStateConverter() );
        addFunction( "pomoRF",                      new Func_pomoRootFrequencies() );
        addFunction( "symDiff",                     new Func_symmetricDifference()      );
        addFunction( "tmrca",                       new Func_tmrca()                    );
        addFunction( "treeAssembly",                new Func_treeAssembly()             );

        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( "fnBlosum62", new Func_blosum62());
        addFunction( "fnChromosomes", new Func_chromosomes());
        addFunction( "fnCpRev",    new Func_cpRev()   );
        addFunction( "fnDayhoff",  new Func_dayhoff() );
        addFunction( "fnF81",      new Func_f81()     );
        addFunction( "fnFreeBinary", new Func_FreeBinary() );
        addFunction( "fnFreeK",    new Func_FreeK()   );
        addFunction( "fnGTR",      new Func_gtr()     );
        addFunction( "fnHKY",      new Func_hky()     );
        addFunction( "fnJC",       new Func_jc()      );
        addFunction( "fnJones",    new Func_jones()   );
        addFunction( "fnK80",      new Func_k80()     );
        addFunction( "fnMtMam",    new Func_mtMam()   );
        addFunction( "fnMtRev",    new Func_mtRev()   );
        addFunction( "fnPomo",     new Func_pomo()    );
        addFunction( "fnRtRev",    new Func_rtRev()   );
        addFunction( "fnT92",      new Func_t92()     );
        addFunction( "fnVT",       new Func_vt()      );
        addFunction( "fnWAG",      new Func_wag()     );
        
        /* rate maps used for data augmentation (in folder "functions/evolution/ratemap") */
        addFunction( "fnBiogeoDE",   new Func_biogeo_de() );
        addFunction( "fnBiogeoGRM",  new Func_biogeo_grm() );
        
        /* cladogenic probs used for e.g. DEC models (in folder "functions/evolution") */
        addFunction( "fnCladoProbs", new Func_cladoProbs() );

    
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
        addFunction( "_sub",      new Func__vectorScalarSub<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( "_sub",      new Func__vectorScalarSub<Real                    , Real                  , Real                      >(  )   );
        addFunction( "_sub",      new Func__scalarVectorSub<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( "_sub",      new Func__scalarVectorSub<Real                    , Real                  , Real                      >(  )   );
        
        // modulo
        addFunction( "_mod",      new Func__mod() );
        
        // exponentiation
        addFunction( "_exp",      new Func_power() );
        addFunction( "_exp",      new Func_powerVector() );
        
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
        
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( "ancestralStateTree",          new Func_ancestralStateTree<BranchLengthTree>() );
		addFunction( "ancestralStateTree",          new Func_ancestralStateTree<TimeTree>() );
		addFunction( "annotateHPDAges",             new Func_annotateHPDAges<TimeTree>()    );
		addFunction( "consensusTree",				new Func_consensusTree<BranchLengthTree>() );
		addFunction( "consensusTree",               new Func_consensusTree<TimeTree>()      );
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
        addFunction( "readTaxonData",               new Func_TaxonReader()                  );
        addFunction( "readTrace",                   new Func_readTrace()                    );
        addFunction( "readTrees",                   new Func_readTrees()                    );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()                );
		addFunction( "readTSVCharacterData",        new Func_readTSVCharacterData()         );
        addFunction( "source",                      new Func_source()                       );
        addFunction( "write",                       new Func_write()                        );
        addFunction( "writeFasta",                  new Func_writeFasta()                   );
        addFunction( "writeNexus",                  new Func_writeNexus()                   );

        
        /* Math functions (in folder "functions/math") */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
		
		// ceil function
        addFunction( "ceil",        new Func_ceil<Real,Integer>()  );
        addFunction( "ceil",        new Func_ceil<RealPos,Natural>()  );
        addFunction( "ceiling",     new Func_ceil<Real,Integer>()  );
        addFunction( "ceiling",     new Func_ceil<RealPos,Natural>()  );
        
        // diagonal matrix
        addFunction("diag",         new Func_diagonalMatrix() );
        
        // exponential function
        addFunction( "exp",         new Func_exp() );
		
		// floor function
        addFunction( "floor",       new Func_floor<Real,Integer>()  );
        addFunction( "floor",       new Func_floor<RealPos,Natural>()  );
        
        // natural log function
        addFunction( "ln",          new Func_ln()  );
        
        // log function
        addFunction( "log",         new Func_log()  );
        
        // min function
        addFunction( "max",         new Func_max()  );
        
        // mean function
		addFunction( "mean",        new Func_mean()  );
        
        // min function
		addFunction( "min",         new Func_min()  );

        // normalize vector function
		addFunction( "normalize",   new Func_normalize()  );

		// power function
        addFunction( "power",       new Func_power() );
        addFunction( "power",       new Func_powerVector() );
        
		// round function
        addFunction( "round",       new Func_round<Real,Integer>()  );
        addFunction( "round",       new Func_round<RealPos,Natural>()  );
		
        // simplex constructor function (from RealPos ellipsis argument values)
        addFunction( "simplex",   new Func_simplex()                  );

        // simplex constructor function (from vector of RealPos values)
        addFunction( "simplex",   new Func_simplexFromVector()        );

		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
        
        // sum function
        addFunction( "sum",       new Func_sum()  );
        addFunction( "sum",       new Func_sumPositive()  );
        
        // standard deviation function
        addFunction( "stdev",     new Func_standardDeviation()  );
        addFunction( "sd",        new Func_standardDeviation()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );
        
        // mean function
        addFunction( "var",       new Func_variance()  );

        
        
        /* Statistics functions (in folder "functions/statistics") */
        
        // MCMC constructor function
        addFunction( "mcmc",   new Func_Mcmc() );
        
        // MCMCMC constructor function
        addFunction( "mcmcmc",   new Func_Mcmcmc() );
        
        // power-posterior constructor function
        addTypeWithConstructor( "powerPosterior",  new PowerPosteriorAnalysis() );

        
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
        
        addFunction( "fnDiscretizeDistribution", new Func_discretizeDistribution( )         );
        addFunction( "fnDiscretizeDistribution", new Func_discretizePositiveDistribution( ) );
        
        // return a discretized gamma distribution (for gamma-dist rates)
        addFunction( "fnDiscretizeGamma",      new Func_discretizeGamma( )   );

        addFunction( "fnVarCovar",             new Func_varianceCovarianceMatrix( )   );
        
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
        
        
        addFunction("rPhyloCTMC", new DistributionFunctionRv< AbstractDiscreteCharacterData >( new Dist_phyloCTMC<TimeTree>() ) );
        addFunction("rPhyloCTMC", new DistributionFunctionRv< AbstractDiscreteCharacterData >( new Dist_phyloCTMC<BranchLengthTree>() ) );


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


