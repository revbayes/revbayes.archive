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
#include "Topology.h"
#include <set>
#include <string>

class TopologyNode;
class ValueRule;



const std::string TreePlate_name = "Tree Plate";

class TreePlate: public MutableMemberObject {

public:
                                    TreePlate(void);                                                                //!< Default constructor
                                    TreePlate(RbPtr<Topology> top);                                                 //!< constructor
                                    TreePlate(const TreePlate& t);                                                  //!< Copy constructor
                                    ~TreePlate(void);                                                               //!< Destructor

    // Basic utility functions
    TreePlate*                      clone(void) const;                                                              //!< Clone object
    const VectorString&             getClass(void) const;                                                           //!< Get class vector
    const TypeSpec&                 getTypeSpec(void) const;                                                        //!< Get language type of the object
    void                            printValue(std::ostream& o) const;                                              //!< Print value for user
    std::string                     richInfo(void) const;                                                           //!< Complete info

    // Member variable functions
    RbPtr<RbLanguageObject>         executeOperation(const std::string& name, const RbPtr<Environment>& args);      //!< Override to map member methods to internal functions
    RbPtr<const MemberRules>        getMemberRules(void) const;                                                     //!< Get member rules
    void                            setMemberVariable(const std::string& name, RbPtr<Variable> var);                //!< Catch setting of the topology

    // Member method inits
    RbPtr<const MethodTable>        getMethods(void) const;                                                         //!< Get methods

    // Tree plate functions
    RbPtr<const Topology>           getTopology(void) const;                                                        //!< Get the topology for this tree
    
protected:
    RbPtr<RbLanguageObject>         executeOperationSimple(const std::string& name, const RbPtr<Environment>& args); //!< Execute method

private:
    static const TypeSpec           typeSpec;
    
    std::vector<std::string>        nodeVariableNames;                                                              //!< The vector of variables names for each node
    
    std::string                     buildNewickString(RbPtr<const TopologyNode> node) const;                        //!< compute the newick string for a tree rooting at this node
    
    void                            orderPlateWithTopology(RbPtr<Topology> t) { orderingTopology = t; }             //!< Order the topology
    size_t                          getNodeIndex(RbPtr<const TopologyNode> theNode) const;                          //!< Get the index of the node
    size_t                          getTipIndex(RbPtr<const TopologyNode> theNode) const;                           //!< Get the index of the node

    RbPtr<Topology>                 orderingTopology;                                                               //!< The topology that orders the tree plate
};

#endif

