/**
 * @file
 * This file contains the declaration of Monitor, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Monitor_H
#define Monitor_H

#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class RbObject;
class StringVector;
class VariableNode;

class Monitor : public RbComplex {

    public:
                                Monitor(std::ostream& o, VariableNode* node, int freq); //!< Constructor

        // Basic utility functions
        RbObject*               clone() const;                                  //!< Clone object
        bool                    equals(const RbObject* obj) const;              //!< Equals comparison
        const StringVector&     getClass() const;                               //!< Get class
        void                    printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string             toString(void) const;                           //!< Complete info about object

        // Monitor functions
        void                    monitor(int gen) const;                         //!< Monitor at generation gen
        void                    printHeader(void) const;                        //!< Monitor at generation gen

    private:
        std::ostream&           outStrm;                                        //!< The stream for writing
        VariableNode*           theNode;                                        //!< The node to monitor
        int                     sampleFrequency;                                //!< The sample frequency
};

#endif


