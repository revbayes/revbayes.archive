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

/// Types ///

/* These types are needed as template types for the moves */
#include "RlBranchLengthTree.h"
#include "RlRateGenerator.h"




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
#include "Move_LevyJumpSum.h"
#include "Move_LevyJump.h"

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
#include "Move_VectorSlide.h"

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

/** Initialize global workspace */
void RevLanguage::Workspace::initializeMoveGlobalWorkspace(void)
{
    
    try
    {  
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
        addTypeWithConstructor("mvSliderUpDown",        new Move_SliderUpDown() );
        addTypeWithConstructor("mvLevyJumpSum",         new Move_LevyJumpSum() );
        addTypeWithConstructor("mvLevyJump",            new Move_LevyJump() );
        
        /* Moves on integer values */
        addTypeWithConstructor("mvRandomIntegerWalk",   new Move_RandomIntegerWalk() );
        addTypeWithConstructor("mvRandomGeometricWalk", new Move_RandomGeometricWalk() );


        /* Moves on simplices */
        addTypeWithConstructor("mvSimplex",             new Move_Simplex() );
        addTypeWithConstructor("mvSimplexElementScale", new Move_SimplexSingleElementScale() );        

        /* Moves on vectors of real values */
        addTypeWithConstructor("mvSingleElementScale",          new Move_SingleElementScale() );
        addTypeWithConstructor("mvVectorScale",                 new Move_VectorScale() );
        addTypeWithConstructor("mvVectorSlide",                 new Move_VectorSlide() );
        addTypeWithConstructor("mvVectorSingleElementScale",    new Move_VectorSingleElementScale() );
        addTypeWithConstructor("mvVectorSingleElementSliding",  new Move_VectorSingleElementSlide() );
        
        /* Moves on matrices of real values */
        addTypeWithConstructor("mvMatrixElementSlide",          new Move_MatrixSingleElementSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor("mvSymmetricMatrixElementSlide", new Move_MatrixRealSymmetricSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor("mvConjugateInverseWishartBrownian", new Move_ConjugateInverseWishartBrownian() );

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
        addTypeWithConstructor("mvMixtureAllocation",              new Move_MixtureAllocation<RateGenerator>( ) );
        
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Real>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<RealPos>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Natural>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Integer>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Probability>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<Simplex>( ) );
        addTypeWithConstructor("mvRJSwitch",                    new Move_ReversibleJumpSwitch<ModelVector<Natural> >( ) );

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
        addTypeWithConstructor("mvFossilSafeSlide",         new Move_FossilSafeSlide() );
        addTypeWithConstructor("mvFossilSafeScale",         new Move_FossilSafeScale() );
        
        /* Moves on character histories / data augmentation */
        addTypeWithConstructor("mvCharacterHistory",                    new Move_CharacterHistory<BranchLengthTree>() );
        addTypeWithConstructor("mvCharacterHistory",                    new Move_CharacterHistory<TimeTree>() );
        addTypeWithConstructor("mvNodeCharacterHistoryRejectionSample", new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvNodeCHRS",                            new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCharacterHistoryRejectionSample", new Move_PathCharacterHistoryRejectionSample() );
        addTypeWithConstructor("mvPathCHRS",                            new Move_PathCharacterHistoryRejectionSample() );

    }
    catch(RbException& rbException)
    {
        
        RBOUT("Caught an exception while initializing moves in the workspace\n");
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


