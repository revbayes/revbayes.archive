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
#include "Func_freeSymmetricRateMatrix.h"
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
#include "Func_PattersonsD.h"
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
#include "Func_readDistanceMatrix.h"
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
#include "Func_absVector.h"
#include "Func_ceil.h"
#include "Func_coala.h"
#include "Func_diagonalMatrix.h"
#include "Func_exp.h"
#include "Func_floor.h"
#include "Func_lnProbability.h"
#include "Func_hyperbolicTangent.h"
#include "Func_ln.h"
#include "Func_log.h"
#include "Func_logistic.h"
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

#include "RlDiscreteCharacterState.h"


/** Initialize global workspace */
void RevLanguage::Workspace::initializeFuncGlobalWorkspace(void)
{

    try
    {
        ///////////////////////////////////////////
        /* Add functions (in "functions" folder) */
        ///////////////////////////////////////////

        /* Rate matrix generator functions (in folder "functions/evolution/ratematrix") */
        addFunction( new Func_blosum62()                 );
        addFunction( new Func_chromosomes()              );
        addFunction( new Func_cpRev()                    );
        addFunction( new Func_dayhoff()                  );
        addFunction( new Func_DECRateMatrix()            );
        addFunction( new Func_epoch()                    );
        addFunction( new Func_f81()                      );
        addFunction( new Func_FreeBinary()               );
        addFunction( new Func_FreeK()                    );
        addFunction( new Func_freeSymmetricRateMatrix()  );
        addFunction( new Func_gtr()                      );
        addFunction( new Func_hky()                      );
        addFunction( new Func_InfiniteSitesRateMatrix()  );
        addFunction( new Func_jc()                       );
        addFunction( new Func_jones()                    );
        addFunction( new Func_k80()                      );
        addFunction( new Func_mtMam()                    );
        addFunction( new Func_mtRev()                    );
        addFunction( new Func_pomo()                     );
        addFunction( new Func_rtRev()                    );
        addFunction( new Func_t92()                      );
        addFunction( new Func_vt()                       );
        addFunction( new Func_wag()                      );

        /* rate maps used for data augmentation (in folder "functions/evolution/ratemap") */
        addFunction( new Func_biogeo_de() );
        addFunction( new Func_biogeo_grm() );

        /* cladogenic probs used for e.g. DEC models (in folder "functions/phylogenetics") */
        addFunction( new Func_cladoProbs() );
        addFunction( new Func_DECRates() );
        addFunction( new Func_DECRoot() );
        addFunction( new Func_phyloDiversity() );

		/* Functions related to phylogenetic trees (in folder "functions/phylogenetics/tree") */
		addFunction( new Func_treePairwiseDistances() );
		addFunction( new Func_treeAssembly( ) );

        /* Population genetics functions (in folder "functions/popgen") */
        addFunction( new Func_PattersonsD()      );
        addFunction( new Func_SegregatingSites() );
        addFunction( new Func_TajimasD()         );
        addFunction( new Func_TajimasPi()        );
        addFunction( new Func_WattersonTheta()   );


        /* Inference functions (in folder "functions/inference") */


        /* Internal functions (in folder "functions/internal") */

        /* Note: These are functions that are called implicitly, and the name of which, if
         called explicitly, starts with an underscore character. */

        // not templated logical functions
        addFunction( new Func__and()   );
        addFunction( new Func__or()    );
        addFunction( new Func__unot()  );

        // range function (x:y)
        addFunction( new Func_range()  );

        // logical templated functions
        addFunction( new Func__eq<             Integer,          Integer >()             );
        addFunction( new Func__eq<                Real,             Real >()             );
        addFunction( new Func__eq<             Integer,             Real >()             );
        addFunction( new Func__eq<                Real,          Integer >()             );
        addFunction( new Func__eq<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__eq<            RlString,         RlString >()             );
        addFunction( new Func__eq<             Simplex,          Simplex >()             );
        addFunction( new Func__eq<       RateGenerator,    RateGenerator >()             );
        addFunction( new Func__eq<            TimeTree,         TimeTree >()             );
        addFunction( new Func__eq<    BranchLengthTree, BranchLengthTree >()             );
        addFunction( new Func__eq<DiscreteCharacterState, DiscreteCharacterState >()     );
        addFunction( new Func__ge<             Integer,          Integer >()             );
        addFunction( new Func__ge<                Real,             Real >()             );
        addFunction( new Func__ge<             Integer,             Real >()             );
        addFunction( new Func__ge<                Real,          Integer >()             );
        addFunction( new Func__ge<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__gt<             Integer,          Integer >()             );
        addFunction( new Func__gt<                Real,             Real >()             );
        addFunction( new Func__gt<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__le<             Integer,          Integer >()             );
        addFunction( new Func__le<                Real,             Real >()             );
        addFunction( new Func__le<             Integer,             Real >()             );
        addFunction( new Func__le<                Real,          Integer >()             );
        addFunction( new Func__le<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__lt<             Integer,          Integer >()             );
        addFunction( new Func__lt<                Real,             Real >()             );
        addFunction( new Func__lt<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__ne<             Integer,          Integer >()             );
        addFunction( new Func__ne<                Real,             Real >()             );
        addFunction( new Func__ne<             Integer,             Real >()             );
        addFunction( new Func__ne<                Real,          Integer >()             );
        addFunction( new Func__ne<           RlBoolean,        RlBoolean >()             );
        addFunction( new Func__ne<       RateGenerator,    RateGenerator >()             );
        addFunction( new Func__ne<             Simplex,          Simplex >()             );
        addFunction( new Func__ne<            TimeTree,         TimeTree >()             );
        addFunction( new Func__ne<    BranchLengthTree, BranchLengthTree >()             );

        // unary minus (e.g. -a)
        addFunction( new Func__uminus<Integer, Integer>()  );
        addFunction( new Func__uminus<Natural, Integer>()  );
        addFunction( new Func__uminus<Real, Real>()        );
        addFunction( new Func__uminus<RealPos, Real>()     );

        // addition (e.g. a+b )
        addFunction( new Func__add< Natural                , Natural               , Natural               >(  )   );
        addFunction( new Func__add< Integer                , Integer               , Integer               >(  )   );
        addFunction( new Func__add< Real                   , Real                  , Real                  >(  )   );
        addFunction( new Func__add< RealPos                , RealPos               , RealPos               >(  )   );
        addFunction( new Func__add< RlString               , RlString              , RlString              >(  )   );
        addFunction( new Func__add< RlString               , Real                  , RlString              >(  )   );
        addFunction( new Func__add< RlString               , Integer               , RlString              >(  )   );
        addFunction( new Func__add< ModelVector<Natural>   , ModelVector<Natural>  , ModelVector<Natural>       >(  )   );
        addFunction( new Func__add< ModelVector<Integer>   , ModelVector<Integer>  , ModelVector<Integer>       >(  )   );
        addFunction( new Func__add< ModelVector<RealPos>   , ModelVector<RealPos>  , ModelVector<RealPos>       >(  )   );
        addFunction( new Func__add< ModelVector<Real>      , ModelVector<Real>     , ModelVector<Real>          >(  )   );
        addFunction( new Func__scalarVectorAdd<Natural     , ModelVector<Natural>  , ModelVector<Natural>       >(  )   );
        addFunction( new Func__scalarVectorAdd<Integer     , ModelVector<Integer>  , ModelVector<Integer>       >(  )   );
        addFunction( new Func__scalarVectorAdd<Real        , ModelVector<Real>     , ModelVector<Real>          >(  )   );
        addFunction( new Func__scalarVectorAdd<RealPos     , ModelVector<RealPos>  , ModelVector<RealPos>       >(  )   );

        // division
        addFunction( new Func__div< Natural                            , RealPos               , RealPos                   >(  )  );
        addFunction( new Func__div< RealPos                            , Natural               , RealPos                   >(  )  );
        addFunction( new Func__div< Integer                            , Real                  , Real                      >(  )  );
        addFunction( new Func__div< Real                               , Integer               , Real                      >(  )  );
        addFunction( new Func__div< Real                               , Real                  , Real                      >(  )  );
        addFunction( new Func__div< RealPos                            , RealPos               , RealPos                   >(  )  );
        addFunction( new Func__div< ModelVector<Natural>               , ModelVector<RealPos>  , ModelVector<RealPos>      >(  )  );
        addFunction( new Func__div< ModelVector<RealPos>               , ModelVector<Natural>  , ModelVector<RealPos>      >(  )  );
        addFunction( new Func__div< ModelVector<Integer>               , ModelVector<Real>     , ModelVector<Real>         >(  )  );
        addFunction( new Func__div< ModelVector<Real>                  , ModelVector<Integer>  , ModelVector<Real>         >(  )  );
        addFunction( new Func__div< ModelVector<RealPos>               , ModelVector<RealPos>  , ModelVector<RealPos>      >(  )  );
        addFunction( new Func__div< ModelVector<Real>                  , ModelVector<Real>     , ModelVector<RealPos>      >(  )  );
        addFunction( new Func__vectorScalarDiv<Natural                 , RealPos               , RealPos                   >(  )   );
        addFunction( new Func__vectorScalarDiv<RealPos                 , Natural               , RealPos                   >(  )   );
        addFunction( new Func__vectorScalarDiv<Integer                 , Real                  , Real                      >(  )   );
        addFunction( new Func__vectorScalarDiv<Real                    , Integer               , Real                      >(  )   );
        addFunction( new Func__vectorScalarDiv<Real                    , Real                  , Real                      >(  )   );
        addFunction( new Func__vectorScalarDiv<RealPos                 , RealPos               , RealPos                   >(  )   );
        addFunction( new Func__scalarVectorDiv<RealPos                 , Natural               , RealPos                   >(  )   );
        addFunction( new Func__scalarVectorDiv<Natural                 , RealPos               , RealPos                   >(  )   );
        addFunction( new Func__scalarVectorDiv<Real                    , Integer               , Real                      >(  )   );
        addFunction( new Func__scalarVectorDiv<Integer                 , Real                  , Real                      >(  )   );
        addFunction( new Func__scalarVectorDiv<Real                    , Real                  , Real                      >(  )   );
        addFunction( new Func__scalarVectorDiv<RealPos                 , RealPos               , RealPos                   >(  )   );

        // multiplication
        addFunction( new Func__mult< Natural               , Natural               , Natural               >(  )  );
        addFunction( new Func__mult< Integer               , Integer               , Integer               >(  )  );
        addFunction( new Func__mult< Real                  , Real                  , Real                  >(  )  );
        addFunction( new Func__mult< RealPos               , RealPos               , RealPos               >(  )  );
        addFunction( new Func__mult< ModelVector<Natural>  , ModelVector<Natural>  , ModelVector<Natural>  >(  )  );
        addFunction( new Func__mult< ModelVector<Integer>  , ModelVector<Integer>  , ModelVector<Integer>  >(  )  );
        addFunction( new Func__mult< ModelVector<RealPos>  , ModelVector<RealPos>  , ModelVector<RealPos>  >(  )  );
        addFunction( new Func__mult< ModelVector<Real>     , ModelVector<Real>     , ModelVector<Real>     >(  )  );
        addFunction( new Func__scalarVectorMult<Natural    , ModelVector<Natural>  , ModelVector<Natural>  >(  )   );
        addFunction( new Func__scalarVectorMult<Integer    , ModelVector<Integer>  , ModelVector<Integer>  >(  )   );
        addFunction( new Func__scalarVectorMult<Real       , ModelVector<Real>     , ModelVector<Real>     >(  )   );
        addFunction( new Func__scalarVectorMult<RealPos    , ModelVector<RealPos>  , ModelVector<RealPos>  >(  )   );

        // subtraction
        addFunction( new Func__sub< Integer                            , Integer               , Integer               >(  )  );
        addFunction( new Func__sub< Real                               , Real                  , Real                  >(  )  );
        addFunction( new Func__sub< ModelVector<Integer>               , ModelVector<Integer>  , ModelVector<Integer>  >(  )  );
        addFunction( new Func__sub< ModelVector<Real>                  , ModelVector<Real>     , ModelVector<Real>     >(  )  );
        addFunction( new Func__vectorScalarSub<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( new Func__vectorScalarSub<Real                    , Real                  , Real                      >(  )   );
        addFunction( new Func__scalarVectorSub<Integer                 , Integer               , Integer                   >(  )   );
        addFunction( new Func__scalarVectorSub<Real                    , Real                  , Real                      >(  )   );

        // modulo
        addFunction( new Func__mod() );

        // exponentiation
        addFunction( new Func_power() );
        addFunction( new Func_powerVector() );

        /* Math functions (in folder "functions/math") */

		// absolute function
        addFunction( new Func_abs()                  );
        addFunction( new Func_absVector()            );

		// ceil function
        addFunction( new Func_ceil<Real,Integer>()  );
        addFunction( new Func_ceil<RealPos,Natural>()  );
        addFunction( new Func_ceil<Real,Integer>()  );
        addFunction( new Func_ceil<RealPos,Natural>()  );

        // coala function
        addFunction( new Func_coala()        );

        // diagonal matrix
        addFunction( new Func_diagonalMatrix() );

        // exponential function
        addFunction( new Func_exp() );

		// floor function
        addFunction( new Func_floor<Real,Integer>()  );
        addFunction( new Func_floor<RealPos,Natural>()  );

        // logistic function
        addFunction( new Func_logistic() );

        // natural log function
        addFunction( new Func_ln()  );

        // log function
        addFunction( new Func_log()  );

        // min function
        addFunction( new Func_max()  );

        // mean function
		addFunction( new Func_mean()  );

        // min function
		addFunction( new Func_min()  );

        // normalize vector function
		addFunction( new Func_normalize()  );

		// power function
        addFunction( new Func_power() );
        addFunction( new Func_powerVector() );

        // conversion function from Real to Probability
        addFunction( new Func_probability() );

		// round function
        addFunction( new Func_round<Real,Integer>()  );
        addFunction( new Func_round<RealPos,Natural>()  );

        // simplex constructor function (from RealPos ellipsis argument values)
        addFunction( new Func_simplex()                  );

        // simplex constructor function (from vector of RealPos values)
        addFunction( new Func_simplexFromVector()        );

		// square root function
        addFunction( new Func_sqrt()  );

        // sum function
        addFunction( new Func_sum()  );
        addFunction( new Func_sumPositive()  );

        // standard deviation function
        addFunction( new Func_standardDeviation()  );
        addFunction( new Func_standardDeviation()  );

        // hyperbolic tangent function
        addFunction( new Func_hyperbolicTangent() );

		// truncate function
        addFunction( new Func_trunc<Real,Integer>()  );
        addFunction( new Func_trunc<RealPos,Natural>()  );

        // mean function
        addFunction( new Func_variance()  );

        // vector flatten
        addFunction( new Func_vectorFlatten() );

        // get ln Probability function
        addFunction( new Func_lnProbability() );



        /* Statistics functions (in folder "functions/statistics") */

        // MCMC constructor function
        addFunction( new Func_Mcmc() );

        // MCMCMC constructor function
        addFunction( new Func_Mcmcmc() );


 		/* Statistics functions (in folder "functions/statistics") */

		// some helper statistics for the DPP distribution
        addFunction( new Func_dppConcFromMean( )     );
        addFunction( new Func_dppMeanFromConc( )  );
        addFunction( new Func_stirling( )     );

		// count the number of unique elements in vector
        addFunction( new Func_numUniqueInVector<Real>( )  );
        addFunction( new Func_numUniqueInVector<RealPos>( )  );
        addFunction( new Func_numUniqueInVector<Integer>( )  );
        addFunction( new Func_numUniqueInVector<Natural>( )  );
        addFunction( new Func_numUniqueInVector<Probability>( )  );
        addFunction( new Func_numUniqueInVector<Simplex>( )  );

        // return a distcretized (by quantile) and normalized vector from a continuous distribution
        addFunction( new Func_fnNormalizedQuantile<Real>()    );
        addFunction( new Func_fnNormalizedQuantile<RealPos>()    );

        addFunction( new Func_discretizeDistribution( )         );
        addFunction( new Func_discretizePositiveDistribution( ) );

        // return a discretized gamma distribution (for gamma-dist rates)
        addFunction( new Func_discretizeGamma( )   );

        addFunction( new Func_varianceCovarianceMatrix( )   );
        addFunction( new Func_decomposedVarianceCovarianceMatrix( )   );


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


