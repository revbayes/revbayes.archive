#ifndef PosteriorPredictiveCharacterDataSimulation_H
#define PosteriorPredictiveCharacterDataSimulation_H

#include "Cloneable.h"
#include "Model.h"
#include "RbVector.h"
#include "ModelTrace.h"

namespace RevBayesCore {
    
    /**
     * @brief Posterior predictive simulation from a posterior sample trace.
     *
     * This class performs a posterior predictive simulation. First, we receive a trace of 
     * all stochastic variables in the model. Then, we go through the trace and pick every i-th
     * sample. The values of the samples are used to set the values of the stochastic variables
     * in the model. You need to make sure that the trace contains values for all the stocahstic
     * nodes, otherwise the stochastic nodes will keep their current values.
     * Next, we simply simulated new values for all clamped variables. The newly simulated
     * values are written into a file per variable. We also create a directory per iteration, that is,
     * the j-th posterior predictive simulation will be written into the directory sim_j.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna and Lyndon Coghill)
     * @since Version 1.0, 2015-09-16
     *
     */
    class PosteriorPredictiveCharacterDataSimulation : public Cloneable {
        
    public:
        PosteriorPredictiveCharacterDataSimulation(const Model &m, const std::string &dir, const RbVector<ModelTrace> &t);
//        PosteriorPredictiveCharacterDataSimulation(const PosteriorPredictiveCharacterDataSimulation &m);
        virtual                                            ~PosteriorPredictiveCharacterDataSimulation(void);                                       //!< Virtual destructor
        
//        PosteriorPredictiveCharacterDataSimulation&         operator=(const PosteriorPredictiveCharacterDataSimulation &m);                         //!< Overloaded assignment operator
        
        // public methods
        PosteriorPredictiveCharacterDataSimulation*         clone(void) const;
        void                                                run(int thinning);
        
    private:
        
        Model                                               model;
        std::string                                         directory;
        RbVector<ModelTrace>                                traces;
        
        
    };
    
}


#endif
