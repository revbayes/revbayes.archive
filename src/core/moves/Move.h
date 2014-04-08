/**
 * @file
 * This file contains the declaration of Move, which is the abstract
 * base class for moves used in the mcmc inference.
 *
 * @brief Declaration of the base class Move
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2012-06-19, version 1.0
 *
 * $Id$
 */

#ifndef Move_H
#define Move_H

#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
    
    class DagNode;

    class Move : public Cloneable {
    
    public:
        virtual                                                ~Move(void);                                                                             //!< Destructor
         
        // pure virtual public methods
        virtual Move*                                           clone(void) const = 0;
        virtual const std::string&                              getMoveName(void) const = 0;                                                            //!< Get the name of the move for summary printing
    
        // functions you should not override
        void                                                    accept(void);                                                                           //!< Accept the InferenceMove
        void                                                    autoTune(void);                                                                         //!< automatically tune this move
        double                                                  getAcceptanceRatio(void) const;                                                         //!< Get acceptance ratio
        const std::set<DagNode*>&                               getDagNodes(void) const;                                                                //!< Get the nodes vector
        double                                                  getUpdateWeight(void) const;                                                            //!< Get update weight of InferenceMove
        virtual bool                                            isActive(unsigned long g) const;                                                        //!< Is this move active at the iteration?
        virtual bool                                            isGibbs(void) const;                                                                    //!< Is this a Gibbs move?
        double                                                  perform(double& probRatio);                                                             //!< Perform the InferenceMove
        void                                                    performGibbs(void);                                                                     //!< Perform a Gibbs proposal
        void                                                    printSummary(std::ostream &o) const;                                                    //!< Print the move summary
        void                                                    reject(void);                                                                           //!< Reject the InferenceMove
        void                                                    resetCounters(void);                                                                    //!< Reset numTried/numAccepted
        
        // methods you may want to overwrite
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN);

    protected:
                                                                Move(DagNode* n, double w, bool autoTune = false);                                      //!< Constructor
                                                                Move( std::vector<DagNode*> n, double w, bool autoTune = false);
    
        // function you have to implement
        virtual void                                            acceptMove(void) = 0;
        virtual void                                            performGibbsMove(void);
        virtual double                                          performMove(double &probRatio) = 0;
        virtual void                                            printParameterSummary(std::ostream &o) const;
        virtual void                                            rejectMove(void) = 0;
    
        // method you might want to override
        virtual void                                            tune(void);                                                                             //!< Specific tuning of the move
        
        // parameters
        double                                                  weight;
    
        // Hidden member variables
       
        unsigned int                                            numAccepted;                                                                            //!< Number of times accepted
        unsigned int                                            numTried;                                                                               //!< Number of times tried
         bool                                                    autoTuning;
    
        std::set<DagNode*>                                      nodes;
        
    };
    
}



// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::Move& x);   


#endif
