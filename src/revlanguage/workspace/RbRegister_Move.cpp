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
#include "RlUserInterface.h"
#include "Workspace.h"

/// Types ///

/* These types are needed as template types for the moves */
#include "RlBranchLengthTree.h"
#include "RlRateGenerator.h"


#include "Probability.h"
#include "RlSimplex.h"


/// Moves ///

/* Move types (in folder "datatypes/inference/moves") (grouped by parameter type) */
#include "RlMove.h"

/* Moves on real values */
#include "Move_SliceSampling.h"
#include "Move_Scale.h"
#include "Move_Slide.h"

/* Compound Moves on Real Values */
#include "Move_UpDownTreeScale.h"
#include "Move_ScalerUpDown.h"
#include "Move_SliderUpDown.h"
#include "Move_LevyJumpSum.h"
#include "Move_LevyJump.h"

/* Moves on integer values */
#include "Move_RandomGeometricWalk.h"
#include "Move_RandomIntegerWalk.h"

/* Moves on simplices */
#include "Move_DirichletSimplex.h"
#include "Move_BetaSimplex.h"

/* Moves on real valued vectors */
#include "Move_ElementScale.h"
#include "Move_ElementSlide.h"
#include "Move_SingleElementScale.h"
#include "Move_SingleElementSlide.h"
#include "Move_VectorSingleElementScale.h"
#include "Move_VectorSingleElementSlide.h"
#include "Move_VectorFixedSingleElementSlide.h"
#include "Move_VectorScale.h"
#include "Move_VectorSlide.h"

/* Moves on real valued matrices */
#include "Move_MatrixSingleElementSlide.h"
#include "Move_ConjugateInverseWishartBrownian.h"


///* Moves on covariance matrices */
#include "Move_MatrixRealSymmetricSlide.h"


/* Moves on mixtures (in folder "datatypes/inference/moves/mixture") */
#include "Move_DPPAllocateAuxGibbsMove.h"
#include "Move_DPPGibbsConcentration.h"
#include "Move_MixtureAllocation.h"
#include "Move_ReversibleJumpSwitchMove.h"

/* Moves on character histories/data augmentation */
#include "Move_NodeCharacterHistoryRejectionSample.h"
#include "Move_PathCharacterHistoryRejectionSample.h"
#include "Move_CharacterHistory.h"


/* Moves on continuous phyloprocesses (Brownian, multivariate Brownian, etc) */

/* Tree proposals (in folder "datatypes/inference/moves/tree") */
#include "Move_CollapseExpandFossilBranch.h"
#include "Move_EmpiricalTree.h"
#include "Move_FNPR.h"
#include "Move_GibbsPruneAndRegraft.h"
#include "Move_NarrowExchange.h"
#include "Move_NNIClock.h"
#include "Move_NNINonclock.h"
#include "Move_NodeTimeScale.h"
#include "Move_NodeTimeSlideUniform.h"
#include "Move_NodeTimeSlideBeta.h"
#include "Move_RateAgeBetaShift.h"
#include "Move_RootTimeSlideUniform.h"
#include "Move_SpeciesNarrowExchange.h"
#include "Move_SpeciesNodeTimeSlideUniform.h"
#include "Move_SpeciesSubtreeScale.h"
#include "Move_SpeciesSubtreeScaleBeta.h"
#include "Move_SpeciesTreeNodeSlide.h"
#include "Move_SpeciesTreeScale.h"
#include "Move_SubtreeScale.h"
#include "Move_SPRNonclock.h"
#include "Move_TreeScale.h"
#include "Move_WeightedNodeTimeSlide.h"

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
        addTypeWithConstructor( new Move_Scale() );
        addTypeWithConstructor( new Move_Slide() );
        addTypeWithConstructor( new Move_SliceSampling() );
		
        /* compound moves */
