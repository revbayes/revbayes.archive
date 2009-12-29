/**
 * @file
 * This file contains the declaration of StochasticNode, which is derived
 * from DAGNode. StochasticNode is used for DAG nodes holding stochastic
 * variables with an associated probability density function.
 *
 * @brief Declaration of StochasticNode
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes development core team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-08-16, version 1.0
 * @extends DAGNode
 *
 * $Id$
 */

#ifndef StochasticNode_H
#define StochasticNode_H

#include "VariableNode.h"

class Distribution;
class RbObject;
class StringVector;

class StochasticNode : public VariableNode {

    public:
                            StochasticNode(Distribution* d);         //!< Constructor from distribution
                            StochasticNode(const StochasticNode& s); //!< Copy constructor
                            ~StochasticNode();                       //!< Destructor

        // Assignment operator
        StochasticNode&     operator=(const StochasticNode& x);      //!< Assignment operator

        // Basic utility functions
        StochasticNode*     clone() const;                          //!< Clone the stochastic node
        const StringVector& getClass() const;                       //!< Get class vector
        const RbObject*     getStoredValue();                       //!< Get stored value
        const RbObject*     getValElement(const IntVector& index) const;  //!< Get value element
        const RbObject*     getValue();                             //!< Get current value
        const RbObject*     getValue() const;                       //!< Get current value (const)
        const StringVector& getValueClass() const;                  //!< Get value class
        void                printStruct(std::ostream& o) const;     //!< Print struct for user
        void                printValue(std::ostream& o) const;      //!< Print struct for user
        void                setElement(const IntVector& index, RbObject* value);    //!< Set value element
        void                setValue(RbObject* value);              //!< Set value
        std::string         toString() const;                       //!< Complete info about object

        // Regular functions
        double              calculateLnProbability(void);           //!< Calculate log conditional probability
        void                clamp(RbObject* observedVal);           //!< Clamp the node with an observed value
        double              getLnLikelihoodRatio(void);             //!< Get log likelihood ratio
        double              getLnPriorRatio(void);                  //!< Get log prior ratio
        void                unclamp();                              //!< Unclamp the node
        
        // Functions for updating part of a DAG
        void                getAffected(std::set<StochasticNode*>& affected);   //!< Mark and get affected nodes
        void                keep();                                 //!< Keep value of this and affected nodes
        void    	        keepAffected();                         //!< Keep value of affected nodes recursively
        void                restore();                              //!< Restore value of this and affected nodes
        void                restoreAffected();                      //!< Restore value of  affected nodes recursively

    protected:
        // Function serving moves
        RbObject*           getValuePtr(std::set<StochasticNode*>& affected);   //!< Get mutable value and affected nodes

        // Member variables
        bool                clamped;                                //!< Is the node clamped with data?

        Distribution*       distribution;                           //!< Distribution (density functions, random draw function)

        RbObject*           value;                                  //!< Current value
        const RbObject*     storedValue;                            //!< Stored value

        friend class RbMove;                                        //!< Allow mmoves access to value
};

#endif

