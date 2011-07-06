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
class RandomNumberGenerator;
class TopologyNode;
class VectorString;

typedef std::vector<ArgumentRule*>   MemberRules;                                                               //!< Member rules type def, for convenience



class Topology: public MemberObject {
    
    public:
        virtual                        ~Topology(void) {}                                                       //!< Destructor
        
        // Basic utility functions you have to override
        virtual Topology*               clone(void) const = 0;                                                  //!< Clone object
        virtual const VectorString&     getClass(void) const;                                                   //!< Get class vector
        
        // Basic utility functions you may want to override
        virtual void                    printValue(std::ostream& o) const;                                      //!< Print value for user
        virtual std::string             richInfo(void) const;                                                   //!< Complete info

        
        // Member variable functions; override getMemberRules to add member variables
        virtual const MemberRules&      getMemberRules(void) const;                                             //!< Get member rules
        virtual void                    setVariable(const std::string& name, DAGNode* var);                     //!< Set member variable
        
        // Member method functions
        virtual const MethodTable&      getMethods(void) const;                                                 //!< Get member methods
        
        // Topology specific methods
        virtual void                    buildRandomTopology(int nTips, RandomNumberGenerator* rng) = 0;         //!< TODO: This function should go into one of the Topology prior distributions (Sebastian)
        std::string                     getNewickTopology(void) const { return newick; }                        //!< Xxxxxx
        std::vector<TopologyNode*>      getNodes(void) const { return nodes; }                                  //!< Xxxxxx
        int                             getNumberOfNodes(void) const { return int(nodes.size()); }              //!< Xxxxxx
        int                             getNumberOfTips(void) const { return int(tips.size()); }                //!< Xxxxxx
        TopologyNode*                   getRoot(void) const { return root; }                                    //!< Xxxxxx
        std::vector<TopologyNode*>      getTips(void) const { return tips; }                                    //!< Xxxxxx
        void                            randomlyBreakTopology(RandomNumberGenerator* rng) { }                   //!< Xxxxxx
        void                            removeSubTopologyFromNode(TopologyNode* p) { }                          //!< Xxxxxx
        void                            setTopologyChanged(bool changed) { topologyChanged = changed; }         //!< Xxxxxx   
        
    protected:
                                        Topology(void);                                                         //!< Default constructor
                                        Topology(const Topology& t);                                            //!< Copy constructor
                                        Topology(const MemberRules& memberRules);                               //!< Constructor
        
        void                            computeNewickString(void);                                              //!< recompute the newick string
        std::string                     newick;                                                                 //!< The newick string
        std::vector<TopologyNode*>      nodes;                                                                  //!< vector of pointers to all nodes
        std::vector<TopologyNode*>      tips;                                                                   //!< vector of pointers to the tips
        TopologyNode*                   root;                                                                   //!< pointer to the root node
        
        // flags indicating recalculation
        bool                            topologyChanged;                                                        //!< flag for topology changes. if the flag is set, then the newick string, nodes in traversal oders
        
        // Override this function to map member method calls to internal functions
        virtual DAGNode*                executeOperation(const std::string& name, ArgumentFrame& args);         //!< Map member methods to internal functions
        
        // Members is the variable frame that stores member variables
        MemberFrame                     members;                                                                //!< Member variables
};

#endif

