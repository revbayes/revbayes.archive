//
//  FossilSafeScaleMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/9/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef FossilSafeScaleMove_H
#define FossilSafeScaleMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "TimeTree.h"

namespace RevBayesCore {
    
    class FossilSafeScaleMove : public MoveOld {
    public:
        FossilSafeScaleMove(std::vector<DagNode*> n, std::vector<double> ta, double lambda, double w, bool autoTune = false);     //!< constructor
        virtual                                 ~FossilSafeScaleMove(void);                               //!< Destructor
        
        // functions you have to override
        FossilSafeScaleMove*                    clone(void) const;                                        //!< Clone the MoveSimple to get an independent copy
        const std::string&                      getMoveName(void) const;                                  //!< Get the name of
        
        // methods you may want to overwrite
        void                                    swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        
        void                                    acceptMove(void);                                         //!< Accept the InferenceMoveSimple
        double                                  performMove(double& probRatio);                           //!< Perform the InferenceMoveSimple
        void                                    rejectMove(void);                                         //!< Reject the InferenceMoveSimple
        virtual void                            tune(void);                                               //!< Specific tuning
        
    private:
        double                                  doMove(void);
        
        ContinuousStochasticNode*               scaler;
        StochasticNode<TimeTree>*               tree;
        std::vector<double>                     tipAges;
        std::vector<size_t>                     fossilIdx;
        
        bool                                    changed;
        double                                  lambda;
        double                                  storedValue;
        std::vector<double>                     storedTipAges;
        double                                  tol;
        
    };
};

#endif /* defned(FossilSafeScaleMove_H) */
