/**
 * @file
 * This file contains the declaration of the Model class. 
 * A Model object holds the model graph.
 *
 *
 * @brief Declaration of the Model class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-06-21
 *
 * $Id$
 */


#ifndef Model_H
#define Model_H

#include <map>
#include <set>
#include <vector>

namespace RevBayesCore {

    class DagNode;

    class Model {
    
        public:
                                                                    Model(const DagNode* source);
                                                                    Model(const std::set<const DagNode*> sources);
                                                                    Model(const Model &m);
        virtual                                                    ~Model(void);                                                          //!< Virtual destructor
    
        // convenience methods
        Model*                                                      clone(void) const;                                                      //!< Create clone
    
        // getters and setters
        std::vector<DagNode*>&                                      getDagNodes(void);
        const std::vector<DagNode*>&                                getDagNodes(void) const;
        const std::map<const DagNode*, DagNode*>&                   getNodesMap(void) const;
        const std::map<const DagNode*, std::set<DagNode*> >&        getReplacementMap(void) const;

    private:
        // private methods
        void                                                        addSourceNode(const DagNode *sourceNode);
    
        // members
        std::vector<DagNode*>                                       nodes;
        std::map<const DagNode*, DagNode*>                          nodesMap;
        std::map<const DagNode*, std::set<DagNode*> >               replacementMap;
        std::set<const DagNode*>                                    sources;
    };
    
}

#endif
