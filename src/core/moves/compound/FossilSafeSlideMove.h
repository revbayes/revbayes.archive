//
//  FossilSafeSlideMove.h
//  revbayes-proj
//
//  Created by Michael Landis on 2/9/15.
//  Copyright (c) 2015 Michael Landis. All rights reserved.
//

#ifndef FossilSafeSlideMove_H
#define FossilSafeSlideMove_H

#include <map>
#include <ostream>
#include <set>
#include <string>

#include "CompoundMove.h"
#include "TimeTree.h"

namespace RevBayesCore {
    
    class FossilSafeSlideMove : public MoveOld {
        public:
            FossilSafeSlideMove(std::vector<DagNode*> n, std::vector<double> ta, double lambda, double w, bool autoTune = false);     //!< constructor
            virtual                                 ~FossilSafeSlideMove(void);                               //!< Destructor
            
            // functions you have to override
            FossilSafeSlideMove*                    clone(void) const;                                        //!< Clone the MoveSimple to get an independent copy
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

#endif /* defned(FossilSafeSlideMove_H) */
