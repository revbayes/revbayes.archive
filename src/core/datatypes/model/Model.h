/**
 * @file
 * This file contains the declaration of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Declaration of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id$
 */

#ifndef Model_H
#define Model_H

#include "MemberObject.h"

#include <ostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class StochasticNode;


class Model : public MemberObject {

    public:
                                                Model(void);                                                //!< Default constructor for a Model object
                                                Model(const std::vector<DAGNode*>& sinkNodes);              //!< Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. 
                                                Model(const Model& x);                                      //!< Copy constructor for a Model object.

        // Assignment operator
        Model&                                  operator=(const Model& x);                                  //!< Assignment operator

        // Basic utility functions
        Model*                                  clone(void) const;                                          //!< Make a copy (clone) of the Model.
        static const std::string&               getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                          //!< Print the Model for the user as a list of the DAGNodes in the model graph.
    
        const MemberRules&                      getMemberRules(void) const;                                 //!< Get member rules
        void                                    setMemberVariable(const std::string &name, const Variable* var);  //!< Set a member variable. We catch here setting of variable nodes
    
        // Model functions
        bool                                    areDagNodesCloned(std::vector<DAGNode*> &orgNodes) const;   //!< Are these already the cloned DAG nodes?
        std::vector<DAGNode*>                   getClonedDagNodes(std::vector<DAGNode*> &orgNodes) const;   //!< Get cloned nodes corresponding to originals
        std::vector<RbDagNodePtr>               getDAGNodes(void) { return dagNodes; }                      //!< Return the DAGNodes in the model graph.

	private:
        int                                     findIndexInVector(const std::vector<RbDagNodePtr>& v, const DAGNode* p) const;

    // Member variables
    std::vector<RbDagNodePtr>                   dagNodes;                
    std::map<const DAGNode*, RbDagNodePtr>      nodesMap;                                               //!< Map of node pointers between original nodes from the workspace to node in the model
};

#endif

