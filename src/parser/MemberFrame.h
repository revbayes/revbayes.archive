/**
 * @file
 * This file contains the declaration of MemberFrame, which is
 * used to hold member variables in MemberObject data types.
 *
 * @brief Declaration of MemberFrame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-11-17, version 1.0
 *
 * $Id$
 */

#ifndef MemberFrame_H 
#define MemberFrame_H

#include "VariableFrame.h"


#include <string>
#include <vector>


class MemberFrame : public VariableFrame {

    public:
                                    MemberFrame(void);                                                              //!< Constructor of variable frame with NULL parent
                                    MemberFrame(Frame* parentFr);                                                   //!< Constructor of variable frame with parent
                                    MemberFrame(const MemberFrame& x);                                              //!< Copy constructor: set frame of variable slots

        // Operators
        VariableSlot&               operator[](const std::string& name);                                            //!< Get named variable slot reference
        const VariableSlot&         operator[](const std::string& name) const;                                      //!< Get named variable slot const reference
        VariableSlot&               operator[](const size_t i);                                                     //!< Get slot by index
        const VariableSlot&         operator[](const size_t i) const;                                               //!< Get slot by index (const)

        // Basic utility functions
        MemberFrame*                clone(void) const { return new MemberFrame(*this); }                            //!< Clone variable frame
        std::string                 richInfo(void) const;                                                           //!< Complete info to string

        // Regular functions
        void                        addVariableSlot(const std::string& name, const TypeSpec& typeSp);               //!< Add empty slot
        void                        eraseVariable(const std::string& name);                                         //!< Erase a variable
        void                        push_back(const std::string& name, VariableSlot slot);                          //!< Add member variable slot
        size_t                      size(void) const { return members.size(); }                                     //!< Number of members

    protected:
        std::vector<std::string>    members;                                                                        //!< Vector of variable names for index access
};

#endif
