/**
 * @file
 * This file contains the declaration of Model, which is used to hold
 * DAGs copied from the working environment.
 *
 * @brief Declaration of Model
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2009-12-14 12:43:32 +0100 (Mån, 14 Dec 2009) $
 * @author The RevBayes core development team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-27, version 1.0
 *
 * $Id: MemberObject.h 194 2009-12-14 11:43:32Z ronquist $
 */

#ifndef Model_H
#define Model_H

#include "RbComplex.h"

#include <ostream>
#include <string>
#include <vector>

class ArgumentRule;
class DAGNode;
class StringVector;

class Model: public RbComplex {

    public:
                                    Model(void);                                        //!< Default constructor
                                    Model(const std::vector<DAGNode*>& sinkNodes);      //!< Constructor
                                    Model(const Model& x);                              //!< Copy constructor
								   ~Model(void);                                        //!< Destructor

        // Assignment operator
        Model&                      operator=(const Model& x);                          //!< Assignment operator

        // Basic utility functions
        Model*                      clone(void) const;                                  //!< Clone object
        bool                        equals(const RbObject* x) const;                    //!< Equals comparison
        const StringVector&         getClass(void) const;                               //!< Get class vector
        void                        printValue(std::ostream& o) const;                  //!< Print value for user
        std::string                 toString(void) const;                               //!< Complete info

        // Model functions
        std::vector<DAGNode*>&      getDAGNodes(void) { return dagNodes; }              //!< Get the DAG nodes

	protected:
        RbObject*                   convertTo(const std::string& type) const;           //!< Convert to type
        bool                        isConvertibleTo(const std::string& type) const;     //!< Is convertible to type and dim?
        std::vector<DAGNode*>       dagNodes;                                           //!< The DAG nodes
        std::vector<bool>           maintainedHere;                                     //!< True if DAG node maintained here
};

#endif

