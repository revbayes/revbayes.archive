/**
 * @file
 * This file contains the declaration of a Topology, the abstract class for all types of Topologys.
 * 
 * We consider Topologys as member objects because they can be associated with distribution and hence
 * need to be able to be wrapped into constant or stochastic dag nodes.
 *
 * A Topology itself is considered as a model rather than a parameter. Considering a Topology as a model instead of
 * a parameter has the effect that the Topology is a collection of dag nodes with a certain structure, namely that
 * the dag nodes are ordered as a Topology. Even when considering Topologys as models, we enable prior distribution on Topologys.
 * Priors on models has the statistical foundation of model comparison and hence can be incorporated into standard mcmc.
 *
 * The Topology is a very light weith object which holds the Topology structure and provides some convinience methods 
 * associated with Topologys (e.g. getTips(), getNewick()). The actual variables on the Topology are associated with the TopologyNodes.
 *
 * Internally we structure all Topologys as rooted Topologys for convinience, though there are derived classes which are rooted or unrooted.
 * Topologys rely on the structure of the Topology nodes, which know their parents and their children. The Topology nodes are flexible enough 
 * to hold any type of descendancies, being either bifurcating or multifurcating.
 *
 * @brief Declaration of the Topology
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

#ifndef Topology_H
#define Topology_H

#include "MemberObject.h"
#include <set>
#include <string>

class ArgumentFrame;
class ArgumentRule;
class DAGNode;
class MethodTable;
class TopologyNode;
class VectorString;



class Topology: public MemberObject {
    
    public:
                                        Topology(void);                                                         //!< Default constructor
                                        Topology(const Topology& t);                                            //!< Copy constructor
                                       ~Topology(void) {}                                                       //!< Destructor

        // Basic utility functions
        Topology*                       clone(void) const;                                                      //!< Clone object
        const VectorString&             getClass(void) const;                                                   //!< Get class vector   
        void                            printValue(std::ostream& o) const;                                      //!< Print value for user
        std::string                     richInfo(void) const;                                                   //!< Complete info

        // Member variable rules
        const MemberRules&              getMemberRules(void) const;                                             //!< Get member rules

        // Member method inits
        DAGNode*                        executeOperation(const std::string& name, ArgumentFrame& args);         //!< Execute method
        const MethodTable&              getMethods(void) const;                                                 //!< Get methods
        
        // Topology functions
        bool                            getIsRooted(void) { return isRooted; }                                  //!< Is the tree rooted
        std::vector<TopologyNode*>&     getNodes(void) { return nodes; }                                        //!< Get a pointer to the nodes in the tree
        size_t                          getNumberOfNodes(void) const { return int(nodes.size()); }              //!< Get the number of nodes in the tree
        size_t                          getNumberOfTips(void) const;                                            //!< Get the number of tip nodes in the tree
        TopologyNode*                   getRoot(void) const { return root; }                                    //!< Get a pointer to the root node of the tree
        void                            setIsRooted(bool tf) { isRooted = tf; }                                 //!< Set the rootedness of the tree
        
	private:
        std::vector<TopologyNode*>      nodes;                                                                  //!< vector of pointers to all nodes
        TopologyNode*                   root;                                                                   //!< Pointer to the root node
        bool                            isRooted;                                                               //!< Is the topology rooted?
};

#endif