//        addTypeWithConstructor("mvUpDownScale",         new Move_UpDownScale() );
        addTypeWithConstructor( new Move_UpDownTreeScale() );
        
		// compound moves on real values
        addTypeWithConstructor( new Move_ScalerUpDown() );
        addTypeWithConstructor( new Move_SliderUpDown() );
        addTypeWithConstructor( new Move_LevyJumpSum() );
        addTypeWithConstructor( new Move_LevyJump() );
        
        /* Moves on integer values */
        addTypeWithConstructor( new Move_RandomIntegerWalk() );
        addTypeWithConstructor( new Move_RandomGeometricWalk() );


        /* Moves on simplices */
        addTypeWithConstructor( new Move_DirichletSimplex() );
        addTypeWithConstructor( new Move_BetaSimplex() );

        /* Moves on vectors of real values */
        addTypeWithConstructor( new Move_SingleElementSlide() );
        addTypeWithConstructor( new Move_SingleElementScale() );
        addTypeWithConstructor( new Move_VectorScale() );
        addTypeWithConstructor( new Move_VectorSlide() );
        addTypeWithConstructor( new Move_ElementScale() );
        addTypeWithConstructor( new Move_ElementSlide() );
        addTypeWithConstructor( new Move_VectorSingleElementScale() );
        addTypeWithConstructor( new Move_VectorSingleElementSlide() );
        addTypeWithConstructor( new Move_VectorFixedSingleElementSlide() );
        
        /* Moves on matrices of real values */
        addTypeWithConstructor( new Move_MatrixSingleElementSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor( new Move_MatrixRealSymmetricSlide() );

        /* Moves on matrices of real values */
        addTypeWithConstructor( new Move_ConjugateInverseWishartBrownian() );

        /* Moves on mixtures (in folder "datatypes/inference/moves/mixture") */
//        addTypeWithConstructor("mvDPPScaleCatVals",                new Move_DPPScaleCatValsMove() );
//        addTypeWithConstructor("mvDPPScaleCatAllocateAux",         new Move_DPPScaleCatAllocateAux() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<Real>() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<RealPos>() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<Probability>() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<Integer>() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<Natural>() );
        addTypeWithConstructor( new Move_DPPAllocateAuxGibbsMove<Simplex>() );
        addTypeWithConstructor( new Move_DPPGibbsConcentration( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<Real>( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<RealPos>( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<Natural>( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<Integer>( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<Probability>( ) );
        addTypeWithConstructor( new Move_MixtureAllocation<RateGenerator>( ) );
        
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Real>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<RealPos>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Natural>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Integer>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Probability>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Simplex>( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<ModelVector<Natural> >( ) );
        addTypeWithConstructor( new Move_ReversibleJumpSwitch<Tree>( ) );

        /* Tree proposals (in folder "datatypes/inference/moves/tree") */
        addTypeWithConstructor( new Move_CollapseExpandFossilBranch() );
		addTypeWithConstructor( new Move_EmpiricalTree() );
        addTypeWithConstructor( new Move_FNPR() );
        addTypeWithConstructor( new Move_GibbsPruneAndRegraft() );
        addTypeWithConstructor( new Move_NarrowExchange() );
        addTypeWithConstructor( new Move_NNIClock() );
        addTypeWithConstructor( new Move_NNINonclock() );
        addTypeWithConstructor( new Move_NodeTimeScale() );
        addTypeWithConstructor( new Move_NodeTimeSlideUniform() );
        addTypeWithConstructor( new Move_NodeTimeSlideBeta() );
        addTypeWithConstructor( new Move_RateAgeBetaShift() );
        addTypeWithConstructor( new Move_RootTimeSlideUniform() );
        addTypeWithConstructor( new Move_SubtreeScale() );
        addTypeWithConstructor( new Move_SPRNonclock() );
        addTypeWithConstructor( new Move_SpeciesNarrowExchange() );
        addTypeWithConstructor( new Move_SpeciesNodeTimeSlideUniform() );
        addTypeWithConstructor( new Move_SpeciesSubtreeScale() );
        addTypeWithConstructor( new Move_SpeciesSubtreeScaleBeta() );
        addTypeWithConstructor( new Move_SpeciesTreeNodeSlide() );
        addTypeWithConstructor( new Move_SpeciesTreeScale() );
        addTypeWithConstructor( new Move_TreeScale() );
//        addTypeWithConstructor("mvFossilSafeSlide",             new Move_FossilSafeSlide() );
//        addTypeWithConstructor("mvFossilSafeScale",             new Move_FossilSafeScale() );
        
        /* Moves on character histories / data augmentation */
        addTypeWithConstructor( new Move_CharacterHistory() );
        addTypeWithConstructor( new Move_NodeCharacterHistoryRejectionSample() );
        addTypeWithConstructor( new Move_PathCharacterHistoryRejectionSample() );

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


