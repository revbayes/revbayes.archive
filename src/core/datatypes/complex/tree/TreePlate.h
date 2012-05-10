/**
 * @file
 * This file contains the declaration of a TreePlate a class that contains a topology and a set of variables.
 *
 * @brief Declaration of the TreePlate
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id$
 */

#ifndef TreePlate_H
#define TreePlate_H

#include "Environment.h"
#include "MemberObject.h"
#include "Topology.h"
#include <set>
#include <string>

class TopologyNode;
class ValueRule;


class TreePlate: public MemberObject {

public:
                                    TreePlate(void);                                                                                    //!< Default constructor
                                    TreePlate(const TreePlate& t);                                                                      //!< Copy constructor
                                   ~TreePlate(void);                                                                                    //!< Destructor

    // Basic utility functions
    TreePlate*                      clone(void) const;                                                                                  //!< Clone object
    static const std::string&       getClassName(void);                                                                                 //!< Get class name
    static const TypeSpec&          getClassTypeSpec(void);                                                                             //!< Get class type spec
    const TypeSpec&                 getTypeSpec(void) const;                                                                            //!< Get language type of the object
    void                            printValue(std::ostream& o) const;                                                                  //!< Print value for user

    // Member variable functions
    const MemberRules&              getMemberRules(void) const;                                                                         //!< Get member rules
    void                            setMemberVariable(const std::string& name, const RbPtr<RbLanguageObject> &var);                     //!< Catch setting of the topology

    // Member method inits
    const MethodTable&              getMethods(void) const;                                                                             //!< Get methods

    // Tree plate functions
    const Topology&                 getTopology(void) const;                                                                            //!< Get the topology for this tree
    void                            setNodeVariable(const TopologyNode& node, const std::string& name, RbLanguageObject* value);
    void                            setNodeVariable(const TopologyNode& node, const std::string& name, const Variable& value);
protected:
    RbPtr<RbLanguageObject>         executeOperationSimple(const std::string& name, const std::vector<RbPtr<Argument> >& args);                 //!< Execute method

private:
    
    std::vector<std::string>        nodeVariableNames;                                                                                  //!< The vector of variables names for each node
    
    std::string                     buildNewickString(const TopologyNode& node) const;                                                  //!< compute the newick string for a tree rooting at this node
    
    size_t                          getNodeIndex(const TopologyNode& theNode) const;                                                    //!< Get the index of the node
    size_t                          getTipIndex(const TopologyNode& theNode) const;                                                     //!< Get the index of the node

    // parameters
    RbPtr<Topology>                 orderingTopology;                                                               //!< The topology that orders the tree plate

    size_t                          numNodes;
    size_t                          nodeIndex;
    size_t                          tipIndex;


    Environment                     memberVariables;
};

#endif

