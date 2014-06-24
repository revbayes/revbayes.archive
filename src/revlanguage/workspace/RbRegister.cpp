/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * (c) Copyright 2009-
 * @license GPL version 3
 *
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "RlUserInterface.h"
#include "Workspace.h"

/* Primitive types (in folder "datatypes/basic") (alphabetic order) */
#include "RlBoolean.h"
#include "Complex.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

/* Container types (in folder "datatypes/container") (alphabetic order) */
#include "RealMatrix.h"
#include "RealSymmetricMatrix.h"
#include "Simplex.h"
#include "Vector.h"

/* Evolution types (in folder "datatypes/evolution") (alphabetic order) */

/* Character types (in folder "datatypes/evolution/character") (alphabetic order) */
#include "RlAminoAcidState.h"
#include "RlClade.h"
#include "RlDnaState.h"
#include "RlRnaState.h"

/* Inference types (in folder "datatypes/inference") */
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlParallelMcmcmc.h"
#include "RlPowerPosterior.h"

/* Monitors (in folder "datatypes/inference/monitors) */
#include "RlMonitor.h"
#include "RlFileMonitor.h"
#include "RlExtendedNewickFileMonitor.h"
#include "RlModelMonitor.h"
#include "RlScreenMonitor.h"

/* Moves (in folder "datatypes/inference/moves") (grouped by parameter type) */
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
#include "RateMatrix.h"

/// Distributions ///

/* Distributions (in folder "distributions/math") (alphabetic order) */
#include "RlBetaDistribution.h"
#include "RlBernoulliDistribution.h"
#include "RlBimodalLognormalDistribution.h"
#include "RlBimodalNormalDistribution.h"
#include "RlBranchRateJumpProcess.h"
#include "RlBrownianPhyloProcess.h"
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
#include "RlWhiteNoisePhyloProcess.h"
#include "RlWishartDistribution.h"

/* Tree priors (in folder "distributions/evolution/tree") */
#include "RlConstantRateBirthDeathProcess.h"
#include "RlConstantRateSerialSampledBirthDeathProcess.h"
#include "RlPiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RlPiecewiseConstantFossilizedBirthDeathProcess.h"
#include "RlDiversityDependentPureBirthProcess.h"
#include "RlMultispeciesCoalescentConstantPopulationProcess.h"
#include "RlPiecewiseConstantSerialSampledBirthDeathProcess.h"
#include "RlUniformTimeTreeDistribution.h"

/* Character evolution models (in folder "distributions/evolution/character") */
#include "RlCharacterStateEvolutionAlongTree.h"

/* Mixture distributions (in folder "distributions/mixture") */
#include "RlDirichletProcessPriorDistribution.h"

/// Parser functions ///

/* Basic internal functions (alphabetic order) */
#include "Func_range.h"

/* Basic arithmetic/logic templated functions */
#include "Func__and.h"
#include "Func__eq.h"
#include "Func__ge.h"
#include "Func__gt.h"
#include "Func__le.h"
#include "Func__lt.h"
#include "Func__ne.h"
#include "Func__or.h"
#include "Func__unot.h"

/* Built-in functions */
#include "Func_citation.h"
#include "Func_clear.h"
#include "Func_contributors.h"
#include "Func_help.h"
#include "Func_license.h"
#include "Func_ls.h"
#include "Func_normalizeVector.h"
#include "Func_quit.h"
#include "Func_seed.h"
#include "Func_seq.h"
#include "Func_simplex.h"
#include "Func_type.h"
#include "Func_source.h"
#include "Func_structure.h"
#include "Func_write.h"

/* Builtin templated functions */
#include "Func_vector.h"
#include "Func_rlvector.h"

/* Constructor functions */
#include "ConstructorClade.h"

/* Phylogeny functions */
#include "Func_mapTree.h"
#include "Func_readCharacterData.h"
#include "Func_readTrace.h"
#include "Func_readTrees.h"
#include "Func_readTreeTrace.h"
#include "Func_writeFasta.h"
#include "Func_writeNexus.h"
#include "RlTmrcaStatistic.h"
#include "RlTreeHeightStatistic.h"
#include "RlTreeAssemblyFunction.h"

#include "RlF81RateMatrixFunction.h"
#include "Func_gtr.h"
#include "RlHkyRateMatrixFunction.h"
#include "RlJcRateMatrixFunction.h"

#include "RlJonesRateMatrixFunction.h"
#include "RlDayhoffRateMatrixFunction.h"
#include "RlMtRevRateMatrixFunction.h"
#include "RlMtMamRateMatrixFunction.h"
#include "RlWagRateMatrixFunction.h"
#include "RlRtRevRateMatrixFunction.h"
#include "RlCpRevRateMatrixFunction.h"
#include "RlVtRateMatrixFunction.h"
#include "RlBlosum62RateMatrixFunction.h"

