/**
 * @file
 * This file contains the declaration of a TreePlate a class that contains a topology and a set of variables.
 * 
 * @brief Declaration of the TreePlate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date:$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

#ifndef TreePlate_H
#define TreePlate_H

#include "MutableMemberObject.h"
#include <set>
#include <string>

class Topology;
class ValueRule;



class TreePlate: public MutableMemberObject {
    
    public:
                                        TreePlate(void);                                                        //!< Default constructor
                                        TreePlate(Topology* top);                                               //!< constructor
                                        TreePlate(const TreePlate& t);                                          //!< Copy constructor
                                       ~TreePlate(void);                                                        //!< Destructor

        // Basic utility functions
        TreePlate*                      clone(void) const;                                                      //!< Clone object
        const VectorString&             getClass(void) const;                                                   //!< Get class vector   
        void                            printValue(std::ostream& o) const;                                      //!< Print value for user
        std::string                     richInfo(void) const;                                                   //!< Complete info

        // Member variable functions
        const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules
        void                            setMemberVariable(const std::string& name, Variable* var);              //!< Catch setting of the topology

        // Member method inits
        RbLanguageObject*               executeOperation(const std::string& name, Environment& args);           //!< Execute method
        const MethodTable&              getMethods(void) const;                                                 //!< Get methods
        
        // Tree plate functions

	private:
        void                            orderPlateWithTopology(Topology* t) { orderingTopology = t; }           //!< Order the topology
        
        Topology*                       orderingTopology;                                                       //!< The topology that orders the tree plate
};

#endif

