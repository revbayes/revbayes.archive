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
                                                Model(const Model& x);                                      //!< Copy constructor for a Model object.

        // Assignment operator
        Model&                                  operator=(const Model& x);                                  //!< Assignment operator

        // Basic utility functions
        Model*                                  clone(void) const;                                          //!< Make a copy (clone) of the Model.
        static const std::string&               getClassName(void);                                                         //!< Get class name
        static const TypeSpec&                  getClassTypeSpec(void);                                                     //!< Get class type spec
        const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                          //!< Print the Model for the user as a list of the DAGNodes in the model graph.
        void                                    printLeanValue(std::ostream& o) const;                          //!< Print the Model for the user as a list of the DAGNodes in the model graph.
   
        const MemberRules&                      getMemberRules(void) const;                                 //!< Get member rules
        void                                    setMemberVariable(const std::string &name, const Variable* var);  //!< Set a member variable. We catch here setting of variable nodes
    
        // Model functions
        const std::set<const DAGNode*>&         getSourceNodes(void) const;                                 // get the source node to pull copies of the model
        const std::map<const DAGNode*, InferenceDagNode*>&  getNodesMap(void) const;
// Sebastian: These functions are commented out because they violate our const-correctness!
        const std::vector<InferenceDagNode*>&   getLeanDagNodes(void) const { return leanDagNodes; }        //!< Return the DAGNodes in the model graph.
//        const std::vector<const Monitor*>&      getMonitors(void) const { return monitors; }
//        const std::vector<const Move*>&         getMoves(void) const { return moves; }

	private:
        void                                    addSourceNode(const DAGNode* sourceNode);
        void                                    createModelFromDagNode(const DAGNode *source );
        void                                    createLeanDag(const DAGNode* fatDagNode);
        int                                     findIndexInVector(const std::vector<RbPtr<DAGNode> >& v, const DAGNode* p) const;
        int                                     findIndexInVector(const std::vector<InferenceDagNode*>& v, const InferenceDagNode* p) const;
        
        // Member variables
        std::vector<RbPtr<DAGNode> >            dagNodes;                                                   //!< The DAG nodes contained in this model
        std::vector<InferenceDagNode*>          leanDagNodes;
        std::set<const DAGNode*>                sourceNodes;                                                //!< The source node for this model
    
        std::map<const DAGNode*, RbPtr<DAGNode> >  nodesMap;
        std::map<const DAGNode*, InferenceDagNode*>  leanNodesMap;
        std::map<const DAGNode*, InferenceDagNode*> orgNodesMap;
};

#endif

