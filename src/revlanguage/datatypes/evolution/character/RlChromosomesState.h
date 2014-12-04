/**
 * @file
 * This file contains the declaration of RlChromosomesState, which is
 * a RevBayes wrapper around a regular Chromosomes character.
 *
 * @brief Declaration of RlChromosomesState
 *
 * (c) Copyright 2014-
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 *
 * $Id: RlChromosomesState.h $
 */

#ifndef RlChromosomesState_H
#define RlChromosomesState_H

#include "ChromosomesState.h"
#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
	
    class ChromosomesState : public ModelObject<RevBayesCore::ChromosomesState> {
        
    public:
        ChromosomesState(void);                                                                                 //!< Default constructor
        ChromosomesState(RevBayesCore::TypedDagNode<RevBayesCore::ChromosomesState> *v);                                                            //!< Constructor from DAG node
        ChromosomesState(const RevBayesCore::ChromosomesState &d);                                              //!< Construct from ChromosomesState
        
        // Operators
        
        // Basic utility functions
        ChromosomesState*               clone(void) const;                                                      //!< Clone object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
		
    };
    
}

#endif