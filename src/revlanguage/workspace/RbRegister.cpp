/**
 * @file
 * This file contains the Workspace function that adds types and functions
 * to the global workspace, registering them with the interpreter/compiler
 * during the process.
 *
 * @brief Function registering language objects
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @extends Frame
 * @package parser
 * @version 1.0
 * @since version 1.0 2009-09-02
 *
 * $Id$
 */

#include "ArgumentRule.h"
#include "RbException.h"
#include "RlUserInterface.h"
#include "Workspace.h"

/* Primitive types (alphabetic order) */
#include "RlBoolean.h"
#include "Complex.h"
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

#include "RlAminoAcidState.h"
#include "RlClade.h"
#include "RlDnaState.h"
#include "RlRnaState.h"

/* Container types (alphabetic order) */
#include "Vector.h"


/* MemberObject types with auto-generated constructors (alphabetic order) */
#include "RlMcmc.h"
#include "RlModel.h"
#include "RlPowerPosterior.h"

/* Distributions with distribution constructors and distribution functions (alphabetic order) */
#include "RlBetaDistribution.h"
#include "RlBernoulliDistribution.h"
#include "RlDirichletDistribution.h"
#include "RlExponentialDistribution.h"
#include "RlGammaDistribution.h"
#include "RlGeometricDistribution.h"
#include "RlLognormalDistribution.h"
#include "RlNormalDistribution.h"
#include "RlOffsetExponentialDistribution.h"
#include "RlOffsetLognormalDistribution.h"
#include "RlOneOverXDistribution.h"
#include "RlPositiveUniformDistribution.h"
#include "RlUniformDistribution.h"
#include "RlUniformTopologyDistribution.h"

// tree priors
#include "RlConstantRateBirthDeathProcess.h"
#include "RlDiversityDependentPureBirthProcess.h"
#include "RlUniformTimeTreeDistribution.h"

// sequence models
#include "RlCharacterStateEvolutionAlongTree.h"

/* Moves */
#include "RlMove.h"

/* Moves on real values*/
#include "RlScaleMove.h"
#include "RlSlidingMove.h"

/* Moves on integer values*/
#include "RlRandomIntegerWalkMove.h"
#include "RlRandomGeometricWalkMove.h"

/* Moves on Simplices */
#include "RlSimplexMove.h"
#include "RlSimplexSingleElementScale.h"

/* Moves on real valued vectors */
#include "RlSingleElementScale.h"
#include "RlVectorScale.h"

/* Tree Proposals */
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


/* Monitors */
#include "RlMonitor.h"
#include "RlFileMonitor.h"
#include "RlExtendedNewickFileMonitor.h"
#include "RlModelMonitor.h"
#include "RlScreenMonitor.h"

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
#include "Func_clear.h"
#include "Func_ls.h"
#include "Func_normalizeVector.h"
#include "Func_quit.h"
#include "Func_seed.h"
#include "Func_simplex.h"
#include "Func_type.h"
#include "Func_Source.h"
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



/* Inference functions */
#include "OptimalBurninFunction.h"
#include "BurninEstimationConvergenceAssessmentFunction.h"


