#ifndef PowerPosteriorAnalysis_H
#define PowerPosteriorAnalysis_H

#include "Cloneable.h"
#include "Model.h"
#include "Move.h"
#include "RbVector.h"

namespace RevBayesCore {
    
    /**
     * @brief Power posterior analysis class.
     *
     * A power posterior analysis runs an analysis for a vector of powers
     * where the likelihood during each analysis run is raised to the given power.
     * The likelihood values and the current powers are stored in a file.
     *
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since Version 1.0, 2012-06-17
     *
     */
    class PowerPosteriorAnalysis : public Cloneable {
        
    public:
        PowerPosteriorAnalysis(const Model& m, const RbVector<Move> &moves, const std::string &fn);
        virtual                                            ~PowerPosteriorAnalysis(void);                                                          //!< Virtual destructor
        
        // public methods
        PowerPosteriorAnalysis*                             clone(void) const;
        void                                                run(size_t g);
        
        void                                                setPowers(const std::vector<double> &p);
        void                                                setSampleFreq(size_t sf);
        
    private:
        
        // members
        std::string                                         filename;
        unsigned long                                       generation;
        Model                                               model;
        RbVector<Monitor>                                   monitors;
        RbVector<Move>                                      moves;
        std::vector<double>                                 powers;
        size_t                                              sampleFreq;
//        MoveSchedule*                                       schedule;
//        std::string                                         scheduleType;                                                                           //!< Type of move schedule to be used
        
    };
    
}

#endif
