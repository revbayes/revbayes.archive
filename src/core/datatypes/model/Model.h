/**
 * @file
 * This file contains the declaration of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Declaration of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Model_H
#define Model_H

#include "ConstantMemberObject.h"

#include <ostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class StochasticNode;
class VectorString;

const std::string Model_name = "Model";

class Model : public ConstantMemberObject {

    public:
                                                Model(void);                                                //!< Default constructor for a Model object
                                                Model(const std::vector<DAGNode*>& sinkNodes);              //!< Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. 
                                                Model(const Model& x);                                      //!< Copy constructor for a Model object.

        // Assignment operator
        Model&                                  operator=(const Model& x);                                  //!< Assignment operator

        // Basic utility functions
        Model*                                  clone(void) const;                                          //!< Make a copy (clone) of the Model.
        const VectorString&                     getClass(void) const;                                       //!< Initialize the inheritance hierarchy for a Model object.
        const TypeSpec&                         getTypeSpec(void) const;                                    //!< Get language type of the object
        void                                    printValue(std::ostream& o) const;                          //!< Print the Model for the user as a list of the DAGNodes in the model graph.
    
        const MemberRules&                      getMemberRules(void) const;                                 //!< Get member rules
    
        // Model functions
        std::vector<DAGNode*>                   getClonedDagNodes(std::vector<DAGNode*> &orgNodes) const;   //!< Get cloned nodes corresponding to originals
        std::vector<RbDagNodePtr >            getDAGNodes(void) { return dagNodes; }                      //!< Return the DAGNodes in the model graph.
        void                                    setMemberVariable(const std::string& name, Variable* var);  //!< set a new member with name. catch setting of sinknode

	private:
        int                                     findIndexInVector(const std::vector<RbDagNodePtr >& v, const RbDagNodePtr p) const;

    // Member variables
    static const TypeSpec                       typeSpec;
    std::vector<RbDagNodePtr >                dagNodes;                
    std::map<const DAGNode*, RbDagNodePtr >   nodesMap;                                               //!< Map of node pointers between original nodes from the workspace to node in the model
};

#endif

