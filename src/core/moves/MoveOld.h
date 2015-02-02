#ifndef MoveOld_H
#define MoveOld_H

#include "AbstractOldMove.h"

#include <ostream>
#include <set>
#include <vector>

namespace RevBayesCore {
    
    class DagNode;

    class MoveOld : public AbstractOldMove {
    
    public:
        virtual                                                ~MoveOld(void);                                                                             //!< Destructor
         
        // pure virtual public methods
        virtual MoveOld*                                        clone(void) const = 0;
        virtual const std::string&                              getMoveName(void) const = 0;                                                            //!< Get the name of the move for summary printing
    
        // functions you should not override
        void                                                    accept(void);                                                                           //!< Accept the InferenceMove
        void                                                    autoTune(void);                                                                         //!< automatically tune this move
        double                                                  getAcceptanceRatio(void) const;                                                         //!< Get acceptance ratio
        virtual const std::set<DagNode*>&                       getDagNodes(void) const;                                                                //!< Get the nodes vector
        double                                                  getUpdateWeight(void) const;                                                            //!< Get update weight of InferenceMove
        virtual bool                                            isActive(unsigned long g) const;                                                        //!< Is this move active at the iteration?
        virtual bool                                            isGibbs(void) const;                                                                    //!< Is this a Gibbs move?
        double                                                  performOld(double& probRatio);                                                             //!< Perform the InferenceMove
        void                                                    performGibbs(void);                                                                     //!< Perform a Gibbs proposal
        void                                                    printSummary(std::ostream &o) const;                                                    //!< Print the move summary
        void                                                    reject(void);                                                                           //!< Reject the InferenceMove
        void                                                    resetCounters(void);                                                                    //!< Reset numTried/numAccepted
        
        // methods you may want to overwrite
        virtual void                                            swapNode(DagNode *oldN, DagNode *newN);

    protected:
                                                                MoveOld( DagNode* n, double w, bool autoTune = false);                                      //!< Constructor
                                                                MoveOld( std::vector<DagNode*> n, double w, bool autoTune = false);
    
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
std::ostream&                       operator<<(std::ostream& o, const RevBayesCore::MoveOld& x);


#endif
