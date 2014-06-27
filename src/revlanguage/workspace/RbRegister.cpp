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
 *
 * (c) Copyright 2009-
 * @license GPL version 3
 *
 */

#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

/* Files including helper classes */
#include "RbException.h"
#include "RlUserInterface.h"
#include "Workspace.h"

/// Miscellaneous types ///

/* Primitive types (in folder "datatypes/basic") */
#include "Complex.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

/* Container types (in folder "datatypes/container") */
#include "Vector.h"

/* Evolution types (in folder "datatypes/evolution") */

/* Character state types (in folder "datatypes/evolution/character") */
#include "RlAminoAcidState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"

/* Character data types (in folder "datatypes/evolution/datamatrix") */

/* Tree types (in folder "datatypes/evolution/trees") */
#include "RlClade.h"

/* Inference types (in folder "datatypes/inference") */
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlParallelMcmcmc.h"
#include "RlPathSampler.h"
#include "RlPowerPosterior.h"
#include "RlSteppingStoneSampler.h"

/// Monitors ///

/* Monitor types (in folder "datatypes/inference/monitors) */
#include "RlMonitor.h"
#include "RlFileMonitor.h"
#include "RlExtendedNewickFileMonitor.h"
#include "RlModelMonitor.h"
#include "RlScreenMonitor.h"

/// Moves ///

/* Move types (in folder "datatypes/inference/moves") (grouped by parameter type) */
#include "RlMove.h"

/* Moves on real values */
#include "RlScaleMove.h"
#include "RlSlidingMove.h"

/* Moves on integer values */
#include "RlRandomIntegerWalkMove.h"
#include "RlRandomGeometricWalkMove.h"

/* Moves on simplices */
#include "RlSimplexMove.h"
#include "RlSimplexSingleElementScale.h"

/* Moves on real valued vectors */
#include "RlRlcRateScaleMove.h"
#include "RlSingleElementScale.h"
#include "RlSwitchRateJumpMove.h"
#include "RlVectorSingleElementScaleMove.h"
#include "RlVectorSingleElementSlidingMove.h"
#include "RlVectorScale.h"

/* Moves on precision matrices */
#include "RlVectorSingleElementSlidingMove.h"

/* Tree proposals (in folder "datatypes/inference/moves/tree") */
#include "RlFixedNodeheightPruneRegraft.h"
#include "RlNarrowExchange.h"
#include "RlNearestNeighborInterchange.h"
#include "RlNearestNeighborInterchange_nonClock.h"
#include "RlNodeTimeSlideUniform.h"
#include "RlRateAgeBetaShift.h"
#include "RlRootTimeSlide.h"
#include "RlSubtreeScale.h"
#include "RlTreeScale.h"
#include "RlWeightedNodeTimeSlide.h"

/* Math types (in folder "datatypes/math") */
#include "RealMatrix.h"
#include "RealSymmetricMatrix.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"

/// Distributions ///

/* Distribution types (in folder "distributions") */

/* Character evolution models (in folder "distributions/evolution/character") */
#include "RlCharacterStateEvolutionAlongTree.h"

/* Branch rate priors (in folder "distributions/evolution/tree") */
#include "RlBranchRateJumpProcess.h"
#include "RlBrownianPhyloProcess.h"
#include "RlWhiteNoisePhyloProcess.h"

/* Tree priors (in folder "distributions/evolution/tree") */
#include "RlConstantRateBirthDeathProcess.h"
#include "RlConstantRateSerialSampledBirthDeathProcess.h"
#include "RlPiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RlPiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RlDiversityDependentPureBirthProcess.h"
#include "RlMultispeciesCoalescentConstantPopulationProcess.h"
#include "RlPiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RlUniformTimeTreeDistribution.h"

/* Distributions on simple variables (in folder "distributions/math") */
#include "RlBetaDistribution.h"
#include "RlBernoulliDistribution.h"
#include "RlBimodalLognormalDistribution.h"
#include "RlBimodalNormalDistribution.h"
#include "RlDirichletDistribution.h"
#include "RlExponentialBranchTree.h"
#include "RlExponentialDistribution.h"
#include "RlGammaDistribution.h"
#include "RlGeometricDistribution.h"
#include "RlPoissonDistribution.h"
#include "RlLognormalDistribution.h"
#include "RlNormalDistribution.h"
#include "RlOffsetExponentialDistribution.h"
#include "RlOffsetLognormalDistribution.h"
#include "RlOneOverXDistribution.h"
#include "RlPositiveUniformDistribution.h"
#include "RlUniformDistribution.h"
#include "RlUniformTopologyDistribution.h"
#include "RlWishartDistribution.h"

