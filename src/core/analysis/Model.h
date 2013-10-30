#ifndef Model_H
#define Model_H

#include <map>
#include <set>
#include <vector>

namespace RevBayesCore {

    class DagNode;
    
    /**
     * \brief Declaration of the Model class
     *
     * This file contains the declaration of the Model class. 
     * A Model object holds the model graph which is an independent copy
     * of the DAG nodes contained in the model.
     *
     * \copyright (c) Copyright 2009-2013 (GPL version 3)
     * \date Last modified: $Date$
     * \author The RevBayes Development Core Team ($Author$)
     * \since Version 1.0, 2012-06-21
     *
     */
    class Model {
        
        public:
                                                                    Model(const DagNode* source);
                                                                    Model(const std::set<const DagNode*> sources);
                                                                    Model(const Model &m);
        virtual                                                    ~Model(void);                                                          
    
        
        // overloaded operators
        Model&                                                      operator=(const Model& x);
        
        
        // convenience methods
        Model*                                                      clone(void) const;                                                      
    
        // getters and setters
        std::vector<DagNode*>&                                      getDagNodes(void);
        const std::vector<DagNode*>&                                getDagNodes(void) const;
        const std::map<const DagNode*, DagNode*>&                   getNodesMap(void) const;

    private:
        // private methods
        void                                                        addSourceNode(const DagNode *sourceNode);
    
        // members
        std::vector<DagNode*>                                       nodes;                                                          //!< The DAG nodes of the model graph
        std::map<const DagNode*, DagNode*>                          nodesMap;                                                       //!< Map between original nodes and own copy
        std::set<const DagNode*>                                    sources;                                                        //!< Set of source nodes for the model graph
    };
    
}

#endif
