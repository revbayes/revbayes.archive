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
#include "AbstractModelObject.h"

/* Primitive types (in folder "datatypes/basic") */
#include "Integer.h"
#include "Natural.h"
#include "Probability.h"
#include "RlBoolean.h"
#include "RlString.h"
#include "Real.h"
#include "RealPos.h"

/* Container types (in folder "datatypes/container") */
#include "RlCorrespondenceAnalysis.h"
#include "RlMatrixReal.h"
#include "RlMatrixRealSymmetric.h"
#include "RlRateMap.h"
#include "RlRateMatrix.h"
#include "RlSimplex.h"

/* Container types (in folder "datatypes/math") */
#include "ModelVector.h"
#include "WorkspaceVector.h"

/* Evolution types (in folder "datatypes/phylogenetics") */

/* Character state types (in folder "datatypes/phylogenetics/character") */
#include "RlAminoAcidState.h"
#include "RlDnaState.h"
#include "RlRnaState.h"
#include "RlStandardState.h"

/* Character data types (in folder "datatypes/phylogenetics/datamatrix") */
#include "RlAbstractCharacterData.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"

/* Tree types (in folder "datatypes/phylogenetics/trees") */
#include "RlClade.h"
#include "RlRootedTripletDistribution.h"


/* Taxon types (in folder "datatypes/phylogenetics") */
#include "RlTaxon.h"

/* Inference types (in folder "analysis") */
#include "RlBurninEstimationConvergenceAssessment.h"
#include "RlModel.h"
#include "RlPathSampler.h"
#include "RlPosteriorPredictiveAnalysis.h"
#include "RlPosteriorPredictiveCharacterDataSimulation.h"
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


/// Types ///

/* These types are needed as template types for the moves */
#include "RlBranchLengthTree.h"
#include "RlMonitor.h"
#include "RlMove.h"
#include "RlRateGenerator.h"
#include "RlTimeTree.h"



/** Initialize global workspace */
void RevLanguage::Workspace::initializeTypeGlobalWorkspace(void)
{
    
    try
    {
        /* Add types: add a dummy variable which we use for type checking, conversion checking and other tasks. */
        
        /* Add base types (in folder "datatypes") */
        addType( new RevAbstractType( RevObject::getClassTypeSpec(), new Integer( 0 ) ) );

        /* Add primitive types (in folder "datatypes/basic") (alphabetic order) */
        AddWorkspaceVectorType<Integer,4>::addTypeToWorkspace( *this, new Integer() );
        AddWorkspaceVectorType<Natural,4>::addTypeToWorkspace( *this, new Natural() );
        AddWorkspaceVectorType<Probability,4>::addTypeToWorkspace( *this, new Probability() );
        AddWorkspaceVectorType<Real,4>::addTypeToWorkspace( *this, new Real() );
        AddWorkspaceVectorType<RealPos,4>::addTypeToWorkspace( *this, new RealPos() );
        AddWorkspaceVectorType<RlBoolean,4>::addTypeToWorkspace( *this, new RlBoolean() );
        AddWorkspaceVectorType<RlString,4>::addTypeToWorkspace( *this, new RlString() );
        AddWorkspaceVectorType<Simplex,4>::addTypeToWorkspace( *this, new Simplex() );
        AddWorkspaceVectorType<Taxon,4>::addTypeToWorkspace( *this, new Taxon() );
        
        
        
        
        AddWorkspaceVectorType<RateGenerator,3>::addTypeToWorkspace( *this, new RateGenerator() );
        AddWorkspaceVectorType<AbstractHomologousDiscreteCharacterData,3>::addTypeToWorkspace( *this, new AbstractHomologousDiscreteCharacterData() );
        
        AddWorkspaceVectorType<TimeTree,3>::addTypeToWorkspace( *this, new TimeTree() );
		AddWorkspaceVectorType<BranchLengthTree,3>::addTypeToWorkspace( *this, new BranchLengthTree() );
        AddWorkspaceVectorType<Clade,3>::addTypeToWorkspace( *this, new Clade() );
		
        
        //        AddWorkspaceVectorType<AbstractModelObject,2>::addTypeToWorkspace( *this, NULL );
        addFunction("v", new Func_workspaceVector<AbstractModelObject>() );
        
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
        addTypeWithConstructor( "CorrespondenceAnalysis",   new CorrespondenceAnalysis()                    );
        addType( new RateMap()              );
        addType( new MatrixReal()           );

        /* Add inference types (in folder "datatypes/inference") (alphabetic order) */
        addTypeWithConstructor( "beca",                             new BurninEstimationConvergenceAssessment()         );
        addTypeWithConstructor( "model",                            new Model()                                         );
        addTypeWithConstructor( "pathSampler",                      new PathSampler()                                   );
        addTypeWithConstructor( "posteriorPredictiveAnalysis",      new PosteriorPredictiveAnalysis()                   );
        addTypeWithConstructor( "posteriorPredictiveSimulation",    new PosteriorPredictiveCharacterDataSimulation()    );
        addTypeWithConstructor( "powerPosterior",                   new PowerPosteriorAnalysis()                        );
        addTypeWithConstructor( "steppingStoneSampler",             new SteppingStoneSampler()                          );

        /* Add stopping rules (in folder "analysis/stoppingRules") (alphabetic order) */
        addTypeWithConstructor( "srGelmanRubin",            new GelmanRubinStoppingRule()                   );
        addTypeWithConstructor( "srGeweke",                 new GewekeStoppingRule()                        );
        addTypeWithConstructor( "srMaxIteration",           new MaxIterationStoppingRule()                  );
        addTypeWithConstructor( "srMaxTime",                new MaxTimeStoppingRule()                       );
        addTypeWithConstructor( "srMinESS",                 new MinEssStoppingRule()                        );
        addTypeWithConstructor( "srStationarity",           new StationarityStoppingRule()                  );
    }
    catch(RbException& rbException)
    {
        
        RBOUT("Caught an exception while initializing types in the workspace\n");
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