/* Mixture distributions (in folder "distributions/mixture") */
#include "RlDirichletProcessPriorDistribution.h"

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
#include "Func_help.h"
#include "Func_license.h"
#include "Func_ls.h"
#include "Func_quit.h"
#include "Func_range.h"
#include "Func_rlvector.h"
#include "Func_seed.h"
#include "Func_seq.h"
#include "Func_structure.h"
#include "Func_type.h"
#include "Func_vector.h"


/* Functions related to evolution (in folder "functions/evolution") */
#include "ConstructorClade.h"
#include "RlRateMultiplierPhyloFunction.h"
#include "RlTmrcaStatistic.h"
#include "RlTreeHeightStatistic.h"
#include "RlTreeAssemblyFunction.h"

/* Rate matrix functions (in folder "functions/evolution/ratematrix") */
#include "Func_gtr.h"
#include "RlBlosum62RateMatrixFunction.h"
#include "RlCpRevRateMatrixFunction.h"
#include "RlDayhoffRateMatrixFunction.h"
#include "RlF81RateMatrixFunction.h"
#include "RlHkyRateMatrixFunction.h"
#include "RlJcRateMatrixFunction.h"
#include "RlJonesRateMatrixFunction.h"
#include "RlMtRevRateMatrixFunction.h"
#include "RlMtMamRateMatrixFunction.h"
#include "RlRtRevRateMatrixFunction.h"
#include "RlVtRateMatrixFunction.h"
#include "RlWagRateMatrixFunction.h"


/* Inference functions (in folder "functions/inference") */

/* Convergence functions (in folder "functions/inference/convergence") */
#include "OptimalBurninFunction.h"
#include "BurninEstimationConvergenceAssessmentFunction.h"


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

#include "Func__and.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__unot.h"
#include "Func__add.h"
#include "Func__div.h"
#include "Func__mult.h"
#include "Func__sub.h"
#include "Func__uminus.h"


/* Input/output functions (in folder "functions/io") */
#include "Func_mapTree.h"
#include "Func_readCharacterData.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_source.h"
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
#include "Func_normalizeVector.h"
#include "Func_power.h"
#include "Func_powermix.h"
#include "Func_round.h"
#include "Func_simplex.h"
#include "Func_sqrt.h"
#include "Func_trunc.h"


/* Statistics functions (in folder "functions/statistics") */
/* These are functions related to statistical distributions */
#include "RlDPPConcFromPriorMean.h"
#include "RlDPPNumFromConcentration.h"
#include "RlDppNumTablesStatistic.h"


