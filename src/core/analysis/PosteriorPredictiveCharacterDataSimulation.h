#ifndef PosteriorPredictiveCharacterDataSimulation_H
#define PosteriorPredictiveCharacterDataSimulation_H

#include "Cloneable.h"
#include "Model.h"
#include "RbVector.h"
#include "ModelTrace.h"

namespace RevBayesCore {
    
    /**
     * @brief Declaration of PosteriorPredictiveCharacterDataSimulation class
     *
     *
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class PosteriorPredictiveCharacterDataSimulation : public Cloneable {
        
    public:
        PosteriorPredictiveCharacterDataSimulation(const Model &m, const std::string &ft, const std::string &dir, const RbVector<ModelTrace> &t);
//        PosteriorPredictiveCharacterDataSimulation(const PosteriorPredictiveCharacterDataSimulation &m);
        virtual                                            ~PosteriorPredictiveCharacterDataSimulation(void);                                       //!< Virtual destructor
        
//        PosteriorPredictiveCharacterDataSimulation&         operator=(const PosteriorPredictiveCharacterDataSimulation &m);                         //!< Overloaded assignment operator
        
        // public methods
        PosteriorPredictiveCharacterDataSimulation*         clone(void) const;
        void                                                run(int thinning);
        
    private:
        
        Model                                               model;
        std::string                                         filetype;
        std::string                                         directory;
        RbVector<ModelTrace>                                traces;
        
        
    };
    
}


#endif
