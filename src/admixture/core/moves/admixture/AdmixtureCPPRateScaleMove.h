//
//  AdmixtureCompoundPoissonProcessRateScaleMove.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 1/30/13.
//  Copyright (c) 2013 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__AdmixtureCPPRateScaleMove__
#define __revbayes_mlandis__AdmixtureCPPRateScaleMove__

#include <ostream>
#include <set>
#include <string>

#include "SimpleMove.h"
#include "StochasticNode.h"
#include "AdmixtureTree.h"

namespace RevBayesCore {
    
    class AdmixtureCPPRateScaleMove : public SimpleMove {
        
    public:
        AdmixtureCPPRateScaleMove( StochasticNode<AdmixtureTree> *n, StochasticNode<double>* r, double hp, double l, bool tuning, double w);
//        AdmixtureCPPRateScaleMove( const AdmixtureCPPRateScaleMove& n );

        // Basic utility functions
        AdmixtureCPPRateScaleMove*      clone(void) const;                                                                  //!< Clone object
        void                            swapNode(DagNode *oldN, DagNode *newN);
        
    protected:
        const std::string&              getMoveName(void) const;                                                            //!< Get the name of the move for summary printing
        double                          performSimpleMove(void);                                                            //!< Perform move
        void                            acceptSimpleMove(void);
        void                            printParameterSummary(std::ostream &o) const;
        void                            rejectSimpleMove(void);
        void                            tune(void);
        
//        void                                            acceptMove(void);
 //       double                                          performMove(double &probRatio);
  //      void                                            printParameterSummary(std::ostream &o) const;
   //     void                                            rejectMove(void);
        
    private:
        // parameters
        double                          lambda;
        double                          cpp_prior;
        
        StochasticNode<AdmixtureTree>*  variable;
        StochasticNode<double>*         rate;
        double                          storedRate;
        
    };
    
}

#endif /* defined(__revbayes_mlandis__AdmixtureCPPRateScaleMove__) */
