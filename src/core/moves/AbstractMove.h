#ifndef AbstractMove_H
#define AbstractMove_H

#include "Cloneable.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
        
    class DagNode;
    
    /**
     * Base class for all move within an MCMC (and all other variants). 
     *
     * The base class of all moves only provides the interface for the call to propose a new move.
     * All specifics are implemented in the derived classes.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2014-03-26, version 1.0
     *
     */
    class AbstractMove : public Cloneable {
        
    public:
        virtual                                                ~AbstractMove(void);                                                                     //!< Destructor
        
        // pure virtual public methods
        virtual AbstractMove*                                   clone(void) const = 0;
        virtual const std::string&                              getMoveName(void) const = 0;                                                            //!< Get the name of the move for summary printing
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN) = 0;                                             //!< Swap the pointers to the variable on which the move works on.
        virtual void                                            perform(void) = 0;                                                                      //!< Perform the move.
        virtual void                                            printSummary(std::ostream &o) const = 0;                                                //!< Print the move summary
        
        // functions you should not override
//        void                                                    autoTune(void);                                                                         //!< automatically tune this move
        double                                                  getUpdateWeight(void) const;                                                            //!< Get update weight of InferenceMove
//        virtual bool                                            isActive(unsigned long g) const;                                                        //!< Is this move active at the iteration?
        
        // methods you may want to overwrite
        
    protected:
        AbstractMove(double w, bool autoTune = false);                                      //!< Constructor
        
        
        // method you might want to override
        virtual void                                            tune(void) = 0;                                                                         //!< Specific tuning of the move
        
        // parameters
        double                                                  weight;
        
        // Hidden member variables
        bool                                                    autoTuning;
        unsigned int                                            numTried;                                                                               //!< Number of times tried
                
    };
    
}



// Global functions using the class
std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::AbstractMove& x);   


#endif
