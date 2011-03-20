/**
 * @file
 * This file contains the declaration of Monitor, used to hold information
 * about the monitoring of a variable DAG node.
 *
 * @brief Declaration of Monitor
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id$
 */

#ifndef Monitor_H
#define Monitor_H

#include "RbComplex.h"

#include <fstream>
#include <ostream>
#include <string>
#include <vector>

class RbObject;
class VectorString;
class VariableNode;

class Monitor : public RbComplex {

    public:
                                    Monitor(VariableNode* node, int freq);          //!< Constructor

        // Basic utility functions
        Monitor*                    clone(void) const;                              //!< Clone object
        bool                        equals(const RbObject* obj) const;              //!< Equals comparison
        const VectorString&         getClass(void) const;                           //!< Get class
        void                        printValue(std::ostream& o) const;              //!< Print value (for user)
        std::string                 richInfo(void) const;                           //!< Complete info about object

        // Monitor functions
        void                        monitor(std::ofstream& o) const;                //!< Monitor unconditionally
        void                        monitor(int gen, std::ofstream& o) const;       //!< Monitor at generation gen
        void                        printHeader(std::ofstream& o) const;            //!< Print header

    private:
        VariableNode*               theNode;                                         //!< The node to monitor
        int                         samplingFrequency;                               //!< Sampling frequency
};

#endif