#include "RlRateMultiplierPhyloFunction.h"



/* Inference functions */
#include "OptimalBurninFunction.h"
#include "BurninEstimationConvergenceAssessmentFunction.h"


/* Distribution functions */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"

/* Distribution helper function */
#include "RlDPPConcFromPriorMean.h"
#include "RlDPPNumFromConcentration.h"
#include "RlDppNumTablesStatistic.h"

/// Inference Functions ///

/* Basic arithmetic/logic templated functions */
#include "Func_add.h"
#include "Func_div.h"
#include "Func_mult.h"
#include "Func_sub.h"
#include "Func_uminus.h"

/* Math functions */
#include "Func_abs.h"
#include "Func_ceil.h"
#include "Func_exp.h"
#include "Func_floor.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_mean.h"
#include "Func_power.h"
#include "Func_powermix.h"
#include "Func_round.h"
#include "Func_sqrt.h"
#include "Func_trunc.h"


#include <sstream>
#include <vector>
#include <set>
#include <cstdlib>

/** Initialize global workspace */
void RevLanguage::Workspace::initializeGlobalWorkspace(void) {
    
    try {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */
        
        /* Add base types (in folder "datatypes") */


        /* Add primitive types (in folder "datatypes/basic") (alphabetic order) */
        addType( new Complex()                        );
        addType( new Integer()                        );
        addType( new Natural()                        );
        addType( new Probability()                    );
        addType( new Real()                           );
        addType( new RealPos()                        );
        addType( new RlBoolean()                      );
        addType( new RlString()                       );
        
        /* Add container types (in folder "datatypes/container") (alphabetic order) */
        addType( new RealMatrix() );
        addType( new Simplex() );
        addType( new Vector<Integer>()            );
        addType( new Vector<Natural>()            );
        addType( new Vector<Real>()               );
        addType( new Vector<RealPos>()            );
        addType( new Vector<RlBoolean>()          );
        addType( new Vector<RlString>()           );
        
        /* Add evolution types (in folder "datatypes/evolution") (alphabetic order) */

        
        /* Add character types (in folder "datatypes/evolution/character") (alphabetic order) */

        
        /* Add tree types (in folder "datatypes/evolution/trees") (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );


        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "mcmc",             new Mcmc()  );
        addTypeWithConstructor( "model",            new Model() );
        addTypeWithConstructor( "pmcmcmc",          new ParallelMcmcmc() );
        addTypeWithConstructor( "powerPosterior",   new PowerPosterior()  );


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
        addType( new RateMatrix() );
        

        ///////////////////////
        /* Add distributions */
        ///////////////////////
        
        /* Pure statistical distributions (in folder "distributions/math") */
        
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
		
		// dirichlet process prior distribution
        addDistribution( "dpp",				new DirichletProcessPriorDistribution<Real>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<RealPos>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Natural>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Integer>() );
		addDistribution( "dpp",				new DirichletProcessPriorDistribution<Probability>() );
		  // TAH: thes don't semm to work with the moves, probably need to figure this out
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution<Topology>() );
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution<Simplex>() );
		//addDistribution( "dpp",				new DirichletProcessPriorDistribution< Vector<RealPos> >() );
        
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
        
        
        /* Branch rate processes (in folder "distributions/evolution/tree" or "distributions/evolution") */
        
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
        addDistribution( "dnPhyloCTMC", new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "dnPhyloCTMC", new CharacterStateEvolutionAlongTree<BranchLengthTree>() );
        addDistribution( "phyloCTMC",   new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "phyloCTMC",   new CharacterStateEvolutionAlongTree<BranchLengthTree>() );
        addDistribution( "substModel",  new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "substModel",  new CharacterStateEvolutionAlongTree<BranchLengthTree>() );

        /* Tree distributions (in folder "distributions/evolution/tree") */
        
        // constant rate birth-death process
        addDistribution( "dnBDP"                        , new ConstantRateBirthDeathProcess() );
        addDistribution( "cBDP"                         , new ConstantRateBirthDeathProcess() );
        addDistribution( "BirthDeathConstant"           , new ConstantRateBirthDeathProcess() );
        addDistribution( "BDConst"                      , new ConstantRateBirthDeathProcess() );
        
        // constant rate birth-death process with serially sampled tips
        addDistribution( "dnSerialBDP"                  , new ConstantRateSerialSampledBirthDeathProcess() );
        addDistribution( "BirthDeathConstantSerial"     , new ConstantRateSerialSampledBirthDeathProcess() );
        addDistribution( "BDConstSS"                    , new ConstantRateSerialSampledBirthDeathProcess() );

        // piecewise constant rate birth-death process with serially sampled tips
        addDistribution( "dnSkySerialBDP"               , new PiecewiseConstantSerialSampledBirthDeathProcess() );
        addDistribution( "BirthDeathSkySerial"          , new PiecewiseConstantSerialSampledBirthDeathProcess() );
        addDistribution( "BDSkySS"                      , new PiecewiseConstantSerialSampledBirthDeathProcess() );

        // piecewise constant rate fossilized birth-death process with serially sampled fossils
        addDistribution( "dnSkyFossilBDP"               , new PiecewiseConstantFossilizedBirthDeathProcess() );
        addDistribution( "FossilizedBirthDeath"         , new PiecewiseConstantFossilizedBirthDeathProcess() );
        addDistribution( "FBD"                          , new PiecewiseConstantFossilizedBirthDeathProcess() );

        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "dnDivDepPBP"                  , new DiversityDependentPureBirthProcess() );
        addDistribution( "divDepPBP"                    , new DiversityDependentPureBirthProcess() );
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
        
        
        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;
        
        //////////////////////////////////////////////////
        /* Add parser functions (in "functions" folder) */
        //////////////////////////////////////////////////
        
        /* Add basic functions (in "functions/basic" directory) (alphabetical order) */
        addFunction( "citation",                 new Func_citation()                 );
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "contributors",             new Func_contributors()             );
        addFunction( "help",                     new Func_help()                     );
        addFunction( "license",                  new Func_license()                  );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "seq",                      new Func_seq<Integer>()             );
        addFunction( "simplex",                  new Func_simplex()                  );
        addFunction( "str",                      new Func_structure()                );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );
        
        // @TODO: To be moved to other directories
        // normalizeVector  to math
        // rbVector
        // rlVector
        // source   to io
        // vector

        
        /* Add builtin functions (in folder "functions/builtin") */
        
        // Note: Builtin functions are usually called implicitly. An explicit function
        // name starting with an underscore is also provided to allow explicit calls.
        // The exception is "range", which does not have an underscore to be compatible
        // with the R range function

        // not templated
        addFunction( "_and",      new Func__and()   );
        addFunction( "_or",       new Func__or()    );
        addFunction( "_unot",     new Func__unot()  );
        addFunction( "range",     new Func_range()  );  // note analogy with R "range" function, therefore not "_range"

        // templated
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
        
        /* Add evolution functions (in folder "functions/evolution") */

        // rate matrix functions
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
        
        // rate matrix functions, nonstandard names (for backwards compatibility)
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
        
        // other evolution functions
        addFunction( "expBranchTree",               new ExponentialBranchTree()            );
        addFunction( "phyloRateMultiplier",         new RateMultiplierPhyloFunction()      );
        addFunction( "tmrca",                       new TmrcaStatistic()                   );
        addFunction( "treeAssembly",                new TreeAssemblyFunction()             );
        addFunction( "treeHeight",                  new TreeHeightStatistic()              );

        // other evolution functions, nonstandard names (for backward compatibility)
        addFunction( "expbranchtree",               new ExponentialBranchTree()            );
        addFunction( "rateMultiplierPhyloFunction", new RateMultiplierPhyloFunction()      );


        /* Add inference functions (in folder "functions/inference/convergence") */
        addFunction( "beca",           new BurninEstimationConvergenceAssessmentFunction() );
        addFunction( "estimateBurnin", new OptimalBurninFunction() );

        
        /////////////////////////////////////
        // Add distribution functions here //
        /////////////////////////////////////

        /* Add io functions (in folder "functions/io") */
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
        addFunction( "readTrace",                   new Func_readTrace()                   );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()               );
        addFunction( "source",                      new Func_source()                      );   // Move here!!
        addFunction( "write",                       new Func_write()                       );
        addFunction( "writeFasta",                  new Func_writeFasta()                  );
        addFunction( "writeNexus",                  new Func_writeNexus()                  );

        /* Add math functions (in folder "functions/math") */

        // templated builtin math functions @TODO: move to builtin

        // unary minus (e.g. -a)
        addFunction( "_uminus",   new Func_uminus<Integer, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Natural, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Real, Real>() );
        addFunction( "_uminus",   new Func_uminus<RealPos, Real>() );

        // addition (e.g. a+b )
        addFunction( "_add",      new Func_add< Natural         , Natural           , Natural           >(  ) );
        addFunction( "_add",      new Func_add< Integer         , Integer           , Integer           >(  ) );
        addFunction( "_add",      new Func_add< Real            , Real              , Real              >(  ) );
        addFunction( "_add",      new Func_add< RealPos         , RealPos           , RealPos           >(  ) );
        addFunction( "_add",      new Func_add< RlString        , RlString          , RlString          >(  ) );
        addFunction( "_add",      new Func_add< RlString        , Real              , RlString          >(  ) );
        addFunction( "_add",      new Func_add< RlString        , Integer           , RlString          >(  ) );
        addFunction( "_add",      new Func_add< Vector<Natural> , Vector<Natural>   , Vector<Natural>   >(  ) );
        addFunction( "_add",      new Func_add< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  ) );
        addFunction( "_add",      new Func_add< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  ) );
        addFunction( "_add",      new Func_add< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  ) );
        
        // division
        addFunction( "_div",      new Func_div< Natural         , Natural           , RealPos           >(  ) );
        addFunction( "_div",      new Func_div< Integer         , Integer           , Real              >(  ) );
        addFunction( "_div",      new Func_div< Real            , Real              , Real              >(  ) );
        addFunction( "_div",      new Func_div< RealPos         , RealPos           , RealPos           >(  ) );
        addFunction( "_div",      new Func_div< Vector<Natural> , Vector<Natural>   , Vector<RealPos>   >(  ) );
        addFunction( "_div",      new Func_div< Vector<Integer> , Vector<Integer>   , Vector<Real>      >(  ) );
        addFunction( "_div",      new Func_div< Vector<RealPos> , Vector<RealPos>   , Vector<RealPos>   >(  ) );
        addFunction( "_div",      new Func_div< Vector<Real>    , Vector<Real>      , Vector<RealPos>   >(  ) );
        
        // multiplication
        addFunction( "_mul",      new Func_mult< Natural            , Natural           , Natural           >(  ) );
        addFunction( "_mul",      new Func_mult< Integer            , Integer           , Integer           >(  ) );
        addFunction( "_mul",      new Func_mult< Real               , Real              , Real              >(  ) );
        addFunction( "_mul",      new Func_mult< RealPos            , RealPos           , RealPos           >(  ) );
        addFunction( "_mul",      new Func_mult< Vector<Natural>    , Vector<Natural>   , Vector<Natural>   >(  ) );
        addFunction( "_mul",      new Func_mult< Vector<Integer>    , Vector<Integer>   , Vector<Integer>   >(  ) );
        addFunction( "_mul",      new Func_mult< Vector<RealPos>    , Vector<RealPos>   , Vector<RealPos>   >(  ) );
        addFunction( "_mul",      new Func_mult< Vector<Real>       , Vector<Real>      , Vector<Real>      >(  ) );
        
        // subtraction
        addFunction( "_sub",      new Func_sub< Integer         , Integer           , Integer           >(  ) );
        addFunction( "_sub",      new Func_sub< Real            , Real              , Real              >(  ) );
        addFunction( "_sub",      new Func_sub< Vector<Integer> , Vector<Integer>   , Vector<Integer>   >(  ) );
        addFunction( "_sub",      new Func_sub< Vector<Real>    , Vector<Real>      , Vector<Real>      >(  ) );
        
        // nontemplated builtin functions (move to builtin!)
        
        // exponentiation
        addFunction( "_exp",      new Func_power() );
        
        // proper math functions
		
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
		addFunction( "normalize",   new Func_normalizeVector()  );

		// power function
        addFunction( "power",     new Func_power() );

        // powermix function (TODO: remove when user functions work)
        addFunction( "powermix",  new Func_powermix() );
        
		// round function
        addFunction( "round",     new Func_round<Real,Integer>()  );
        addFunction( "round",     new Func_round<RealPos,Natural>()  );
		
		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );

        
        /* Add constructor functions (in folder "functions/constructor") (alphabetical order) */
        addFunction( "clade",                       new ConstructorClade()                 );   // TODO! mMove to evolution
        
        /* Add vector functions (in folder "functions/basic" currently) */
        addFunction( "v",         new Func_rlvector<Monitor>() );
        addFunction( "v",         new Func_rlvector<Move>() );
        addFunction( "v",         new Func_rlvector<AbstractCharacterData>() );
        addFunction( "v",         new Func_vector<Natural>() );
        addFunction( "v",         new Func_vector<Integer>() );
        addFunction( "v",         new Func_vector<Real>() );
        addFunction( "v",         new Func_vector<RealPos>() );
        addFunction( "v",         new Func_vector<RlBoolean>() );
        addFunction( "v",         new Func_vector<Clade>() );
        addFunction( "v",         new Func_vector<RlString>() );
        addFunction( "v",         new Func_vector<TimeTree>() );

        
        //////////////////////////////////////////////////////////////////////
        /* Add distribution functions (using classes in folder "functions") */
        //////////////////////////////////////////////////////////////////////
        
        // Bernoulli distribution
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

		// distribution helper function
		addFunction("dppCPFromNum", new DPPConcFromPriorMean( ) );
		addFunction("dppNumFromCP", new DPPNumFromConcentration( ) );
    
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