/** Initialize global workspace */
void RevLanguage::Workspace::initializeGlobalWorkspace(void) {
    
    try {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */
        
        /* Add base types (in folder "datatypes") */


        /* Add primitive types (in folder "datatypes/basic") (alphabetic order) */
        addType( new Complex()                  );
        addType( new Integer()                  );
        addType( new Natural()                  );
        addType( new Probability()              );
        addType( new Real()                     );
        addType( new RealPos()                  );
        addType( new RlBoolean()                );
        addType( new RlString()                 );
        
        /* Add container types (in folder "datatypes/container") (alphabetic order) */
        addType( new Vector<Integer>()          );
        addType( new Vector<Natural>()          );
        addType( new Vector<Real>()             );
        addType( new Vector<RealPos>()          );
        addType( new Vector<RlBoolean>()        );
        addType( new Vector<RlString>()         );
        

        /* Add evolution types (in folder "datatypes/evolution") (alphabetic order) */

        
        /* Add character types (in folder "datatypes/evolution/character") (alphabetic order) */

        
        /* Add tree types (in folder "datatypes/evolution/trees") (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );


        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "mcmc",                   new Mcmc()  );
        addTypeWithConstructor( "model",                  new Model() );
        addTypeWithConstructor( "pmcmcmc",                new ParallelMcmcmc() );
        addTypeWithConstructor( "pathSampler",            new PathSampler() );
        addTypeWithConstructor( "powerPosterior",         new PowerPosterior()  );
        addTypeWithConstructor( "steppingStoneSampler",   new SteppingStoneSampler() );


        ////////////////////////////////////////////////////////////////////////////////
        /* Add monitors (in folder "datatypes/inference/monitors") (alphabetic order) */
        ////////////////////////////////////////////////////////////////////////////////

        addTypeWithConstructor("mnExtNewick", new ExtendedNewickFileMonitor());
        addTypeWithConstructor("mnFile",      new FileMonitor());
        addTypeWithConstructor("mnModel",     new ModelMonitor());
        addTypeWithConstructor("mnScreen",    new ScreenMonitor());

        // Nonstandard constructor names (for backward compatibility)
        addTypeWithConstructor("extNewickmonitor", new ExtendedNewickFileMonitor());
        addTypeWithConstructor("filemonitor",      new FileMonitor());
        addTypeWithConstructor("modelmonitor",     new ModelMonitor());
        addTypeWithConstructor("screenmonitor",    new ScreenMonitor());
        
        
        ///////////////////////////////////////////////////////////////////////////////////
        /* Add moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
        ///////////////////////////////////////////////////////////////////////////////////
        
        /* Moves on real values */
        addTypeWithConstructor("mvScale",               new ScaleMove() );
        addTypeWithConstructor("mvSlide",               new SlidingMove() );
        
        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mScale",                new ScaleMove() );
        addTypeWithConstructor("mSlide",                new SlidingMove() );
        

        /* Moves on integer values */
        addTypeWithConstructor("mvRandomIntegerWalk",   new RandomIntegerWalkMove() );
        addTypeWithConstructor("mvRandomGeometricWalk", new RandomGeometricWalkMove() );

        addTypeWithConstructor("mRandomIntegerWalk",    new RandomIntegerWalkMove() );
        addTypeWithConstructor("mRandomGeometricWalk",  new RandomGeometricWalkMove() );


        /* Moves on simplices */
        addTypeWithConstructor("mvSimplex",             new SimplexMove() );
        addTypeWithConstructor("mvSimplexElementScale", new SimplexSingleElementScale() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mSimplex",              new SimplexMove() );
        addTypeWithConstructor("mSimplexElementScale",  new SimplexSingleElementScale() );
        

        /* Moves on vectors of real values */
        addTypeWithConstructor("mvRlcRateScale",                new RlcRateScaleMove() );
        addTypeWithConstructor("mvSingleElementScale",          new SingleElementScale() );
        addTypeWithConstructor("mvSwitchRateJump",              new SwitchRateJumpMove() );
        addTypeWithConstructor("mvVectorScale",                 new VectorScale() );
        addTypeWithConstructor("mvVectorSingleElementScale",    new VectorSingleElementScaleMove() );
        addTypeWithConstructor("mvVectorSingleElementSliding",  new VectorSingleElementSlidingMove() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mRlcRateScale",                 new RlcRateScaleMove() );
        addTypeWithConstructor("mSingleElementScale",           new SingleElementScale() );
        addTypeWithConstructor("mSwitchRateJump",               new SwitchRateJumpMove() );
        addTypeWithConstructor("mVectorScale",                  new VectorScale() );
        addTypeWithConstructor("mVectorSingleElementScale",     new VectorSingleElementScaleMove() );
        addTypeWithConstructor("mVectorSingleElementSliding",   new VectorSingleElementSlidingMove() );

        
        /* Add tree proposals (in folder "datatypes/inference/moves/tree") */
        addTypeWithConstructor("mvFNPR",                    new FixedNodeheightPruneRegraft() );
        addTypeWithConstructor("mvNarrow",                  new NarrowExchange() );
        addTypeWithConstructor("mvNNI",                     new NearestNeighborInterchange() );
        addTypeWithConstructor("mvNNI",                     new NearestNeighborInterchange_nonClock() );
        addTypeWithConstructor("mvNodeTimeSlideUniform",    new NodeTimeSlideUniform() );
        addTypeWithConstructor("mvRateAgeBetaShift",        new RateAgeBetaShift() );
        addTypeWithConstructor("mvRootTimeSlide",           new RootTimeSlide() );
        addTypeWithConstructor("mvSubtreeScale",            new SubtreeScale() );
        addTypeWithConstructor("mvTreeScale",               new TreeScale() );

        // nonstandard forms (for backward compatibility)
        addTypeWithConstructor("mFNPR",                 new FixedNodeheightPruneRegraft() );
        addTypeWithConstructor("mNarrow",               new NarrowExchange() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange_nonClock() );
        addTypeWithConstructor("mNodeTimeSlideUniform", new NodeTimeSlideUniform() );
        addTypeWithConstructor("mRateAgeBetaShift",     new RateAgeBetaShift() );
        addTypeWithConstructor("mRootTimeSlide",        new RootTimeSlide() );
        addTypeWithConstructor("mSubtreeScale",         new SubtreeScale() );
        addTypeWithConstructor("mTreeScale",            new TreeScale() );
        
        
        /* Add math types (in folder "datatypes/math") */
        addType( new RateMatrix()           );
        addType( new RealMatrix()           );
        addType( new Simplex()              );
        

        ///////////////////////////////////////////////////
        /* Add distributions (in folder "distributions") */
        ///////////////////////////////////////////////////
        
        /* Evolutionary processes (in folder "distributions/evolution") */

        /* Branch rate processes (in folder "distributions/evolution/branchrate") */
        
        // branch-rate jump process
        addDistribution( "dnBranchRateJumpProcess", new BranchRateJumpProcess() );
        addDistribution( "branchRateJumpProcess",   new BranchRateJumpProcess() );
        
        // brownian motion
        addDistribution( "dnBrownian",  new BrownianPhyloProcess() );
        addDistribution( "brownian",    new BrownianPhyloProcess() );
        
        // white noise process
        addDistribution( "dnWhiteNoise",    new WhiteNoisePhyloProcess() );
        addDistribution( "whiteNoise",      new WhiteNoisePhyloProcess() );
        addDistribution( "whitenoise",      new WhiteNoisePhyloProcess() );
        
        
        /* Character state evolution processes (in folder "distributions/evolution/character") */
        
        // simple phylogenetic CTMC on fixed number of discrete states
        addDistribution( "dnPhyloCTMC", new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "dnPhyloCTMC", new CharacterStateEvolutionAlongTree<BranchLengthTree>() );
        addDistribution( "phyloCTMC",   new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "phyloCTMC",   new CharacterStateEvolutionAlongTree<BranchLengthTree>() );
        addDistribution( "substModel",  new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "substModel",  new CharacterStateEvolutionAlongTree<BranchLengthTree>() );

        
        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
        addDistribution( "dnBDP"                        , new ConstantRateBirthDeathProcess() );
        addDistribution( "bdp"                          , new ConstantRateBirthDeathProcess() );
        addDistribution( "cBDP"                         , new ConstantRateBirthDeathProcess() );
        addDistribution( "BirthDeathConstant"           , new ConstantRateBirthDeathProcess() );
        addDistribution( "BDConst"                      , new ConstantRateBirthDeathProcess() );
        
        // constant rate birth-death process with serially sampled tips
        addDistribution( "dnSerialBDP"                  , new ConstantRateSerialSampledBirthDeathProcess() );
        addDistribution( "serialBDP"                    , new ConstantRateSerialSampledBirthDeathProcess() );
        addDistribution( "BirthDeathConstantSerial"     , new ConstantRateSerialSampledBirthDeathProcess() );
        addDistribution( "BDConstSS"                    , new ConstantRateSerialSampledBirthDeathProcess() );

        // piecewise constant rate birth-death process with serially sampled tips
        addDistribution( "dnSkySerialBDP"               , new PiecewiseConstantSerialSampledBirthDeathProcess() );
        addDistribution( "skySerialBDP"                 , new PiecewiseConstantSerialSampledBirthDeathProcess() );
        addDistribution( "BirthDeathSkySerial"          , new PiecewiseConstantSerialSampledBirthDeathProcess() );
        addDistribution( "BDSkySS"                      , new PiecewiseConstantSerialSampledBirthDeathProcess() );

        // piecewise constant rate fossilized birth-death process with serially sampled fossils
        addDistribution( "dnSkyFossilBDP"               , new PiecewiseConstantFossilizedBirthDeathProcess() );
        addDistribution( "skyFossilBDP"                 , new PiecewiseConstantFossilizedBirthDeathProcess() );
        addDistribution( "FossilizedBirthDeath"         , new PiecewiseConstantFossilizedBirthDeathProcess() );
        addDistribution( "FBD"                          , new PiecewiseConstantFossilizedBirthDeathProcess() );

        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnDivDepYuleProcess"          , new DiversityDependentPureBirthProcess() );
        addDistribution( "dnDivDepPBP"                  , new DiversityDependentPureBirthProcess() );
        addDistribution( "divDepPBP"                    , new DiversityDependentPureBirthProcess() );
        addDistribution( "divDepYuleProcess"            , new DiversityDependentPureBirthProcess() );
        addDistribution( "diversityDependentPureBirthProcess", new DiversityDependentPureBirthProcess() );
        addDistribution( "PureBirthDiversityDependent"  , new DiversityDependentPureBirthProcess() );
        addDistribution( "PBDD"                         , new DiversityDependentPureBirthProcess() );
        
        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnConstPopMultispCoal"        , new MultispeciesCoalescentConstantPopulationProcess() );
        addDistribution( "MultispCoal"                  , new MultispeciesCoalescentConstantPopulationProcess() );
        addDistribution( "MultispeciesCoalescentConstantPopulationProcess", new MultispeciesCoalescentConstantPopulationProcess() );

        // uniform time tree distribution
        addDistribution( "dnUniformTimeTree"            , new UniformTimeTreeDistribution() );
        addDistribution( "uniformTimeTree"              , new UniformTimeTreeDistribution() );
        
        // uniform topology distribution
        addDistribution( "dnUniformTopology"            , new UniformTopologyDistribution() );
        addDistribution( "uniformTopology"              , new UniformTopologyDistribution() );
        
        
        /* Statistical distributions on simple variables (in folder "distributions/math") */
        
        // bernoulli distribution
        addDistribution( "dnBernoulli",     new BernoulliDistribution() );
        addDistribution( "bernoulli",       new BernoulliDistribution() );
        
        // beta distribution
        addDistribution( "dnBeta",          new BetaDistribution() );
        addDistribution( "beta",            new BetaDistribution() );
        
        // bimodal normal distribution
        addDistribution( "dnBnorm",         new BimodalNormalDistribution() );
        addDistribution( "bnorm",           new BimodalNormalDistribution() );
        
        // bimodal lognormal distribution
        addDistribution( "dnBlnorm",        new BimodalLognormalDistribution() );
        addDistribution( "blnorm",          new BimodalLognormalDistribution() );
        
        // dirichlet distribution
        addDistribution( "dnDirichlet",     new DirichletDistribution() );
        addDistribution( "dirichlet",       new DirichletDistribution() );
		
        // gamma distribution
        addDistribution( "dnGamma",         new GammaDistribution() );
        addDistribution( "gamma",           new GammaDistribution() );
        
        // geometric distribution
        addDistribution( "dnGeom",          new GeometricDistribution() );
        addDistribution( "geom",            new GeometricDistribution() );
        
        // poisson distribution
        addDistribution( "dnPoisson",       new PoissonDistribution() );
        addDistribution( "poisson",         new PoissonDistribution() );
        
        // exponential distribution
        addDistribution( "dnExponential",   new ExponentialDistribution() );
        addDistribution( "dnExponential",   new OffsetExponentialDistribution() );
        addDistribution( "exponential",     new ExponentialDistribution() );
        addDistribution( "exponential",     new OffsetExponentialDistribution() );
        
        // lognormal distribution
        addDistribution( "dnLnorm",         new LognormalDistribution() );
        addDistribution( "dnLnorm",         new OffsetLognormalDistribution() );
        addDistribution( "lnorm",           new LognormalDistribution() );
        addDistribution( "lnorm",           new OffsetLognormalDistribution() );
        
        // normal distribution
        addDistribution( "dnNorm",          new NormalDistribution() );
        addDistribution( "norm",            new NormalDistribution() );
        
        // 1/x distribution
        addDistribution( "dnOneOverX",      new OneOverXDistribution() );
        addDistribution( "oneOverX",        new OneOverXDistribution() );
        
        // uniform distribution
        addDistribution( "dnUnif",          new UniformDistribution() );
        addDistribution( "dnUnif",          new PositiveUniformDistribution() );
        addDistribution( "unif",            new UniformDistribution() );
        addDistribution( "unif",            new PositiveUniformDistribution() );
        
        // wishart distribution
        addDistribution( "dnWishart",       new WishartDistribution() );
        addDistribution( "wishart",         new WishartDistribution() );
        
        
        /* Mixture distributions (in folder "distributions/mixture") */
        
        // dirichlet process prior distribution
        addDistribution( "dnDPP",		    new DirichletProcessPriorDistribution<Real>() );
		addDistribution( "dnDPP",			new DirichletProcessPriorDistribution<RealPos>() );
		addDistribution( "dnDPP",			new DirichletProcessPriorDistribution<Natural>() );
		addDistribution( "dnDPP",			new DirichletProcessPriorDistribution<Integer>() );
		addDistribution( "dnDPP",			new DirichletProcessPriorDistribution<Probability>() );
        addDistribution( "dpp",				new DirichletProcessPriorDistribution<Real>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<RealPos>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Natural>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Integer>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Probability>() );
        
        // TAH: these don't seem to work with the moves, probably need to figure this out
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution<Topology>() );
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution<Simplex>() );
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution< Vector<RealPos> >() );
        

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
        addFunction( "help",                     new Func_help()                     );
        addFunction( "license",                  new Func_license()                  );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "range",                    new Func_range()                    );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "seq",                      new Func_seq<Integer>()             );
        addFunction( "seq",                      new Func_seq<Real>()                );
        addFunction( "str",                      new Func_structure()                );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );

        // vector functions
        addFunction( "v",         new Func_rlvector<Monitor>()                  );
        addFunction( "v",         new Func_rlvector<Move>()                     );
        addFunction( "v",         new Func_rlvector<AbstractCharacterData>()    );
        addFunction( "v",         new Func_vector<Natural>()                    );
        addFunction( "v",         new Func_vector<Integer>()                    );
        addFunction( "v",         new Func_vector<Real>()                       );
        addFunction( "v",         new Func_vector<RealPos>()                    );
        addFunction( "v",         new Func_vector<RlBoolean>()                  );
        addFunction( "v",         new Func_vector<Clade>()                      );
        addFunction( "v",         new Func_vector<RlString>()                   );
        addFunction( "v",         new Func_vector<TimeTree>()                   );
        
        
        /* Evolution-related functions (in folder "functions/evolution") */
        addFunction( "clade",                       new ConstructorClade()                 );
        addFunction( "expBranchTree",               new ExponentialBranchTree()            );
        addFunction( "phyloRateMultiplier",         new RateMultiplierPhyloFunction()      );
        addFunction( "tmrca",                       new TmrcaStatistic()                   );
        addFunction( "treeAssembly",                new TreeAssemblyFunction()             );
        addFunction( "treeHeight",                  new TreeHeightStatistic()              );
        
        // nonstandard names (for backward compatibility)
        addFunction( "expbranchtree",               new ExponentialBranchTree()            );
        addFunction( "rateMultiplierPhyloFunction", new RateMultiplierPhyloFunction()      );

        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( "blosum62", new Blosum62RateMatrixFunction() );
        addFunction( "cpRev",    new CpRevRateMatrixFunction()    );
        addFunction( "dayhoff",  new DayhoffRateMatrixFunction()  );
        addFunction( "f81",      new F81RateMatrixFunction() );
        addFunction( "gtr",      new Func_gtr() );
        addFunction( "hky",      new HkyRateMatrixFunction() );
        addFunction( "jc",       new JcRateMatrixFunction() );
        addFunction( "jones",    new JonesRateMatrixFunction()    );
        addFunction( "mtMam",    new MtMamRateMatrixFunction()    );
        addFunction( "mtRev",    new MtRevRateMatrixFunction()    );
        addFunction( "rtRev",    new RtRevRateMatrixFunction()    );
        addFunction( "vt",       new VtRateMatrixFunction()       );
        addFunction( "wag",      new WagRateMatrixFunction()      );
        
        // nonstandard names (for backwards compatibility)
        addFunction( "Blosum62", new Blosum62RateMatrixFunction() );
        addFunction( "CpRev",    new CpRevRateMatrixFunction()    );
        addFunction( "Dayhoff",  new DayhoffRateMatrixFunction()  );
        addFunction( "F81",      new F81RateMatrixFunction() );
        addFunction( "HKY",      new HkyRateMatrixFunction() );
        addFunction( "JC",       new JcRateMatrixFunction() );
        addFunction( "Jones",    new JonesRateMatrixFunction()    );
        addFunction( "MtMam",    new MtMamRateMatrixFunction()    );
        addFunction( "MtRev",    new MtRevRateMatrixFunction()    );
        addFunction( "RtRev",    new RtRevRateMatrixFunction()    );
        addFunction( "VT",       new VtRateMatrixFunction()       );
        addFunction( "WAG",      new WagRateMatrixFunction()      );


        /* Inference functions (in folder "functions/inference") */

        /* Convergence functions (in folder "functions/inference/convergence") */
        addFunction( "beca",           new BurninEstimationConvergenceAssessmentFunction() );
        addFunction( "estimateBurnin", new OptimalBurninFunction() );

        
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
        addFunction( "_add",      new Func__add< Natural         , Natural           , Natural           >(  )  );
        addFunction( "_add",      new Func__add< Integer         , Integer           , Integer           >(  )  );
        addFunction( "_add",      new Func__add< Real            , Real              , Real              >(  )  );
        addFunction( "_add",      new Func__add< RealPos         , RealPos           , RealPos           >(  )  );
        addFunction( "_add",      new Func__add< RlString        , RlString          , RlString          >(  )  );
        addFunction( "_add",      new Func__add< RlString        , Real              , RlString          >(  )  );
        addFunction( "_add",      new Func__add< RlString        , Integer           , RlString          >(  )  );
        addFunction( "_add",      new Func__add< Vector<Natural> , Vector<Natural>   , Vector<Natural>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_add",      new Func__add< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  )  );
        
        // division
        addFunction( "_div",      new Func__div< Natural         , Natural           , RealPos           >(  )  );
        addFunction( "_div",      new Func__div< Integer         , Integer           , Real              >(  )  );
        addFunction( "_div",      new Func__div< Real            , Real              , Real              >(  )  );
        addFunction( "_div",      new Func__div< RealPos         , RealPos           , RealPos           >(  )  );
        addFunction( "_div",      new Func__div< Vector<Natural> , Vector<Natural>   , Vector<RealPos>   >(  )  );
        addFunction( "_div",      new Func__div< Vector<Integer> , Vector<Integer>   , Vector<Real>      >(  )  );
        addFunction( "_div",      new Func__div< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_div",      new Func__div< Vector<Real>    , Vector<Real>      , Vector<RealPos>   >(  )  );
        
        // multiplication
        addFunction( "_mul",      new Func__mult< Natural            , Natural           , Natural           >(  )  );
        addFunction( "_mul",      new Func__mult< Integer            , Integer           , Integer           >(  )  );
        addFunction( "_mul",      new Func__mult< Real               , Real              , Real              >(  )  );
        addFunction( "_mul",      new Func__mult< RealPos            , RealPos           , RealPos           >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Natural>    , Vector<Natural>   , Vector<Natural>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Integer>    , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<RealPos>    , Vector<RealPos>   , Vector<RealPos>   >(  )  );
        addFunction( "_mul",      new Func__mult< Vector<Real>       , Vector<Real>      , Vector<Real>      >(  )  );
        
        // subtraction
        addFunction( "_sub",      new Func__sub< Integer         , Integer           , Integer           >(  )  );
        addFunction( "_sub",      new Func__sub< Real            , Real              , Real              >(  )  );
        addFunction( "_sub",      new Func__sub< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  )  );
        addFunction( "_sub",      new Func__sub< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  )  );
        
        // exponentiation
        addFunction( "_exp",      new Func_power() );
        

        /* Input/output functions (in folder "functions/io") */
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
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
        
        // normalize vector function
		addFunction( "normalize", new Func_normalizeVector()  );

		// power function
        addFunction( "power",     new Func_power() );

        // powermix function (TODO: remove when user functions work)
        addFunction( "powermix",  new Func_powermix() );
        
		// round function
        addFunction( "round",     new Func_round<Real,Integer>()  );
        addFunction( "round",     new Func_round<RealPos,Natural>()  );
		
        // simplex constructor function
        addFunction( "simplex",   new Func_simplex()                  );

		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );


 		/* Statistics functions (in folder "functions/statistics") */
        addFunction("dppConcFromMean",  new DPPConcFromPriorMean( )     );
        addFunction("dppMeanFromConc",  new DPPNumFromConcentration( )  );

        // nonstandard forms form backward compatibility
        addFunction("dppCPFromNum",     new DPPConcFromPriorMean( )     );
        addFunction("dppNumFromCP",     new DPPNumFromConcentration( )  );

        
        ///////////////////////////////////////////////////////////////////////////
        /* Add distribution functions (using help classes in folder "functions") */
        ///////////////////////////////////////////////////////////////////////////
        
        // bernoulli distribution
        addFunction("dbernoulli", new DistributionFunctionPdf<Natural>( new BernoulliDistribution() ) );
        addFunction("rbernoulli", new DistributionFunctionRv<Natural>( new BernoulliDistribution() ) );
        
        // beta distribution
        addFunction("dbeta", new DistributionFunctionPdf<Probability>( new BetaDistribution() ) );
        //        addFunction("pbeta", new DistributionFunctionCdf( new BetaDistribution() ) );
        //        addFunction("qbeta", new DistributionFunctionQuantile( new BetaDistribution() ) );
        addFunction("rbeta", new DistributionFunctionRv<Probability>( new BetaDistribution() ) );
        
        // exponential distribution
        addFunction("dexponential", new DistributionFunctionPdf<RealPos>( new ExponentialDistribution() ) );
        addFunction("pexponential", new DistributionFunctionCdf( new ExponentialDistribution() ) );
        addFunction("qexponential", new DistributionFunctionQuantile( new ExponentialDistribution() ) );
        addFunction("rexponential", new DistributionFunctionRv<RealPos>( new ExponentialDistribution() ) );
        
        // gamma distribution
        addFunction("dgamma", new DistributionFunctionPdf<RealPos>( new GammaDistribution() ) );
        addFunction("pgamma", new DistributionFunctionCdf( new GammaDistribution() ) );
        addFunction("qgamma", new DistributionFunctionQuantile( new GammaDistribution() ) );
        addFunction("rgamma", new DistributionFunctionRv<RealPos>( new GammaDistribution() ) );
        
        // lognormal distribution
        addFunction("dlnorm", new DistributionFunctionPdf<RealPos>( new LognormalDistribution() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new LognormalDistribution() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new LognormalDistribution() ) );
        addFunction("rlnorm", new DistributionFunctionRv<RealPos>( new LognormalDistribution() ) );
        addFunction("dlnorm", new DistributionFunctionPdf<Real>( new OffsetLognormalDistribution() ) );
        addFunction("plnorm", new DistributionFunctionCdf( new OffsetLognormalDistribution() ) );
        addFunction("qlnorm", new DistributionFunctionQuantile( new OffsetLognormalDistribution() ) );
        addFunction("rlnorm", new DistributionFunctionRv<Real>( new OffsetLognormalDistribution() ) );
        
        // normal distribution
        addFunction("dnorm", new DistributionFunctionPdf<Real>( new NormalDistribution() ) );
        addFunction("pnorm", new DistributionFunctionCdf( new NormalDistribution() ) );
        addFunction("qnorm", new DistributionFunctionQuantile( new NormalDistribution() ) );
        addFunction("rnorm", new DistributionFunctionRv<Real>( new NormalDistribution() ) );
        
        // uniform distribution
        addFunction("dunif", new DistributionFunctionPdf<Real>( new UniformDistribution() ) );
        addFunction("punif", new DistributionFunctionCdf( new UniformDistribution() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new UniformDistribution() ) );
        addFunction("runif", new DistributionFunctionRv<Real>( new UniformDistribution() ) );
        addFunction("dunif", new DistributionFunctionPdf<RealPos>( new PositiveUniformDistribution() ) );
        addFunction("punif", new DistributionFunctionCdf( new PositiveUniformDistribution() ) );
        addFunction("qunif", new DistributionFunctionQuantile( new PositiveUniformDistribution() ) );
        addFunction("runif", new DistributionFunctionRv<RealPos>( new PositiveUniformDistribution() ) );


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