/* Distribution functions */
#include "DistributionFunctionCdf.h"
#include "DistributionFunctionPdf.h"
#include "DistributionFunctionQuantile.h"
#include "DistributionFunctionRv.h"


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
        
        /* Add primitive types (alphabetic order) */
        addType( new RlBoolean()                      );
        addType( new Complex()                        );
        addType( new Integer()                        );
        addType( new Natural()                        );
        addType( new Probability()                    );
        addType( new RlString()                       );
        addType( new Real()                           );
        addType( new RealPos()                        );
        
        /* Add container types (alphabetic order) */
        addType( new Vector<RlBoolean>()          );
        addType( new Vector<Integer>()            );
        addType( new Vector<Natural>()            );
        addType( new Vector<Real>()               );
        addType( new Vector<RealPos>()            );
        addType( new Vector<RlString>()           );
        
        /* Add MemberObject types with auto-generated constructors (alphabetic order) */
        addTypeWithConstructor( "clade",            new Clade() );
        addTypeWithConstructor( "mcmc",             new Mcmc()  );
        addTypeWithConstructor( "model",            new Model() );
        addTypeWithConstructor( "powerPosterior",   new PowerPosterior()  );
        
        
        //////////////////
        /* Add monitors */
        //////////////////
        
        /* File monitor */
        addTypeWithConstructor("extNewickmonitor",    new ExtendedNewickFileMonitor());
        
        /* File monitor */
        addTypeWithConstructor("filemonitor",    new FileMonitor());
        
        /* File monitor */
        addTypeWithConstructor("modelmonitor",    new ModelMonitor());
        
        /* Screen monitor */
        addTypeWithConstructor("screenmonitor",    new ScreenMonitor());
        
        
        
        ///////////////
        /* Add moves */
        ///////////////
        
        
        /* Moves on real values */
        addTypeWithConstructor("mScale",                new ScaleMove() );
        addTypeWithConstructor("mSlide",                new SlidingMove() );
        
        /* Moves on integer values */
        addTypeWithConstructor("mRandomIntegerWalk",    new RandomIntegerWalkMove() );
        addTypeWithConstructor("mRandomGeometricWalk",  new RandomGeometricWalkMove() );

        /* Moves on simplices */
        addTypeWithConstructor("mSimplex",              new SimplexMove() );
        addTypeWithConstructor("mSimplexElementScale",  new SimplexSingleElementScale() );
        
        /* Moves on vectors of real values */
        addTypeWithConstructor("mSingleElementScale",   new SingleElementScale() );
        addTypeWithConstructor("mVectorScale",          new VectorScale() );        
        
        /* Tree Proposals */
        addTypeWithConstructor("mFNPR",                 new FixedNodeheightPruneRegraft() );
        addTypeWithConstructor("mNarrow",               new NarrowExchange() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange() );
        addTypeWithConstructor("mNNI",                  new NearestNeighborInterchange_nonClock() );
        addTypeWithConstructor("mNodeTimeSlideUniform", new NodeTimeSlideUniform() );
        addTypeWithConstructor("mRateAgeBetaShift",     new RateAgeBetaShift() );
        addTypeWithConstructor("mRootTimeSlide",        new RootTimeSlide() );
        addTypeWithConstructor("mSubtreeScale",         new SubtreeScale() );
        addTypeWithConstructor("mTreeScale",            new TreeScale() );
        
        
        
        ///////////////////////
        /* Add Distributions */
        ///////////////////////
        
        // Pure statistical distributions
        
        // Bernoulli distribution
        addDistribution( "bernoulli", new BernoulliDistribution() );
        
        
        // beta distribution
        addDistribution( "beta", new BetaDistribution() );
        
        
        // dirichlet distribution
        addDistribution( "dirichlet", new DirichletDistribution() );
        
        
        // gamma distribution
        addDistribution( "gamma", new GammaDistribution() );
        
        
        // geometric distribution
        addDistribution( "geom", new GeometricDistribution() );
        
        
        // exponential distribution
        addDistribution( "exponential", new ExponentialDistribution() );
        addDistribution( "exponential", new OffsetExponentialDistribution() );
        
        // lognormal distribution
        addDistribution( "lnorm", new LognormalDistribution() );
        addDistribution( "lnorm", new OffsetLognormalDistribution() );
        
        // normal distribution
        addDistribution( "norm", new NormalDistribution() );
                
        // 1/x distribution
        addDistribution( "oneOverX", new OneOverXDistribution() );
        
        // uniform distribution
        addDistribution( "unif", new UniformDistribution() );
        addDistribution( "unif", new PositiveUniformDistribution() );
        
        
        
        // Phylogenetic distributions
        
        // constant rate birth-death process distribution
        addDistribution( "cBDP", new ConstantRateBirthDeathProcess() );

        // diversity-dependent pure-birth process (renamed to be somewhat consistent with cBDP)
        addDistribution( "divDepPBP", new DiversityDependentPureBirthProcess() );
        
        // uniform time tree distribution
        addDistribution( "uniformTimeTree", new UniformTimeTreeDistribution() );
        
        // character state evolution model (DNA|RNA|AA)
        addDistribution( "substModel", new CharacterStateEvolutionAlongTree<TimeTree>() );
        addDistribution( "substModel", new CharacterStateEvolutionAlongTree<BranchLengthTree>() );
        
        
        // uniform topology distribution
        addDistribution( "uniformTopology", new UniformTopologyDistribution() );
        
        
        
        
        /* Now we have added all primitive and complex data types and can start type checking */
        Workspace::globalWorkspace().typesInitialized = true;
        Workspace::userWorkspace().typesInitialized   = true;
        
        ///////////////////////////////
        // Add parser functions here //
        ///////////////////////////////
        
        /* Add basic internal functions (alphabetic order) */
        addFunction( "_range",    new Func_range()       );
        
        
        /* Add basic unary arithmetic templated functions */
        
        /* Add basic logical functions */
        addFunction( "_and",      new Func__and()  );
        addFunction( "_unot",     new Func__unot() );
        addFunction( "_or",       new Func__or()   );
        
        /* Add basic logic templated functions */
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
        
        
        /* Add builtin functions (alphabetical order) */
        addFunction( "clear",                    new Func_clear()                    );
        addFunction( "ls",                       new Func_ls()                       );
        addFunction( "q",                        new Func_quit()                     );
        addFunction( "quit",                     new Func_quit()                     );
        addFunction( "seed",                     new Func_seed()                     );
        addFunction( "simplex",                  new Func_simplex()                  );
        addFunction( "structure",                new Func_structure()                );
        addFunction( "type",                     new Func_type()                     );
        
        
        /////////////////////////////////////
        // Add distribution functions here //
        /////////////////////////////////////
        
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
        
        // log-normal distribution
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
        
        
        //////////////////////////////////
        // Add inference functions here //
        //////////////////////////////////
        
        
        /* Add basic unary arithmetic and logical templated functions */
        
        // unary minus ( e.g. -a )
        addFunction( "_uminus",   new Func_uminus<Integer, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Natural, Integer>() );
        addFunction( "_uminus",   new Func_uminus<Real, Real>() );
        addFunction( "_uminus",   new Func_uminus<RealPos, Real>() );
        
        
        
        /* Add basic arithmetic templated functions */
        
        // addition (e.g. a+b )
        addFunction( "_add",      new Func_add<Natural, Natural, Natural>(  ) );
        addFunction( "_add",      new Func_add<Integer, Integer, Integer>(  ) );
        addFunction( "_add",      new Func_add<Real, Real, Real>(  ) );
        addFunction( "_add",      new Func_add<RealPos, RealPos, RealPos>(  ) );
        addFunction( "_add",      new Func_add<RlString, RlString, RlString>(  ) );
        addFunction( "_add",      new Func_add<RlString, Real, RlString>(  ) );
        addFunction( "_add",      new Func_add<RlString, Integer, RlString>(  ) );
        
        
        // division
        addFunction( "_div",      new Func_div<Natural, Natural, RealPos>(  ) );
        addFunction( "_div",      new Func_div<Integer, Integer, Real>(  ) );
        addFunction( "_div",      new Func_div<Real, Real, Real>(  ) );
        addFunction( "_div",      new Func_div<RealPos, RealPos, RealPos>(  ) );
        
        // multiplication
        addFunction( "_mul",      new Func_mult<Natural, Natural, Natural>(  ) );
        addFunction( "_mul",      new Func_mult<Integer, Integer, Integer>(  ) );
        addFunction( "_mul",      new Func_mult<Real, Real, Real>(  ) );
        addFunction( "_mul",      new Func_mult<RealPos, RealPos, RealPos>(  ) );
        
        // subtraction
        addFunction( "_sub",      new Func_sub<Integer, Integer, Integer>(  ) );
        addFunction( "_sub",      new Func_sub<Real, Real, Real>(  ) );
        
        
        addFunction( "_exp",      new Func_power() );
        
        /* Add math functions (alphabetical order) */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
		
		// ceil function
        addFunction( "ceil",        new Func_ceil<Real,Integer>()  );
        addFunction( "ceil",        new Func_ceil<RealPos,Natural>()  );
        addFunction( "ceiling",     new Func_ceil<Real,Integer>()  );
        addFunction( "ceiling",     new Func_ceil<RealPos,Natural>()  );
        
        // cos function
        //        addFunction( "cos",       new Func_cos()  );
		
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
		
		
        addFunction( "power",     new Func_power() );
        
        
        // sin function
        //        addFunction( "sin",       new Func_sin() );
        
		// square root function
        addFunction( "sqrt",      new Func_sqrt()  );
		
		// round function
        addFunction( "round",     new Func_round<Real,Integer>()  );
        addFunction( "round",     new Func_round<RealPos,Natural>()  );
		
		// truncate function
        addFunction( "trunc",     new Func_trunc<Real,Integer>()  );
        addFunction( "trunc",     new Func_trunc<RealPos,Natural>()  );
        
        
        
		// rate matrix functions
        addFunction( "JC",       new JcRateMatrixFunction() );
        addFunction( "F81",      new F81RateMatrixFunction() );
        addFunction( "HKY",      new HkyRateMatrixFunction() );
        addFunction( "gtr",      new Func_gtr() );
        
        // pre-defined rate matrices
        addFunction( "Jones",    new JonesRateMatrixFunction()    );
        addFunction( "Dayhoff",  new DayhoffRateMatrixFunction()  );
        addFunction( "MtRev",    new MtRevRateMatrixFunction()    );
        addFunction( "MtMam",    new MtMamRateMatrixFunction()    );
        addFunction( "WAG",      new WagRateMatrixFunction()      );
        addFunction( "RtRev",    new RtRevRateMatrixFunction()    );
        addFunction( "CpRev",    new CpRevRateMatrixFunction()    );
        addFunction( "VT",       new VtRateMatrixFunction()       );
        addFunction( "Blosum62", new Blosum62RateMatrixFunction() );
		
        
        /* Add phylogeny-related functions (alphabetical order) */
        addFunction( "clade",                       new ConstructorClade()                 );
        addFunction( "readTrace",                   new Func_readTrace()                   );
        addFunction( "mapTree",                     new Func_mapTree<BranchLengthTree>()   );
        addFunction( "mapTree",                     new Func_mapTree<TimeTree>()           );
        addFunction( "readCharacterData",           new Func_readCharacterData()           );
        addFunction( "readTrees",                   new Func_readTrees()                   );
        addFunction( "readTreeTrace",               new Func_readTreeTrace()               );
        addFunction( "writeFasta",                  new Func_writeFasta()                  );
        addFunction( "writeNexus",                  new Func_writeNexus()                  );
        
        addFunction( "tmrca",                       new TmrcaStatistic()                   );
        addFunction( "treeAssembly",                new TreeAssemblyFunction()             );
        addFunction( "treeHeight",                  new TreeHeightStatistic()              );
        
        /* Add builtin templated functions */
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

        /////////////////////////////////////////
        // Add RevLanguage only functions here //
        /////////////////////////////////////////
        
        addFunction( "source",         new Func_Source() );
        addFunction( "write",          new Func_write()  );
        
        // inference function
        addFunction( "beca",           new BurninEstimationConvergenceAssessmentFunction() );
        addFunction( "estimateBurnin", new OptimalBurninFunction() );
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


