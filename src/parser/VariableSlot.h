/**
 * @file
 * This file contains the declaration of VariableSlot, which is
 * used to manage variables in frames and processed argument lists.
 *
 * @brief Declaration of VariableSlot
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2010-09-06, version 1.0
 *
 * $Id$
 */

#ifndef VariableSlot_H 
#define VariableSlot_H

#include "TypeSpec.h"

#include <string>

class DAGNode;

class VariableSlot {
    
    public:
        friend class            Frame;                                                          //!< Expose slot to frame

                                VariableSlot(DAGNode* var, bool ref=false);                     //!< Constructor of filled slot
                                VariableSlot(const std::string& type, int dim, bool ref=false); //!< Constructor of empty slot
                                VariableSlot(const VariableSlot& x);                            //!< Copy value or reference correctly
        virtual                ~VariableSlot(void);                                             //!< Manage variable (DAGNode) destruction

        // Assignment operator
        VariableSlot&           operator=(const VariableSlot& x);                               //!< Assignment

        // Regular functions
        const std::string&      getName(void) const;                                            //!< Get name of slot
        const DAGNode*          getReference(void) const;                                       //!< Get a const reference to the variable
        DAGNode*                getReference(void);                                             //!< Get a reference to the variable
        const TypeSpec&         getTypeSpec(void) const { return typeSpec; }                    //!< Return type specification for slot
        const DAGNode*          getValue(void) const;                                           //!< Get the value of the variable
        void                    printValue(std::ostream& o) const;                              //!< Print value of slot
        void                    setFrame(Frame* slotFrame) { frame=slotFrame; }                 //!< Set frame of slot
        void                    setReference(DAGNode* ref);                                     //!< Set a slot with a reference
        void                    setReferenceFlag(bool refFlag=true);                            //!< Potentially switch between reference and value slot
        void                    setValue(DAGNode* value);                                       //!< Set a slot with a value
        void                    swapReference(DAGNode* oldRef, DAGNode* newRef);                //!< Swap reference

    private:
        // Help function
        void                    removeVariable(void);                           //!< Remove old variable from slot

        // Member variables
        TypeSpec                typeSpec;                                       //!< The type specification for the slot
        bool                    temp;                                           //!< Flags whether a reference slot is used to hold a temp value
        DAGNode*                variable;                                       //!< Pointer to the variable
        Frame*                  frame;                                          //!< Pointer to the slot frame or NULL if none
};

#endif
