/**
 * @file
 * This file contains the declaration of InferenceMove, which is the abstract
 * base class for InferenceMoves used in mcmc inference.
 *
 * @brief Declaration of InferenceMove
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-03-23 18:42:52 +0100 (Fri, 23 Mar 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2009-09-08, version 1.0
 *
 * $Id: InferenceMove.h 1358 2012-03-23 17:42:52Z hoehna $
 */

#ifndef InferenceMove_H
#define InferenceMove_H

#include <vector>

class InferenceMove {
    
public:
    virtual                                    ~InferenceMove(void);                                                              //!< Destructor
     
    // InferenceMove functions you have to override
    virtual void                            accept(void) = 0;                                                                   //!< Accept the InferenceMove
    virtual InferenceMove*                  clone(void) = 0;                                                                    //!< Clone the move to get an independent copy
//    virtual std::vector<StochasticNode*>&   getDagNodes(void) = 0;                                                                  //!< Get the nodes vector
    virtual double                          perform(double& probRatio) = 0;                                                     //!< Perform the InferenceMove
    virtual void                            reject(void) = 0;                                                                   //!< Reject the InferenceMove
    
    // InferenceMove functions you should not override
//    virtual void                            addDagNode(StochasticNode* d);                                                                 //!< Add a DAG node to this InferenceMove
    double                                  getAcceptanceRatio(void) const;                                                         //!< Get acceptance ratio
    double                                  getUpdateWeight(void) const;                                                            //!< Get update weight of InferenceMove
    void                                    resetCounters(void);                                                                    //!< Reset numTried/numAccepted
    
protected:
    InferenceMove();                                                                                           //!< Default constructor
    InferenceMove(const InferenceMove& m);                                                                                                            //!< Copy constructor
    
    // parameters
    double                                  weight;
    
    // Hidden member variables
    size_t                                  numAccepted;                                                                            //!< Number of times accepted
    size_t                                  numTried;                                                                               //!< Number of times tried
    double                                  acceptanceR;
    
//    std::vector<StochasticNode*>            nodes;
        
};

#endif
