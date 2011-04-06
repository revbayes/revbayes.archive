/**
 * @file
 * This file contains the declaration of ArgumentFrame, which
 * is used to hold arguments passed in to functions after
 * processing.
 *
 * @brief Declaration of ArgumentFrame
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 *
 * $Id$
 */

#ifndef ArgumentFrame_H 
#define ArgumentFrame_H

#include "Frame.h"
#include "VariableSlot.h"

#include <string>
#include <vector>

class DAGNode;
class RbObject;


class ArgumentFrame : public Frame {

    public:
        friend class                RbFunction;                                                                             //!< Give for condition possibility to modify referenc flag

                                    ArgumentFrame(void);                                                                    //!< Constructor of frame with NULL parent
                                    ArgumentFrame(Frame* parentFr);                                                         //!< Constructor of frame with parent
                                    ArgumentFrame(const ArgumentFrame& x);                                                  //!< Copy constructor: set frame of variable slots

        // Overloaded operators
        VariableSlot&               operator[](size_t i);                                                                   //!< Get variable slot reference
        const VariableSlot&         operator[](size_t i) const;                                                             //!< Get variable slot const reference
        VariableSlot&               operator[](const std::string& name);                                                    //!< Get named variable slot reference
        const VariableSlot&         operator[](const std::string& name) const;                                              //!< Get named variable slot const reference

        // Basic utility functions
        virtual ArgumentFrame*      clone(void) const;                                                                      //!< Clone object
        virtual void                printValue(std::ostream& o) const;                                                      //!< Print table for user
        virtual std::string         richInfo(void) const;                                                                   //!< Complete info to string

        // Regular functions
        void                        clear(void);                                                                            //!< Clear the frame
        size_t                      getIndex(const std::string& name) const;                                                //!< Get index of a named variable
        const std::string&          getLabel(size_t i) const { return arguments[i].first; }                                 //!< Get index of a named variable
        DAGNode*                    getReference(const std::string& name) const;                                            //!< Get reference
        const RbObject*             getValue(const std::string& name) const;                                                //!< Get value
        const DAGNode*              getVariable(const std::string& name) const;                                             //!< Get variable
        size_t                      size(void) const { return arguments.size(); }                                           //!< Get size of frame           

    protected:
        void                        addArgumentSlot(VariableSlot& slot);                                                    //!< Add argument slot without label
        void                        addArgumentSlot(const std::string& name, VariableSlot& slot);                           //!< Add argument slot with label
        void                        push_back(VariableSlot& slot);                                                          //!< Add argument slot without label
        void                        push_back(const std::string& name, VariableSlot& slot);                                 //!< Add argument slot with label
        void                        setArgumentLabel(size_t i, const std::string& name);                                    //!< Set argument name

        std::vector<std::pair<std::string, VariableSlot> >  arguments;                                                      //!< The arguments
};

#endif
