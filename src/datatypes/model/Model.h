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

#include "MemberFrame.h"
#include "MemberObject.h"

#include <ostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class VectorString;

class Model : public MemberObject {

    public:
                                    Model(void);                                                                                            //!< Default constructor for a Model object
                                    Model(const std::vector<DAGNode*>& sinkNodes);                                                          //!< Constructor for the Model object that takes as an argument a vector containing at least one of the DAGNodes in the graph representing the model. 
                                    Model(const Model& x);                                                                                  //!< Copy constructor for a Model object.

        // Assignment operator
        Model&                      operator=(const Model& x);                                                                              //!< Assignment operator

        // Basic utility functions
        Model*                      clone(void) const;                                                                                      //!< Make a copy (clone) of the Model.
        const VectorString&         getClass(void) const;                                                                                   //!< Initialize the inheritance hierarchy for a Model object.
        void                        printValue(std::ostream& o) const;                                                                      //!< Print the Model for the user as a list of the DAGNodes in the model graph.
        std::string                 richInfo(void) const;                                                                                   //!< Complete info

        // Model functions
        std::vector<DAGNode*>&      getDAGNodes(void) { return dagNodes; }                                                                  //!< Return the DAGNodes in the model graph.

	protected:
        // Help functions
		void                        getExposedChildren(DAGNode* p, std::set<VariableNode*>& ec, std::vector<DAGNode*>& nodeList) const;     //!< Make a list of the children of a DAGNode (p) that are exposed to the user.
		void                        getExposedDagNodes(std::vector<DAGNode*>& exposedDagNodes, bool exposeEverybody, bool usePlates) const; //!< Get a list of the DAGNodes that are exposed to the user.
		void                        getExposedParents(DAGNode* p, std::set<DAGNode*>& ep, std::vector<DAGNode*>& nodeList) const;           //!< Make a list of the parents of a DAGNode (p) that are exposed to the user.
		int                         getIndexForVector(const std::vector<DAGNode*>& v, const DAGNode* p) const;                              //!< Finds position of a DAGNode in a vector of DAGNodes

        // Member variables
        std::vector<DAGNode*>       dagNodes;                                                                                               //!< Direct access to DAGNodes in the model graph
        MemberFrame                 dagNodeMembers;                                                                                         //!< Maintain dag node variables here, hidden from user
};

#endif

