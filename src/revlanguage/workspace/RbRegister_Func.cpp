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


/* Taxon types (in folder "datatypes/evolution") */
#include "RlTaxon.h"


/* Math types (in folder "datatypes/math") */
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "RlRateMap.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"


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


/* Functions related to evolution (in folder "functions/phylogenetics") */
#include "Func_branchScoreDistance.h"
#include "Func_clade.h"
#include "Func_concatenate.h"
#include "Func_constructRootedTripletDistribution.h"
#include "Func_maximumTree.h"
#include "Func_mrcaIndex.h"
#include "Func_phyloDiversity.h"
#include "Func_pomoStateConverter.h"
#include "Func_pomoRootFrequencies.h"
#include "Func_simTree.h"
#include "Func_symmetricDifference.h"
#include "Func_tmrca.h"
#include "Func_treeAssembly.h"
#include "Func_treePairwiseDistances.h"
#include "Func_treeScale.h"


/* Rate matrix functions (in folder "functions/phylogenetics/ratematrix") */
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
#include "Func_InfiniteSitesRateMatrix.h"
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

/* Functions related to evolution (in folder "functions/popgen") */
#include "Func_SegregatingSites.h"
#include "Func_TajimasD.h"
#include "Func_TajimasPi.h"
#include "Func_WattersonTheta.h"


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
#include "Func_coala.h"
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
#include "Func_vectorFlatten.h"


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
void RevLanguage::Workspace::initializeFuncGlobalWorkspace(void)
{
    
    try
    {
        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////
        
        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( "fnBlosum62",          new Func_blosum62()                 );
        addFunction( "fnChromosomes",       new Func_chromosomes()              );
        addFunction( "fnCpRev",             new Func_cpRev()                    );
        addFunction( "fnDayhoff",           new Func_dayhoff()                  );
        addFunction( "fnDECRateMatrix",     new Func_DECRateMatrix()            );
        addFunction( "fnEpoch",             new Func_epoch()                    );
        addFunction( "fnF81",               new Func_f81()                      );
        addFunction( "fnFreeBinary",        new Func_FreeBinary()               );
        addFunction( "fnFreeK",             new Func_FreeK()                    );
        addFunction( "fnGTR",               new Func_gtr()                      );
        addFunction( "fnHKY",               new Func_hky()                      );
        addFunction( "fnInfiniteSites",     new Func_InfiniteSitesRateMatrix()  );
        addFunction( "fnJC",                new Func_jc()                       );
        addFunction( "fnJones",             new Func_jones()                    );
        addFunction( "fnK80",               new Func_k80()                      );
        addFunction( "fnMtMam",             new Func_mtMam()                    );
        addFunction( "fnMtRev",             new Func_mtRev()                    );
        addFunction( "fnPomo",              new Func_pomo()                     );
        addFunction( "fnRtRev",             new Func_rtRev()                    );
        addFunction( "fnT92",               new Func_t92()                      );
        addFunction( "fnVT",                new Func_vt()                       );
        addFunction( "fnWAG",               new Func_wag()                      );
        
        /* rate maps used for data augmentation (in folder "functions/evolution/ratemap") */
        addFunction( "fnBiogeoDE",      new Func_biogeo_de() );
        addFunction( "fnBiogeoGRM",     new Func_biogeo_grm() );
        
        /* cladogenic probs used for e.g. DEC models (in folder "functions/phylogenetics") */
        addFunction( "fnCladoProbs",        new Func_cladoProbs() );
        addFunction( "fnDECRates",          new Func_DECRates() );
        addFunction( "fnDECRoot",           new Func_DECRoot() );
        addFunction( "fnPD",                new Func_phyloDiversity<BranchLengthTree>() );
        addFunction( "fnPD",                new Func_phyloDiversity<TimeTree>() );
		
		/* Functions related to phylogenetic trees (in folder "functions/phylogenetics/tree") */
		addFunction( "fnTreePairwiseDistances",        new Func_treePairwiseDistances<BranchLengthTree>() );
		addFunction( "fnTreePairwiseDistances",        new Func_treePairwiseDistances<TimeTree>() );
		
        /* Population genetics functions (in folder "functions/popgen") */
        addFunction( "fnSegregatingSites",  new Func_SegregatingSites() );
        addFunction( "fnTajimasD",          new Func_TajimasD()         );
        addFunction( "fnTajimasPi",         new Func_TajimasPi()        );
        addFunction( "fnWattersonsTheta",   new Func_WattersonTheta()   );

    
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
        addFunction( "_eq",       new Func__eq<             Integer,          Integer >()             );
        addFunction( "_eq",       new Func__eq<                Real,             Real >()             );
        addFunction( "_eq",       new Func__eq<             Integer,             Real >()             );
        addFunction( "_eq",       new Func__eq<                Real,          Integer >()             );
        addFunction( "_eq",       new Func__eq<           RlBoolean,        RlBoolean >()             );
        addFunction( "_eq",       new Func__eq<            RlString,         RlString >()             );
        addFunction( "_eq",       new Func__eq<             Simplex,          Simplex >()             );
        addFunction( "_eq",       new Func__eq<       RateGenerator,    RateGenerator >()             );
        addFunction( "_eq",       new Func__eq<            TimeTree,         TimeTree >()             );
        addFunction( "_eq",       new Func__eq<    BranchLengthTree, BranchLengthTree >()             );
        addFunction( "_ge",       new Func__ge<             Integer,          Integer >()             );
        addFunction( "_ge",       new Func__ge<                Real,             Real >()             );
        addFunction( "_ge",       new Func__ge<             Integer,             Real >()             );
        addFunction( "_ge",       new Func__ge<                Real,          Integer >()             );
        addFunction( "_ge",       new Func__ge<           RlBoolean,        RlBoolean >()             );
        addFunction( "_gt",       new Func__gt<             Integer,          Integer >()             );
        addFunction( "_gt",       new Func__gt<                Real,             Real >()             );
        addFunction( "_gt",       new Func__gt<           RlBoolean,        RlBoolean >()             );
        addFunction( "_le",       new Func__le<             Integer,          Integer >()             );
        addFunction( "_le",       new Func__le<                Real,             Real >()             );
        addFunction( "_le",       new Func__le<             Integer,             Real >()             );
        addFunction( "_le",       new Func__le<                Real,          Integer >()             );
        addFunction( "_le",       new Func__le<           RlBoolean,        RlBoolean >()             );
        addFunction( "_lt",       new Func__lt<             Integer,          Integer >()             );
        addFunction( "_lt",       new Func__lt<                Real,             Real >()             );
        addFunction( "_lt",       new Func__lt<           RlBoolean,        RlBoolean >()             );
        addFunction( "_ne",       new Func__ne<             Integer,          Integer >()             );
        addFunction( "_ne",       new Func__ne<                Real,             Real >()             );
        addFunction( "_ne",       new Func__ne<             Integer,             Real >()             );
        addFunction( "_ne",       new Func__ne<                Real,          Integer >()             );
        addFunction( "_ne",       new Func__ne<           RlBoolean,        RlBoolean >()             );
        addFunction( "_ne",       new Func__ne<       RateGenerator,    RateGenerator >()             );
        addFunction( "_ne",       new Func__ne<             Simplex,          Simplex >()             );
        addFunction( "_ne",       new Func__ne<            TimeTree,         TimeTree >()             );
        addFunction( "_ne",       new Func__ne<    BranchLengthTree, BranchLengthTree >()             );
        
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
        
        /* Math functions (in folder "functions/math") */
		
		// absolute function
        addFunction( "abs",         new Func_abs()  );
		
		// ceil function
        addFunction( "ceil",        new Func_ceil<Real,Integer>()  );
        addFunction( "ceil",        new Func_ceil<RealPos,Natural>()  );
        addFunction( "ceiling",     new Func_ceil<Real,Integer>()  );
        addFunction( "ceiling",     new Func_ceil<RealPos,Natural>()  );
        
        // coala function
        addFunction( "fnCoala",     new Func_coala()        );
        
        // diagonal matrix
        addFunction( "diag",         new Func_diagonalMatrix() );
        
        // exponential function
        addFunction( "exp",         new Func_exp() );
		
		// floor function
        addFunction( "floor",       new Func_floor<Real,Integer>()  );
        addFunction( "floor",       new Func_floor<RealPos,Natural>()  );
        
        // hyperbolic tangent function
        addFunction( "tanh",        new Func_hyperbolicTangent() );
        
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
        
        // conversion function from Real to Probability
        addFunction( "Probability", new Func_probability() );
        
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
        
        // vector flatten
        addFunction( "vectorFlatten",   new Func_vectorFlatten() );
        
        // get ln Probability function
        addFunction( "fnLnProbability", new Func_lnProbability() );

        
        
        /* Statistics functions (in folder "functions/statistics") */
        
        // MCMC constructor function
        addFunction( "mcmc",   new Func_Mcmc() );
        
        // MCMCMC constructor function
        addFunction( "mcmcmc",   new Func_Mcmcmc() );

        
 		/* Statistics functions (in folder "functions/statistics") */
		
		// some helper statistics for the DPP distribution
        addFunction("fnDPPConcFromMean",  new Func_dppConcFromMean( )     );
        addFunction("fnDPPMeanFromConc",  new Func_dppMeanFromConc( )  );
        addFunction("fnStirling",  new Func_stirling( )     );
		
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
        addFunction( "fnDecompVarCovar",       new Func_decomposedVarianceCovarianceMatrix( )   );


    }
    catch(RbException& rbException)
    {
        
        RBOUT("Caught an exception while initializing functions in the workspace\n");
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


