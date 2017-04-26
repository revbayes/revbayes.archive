/**
 * @file
 * This file contains the declaration of RlPomoState, which is
 * a RevBayes wrapper around a regular Pomo character.
 *
 * @brief Declaration of RlPomoState
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date: 2012-08-23 09:27:32 +0200 (Thu, 23 Aug 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-09-08, version 1.0
 * @extends RbObject
 *
 * $Id: RlPomoState.h 1766 2012-08-23 07:27:32Z Boussau $
 */

#ifndef RlPomoState_H
#define RlPomoState_H

#include "PomoState.h"
#include "ModelObject.h"
#include "TypedDagNode.h"

#include <ostream>
#include <string>

namespace RevLanguage {
    
    class PomoState : public ModelObject<RevBayesCore::PomoState> {
        
    public:
        PomoState(void);                                                                                   //!< Default constructor
        PomoState(const RevBayesCore::PomoState &d);                                                  //!< Construct from PomoState
        
        // Operators
        
        // Basic utility functions
        PomoState*                      clone(void) const;                                                      //!< Clone object
        static const std::string&       getClassType(void);                                                     //!< Get Rev type
        static const TypeSpec&          getClassTypeSpec(void);                                                 //!< Get class type spec
        const TypeSpec&                 getTypeSpec(void) const;                                                //!< Get language type of the object
        
<<<<<<< HEAD
        std::string                     getGuiName(void) { return ""; }
        std::string                     getGuiSymbol(void) { return ""; }
        std::string                     getGuiInfo(void) { return ""; }
=======
>>>>>>> c54d844d6bccb46f193d8e4b18ac6f817cb88097
    };
    
}

#endif

