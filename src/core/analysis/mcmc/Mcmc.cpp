#include "DagNode.h"
#include "FileMonitor.h"
#include "Mcmc.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"
#include "SingleRandomMoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "ExtendedNewickTreeMonitor.h"
#include "ExtendedNewickAdmixtureTreeMonitor.h"
#include "AdmixtureBipartitionMonitor.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <typeinfo>

using namespace RevBayesCore;


/**
 * Constructor. We create an independent copy of the model and thus of all DAG nodes.
 * Someone might have wanted to run another MCMC with different settings on the same model.
 * Thus we also create our own copies of the monitors and moves.
 *
 * \param[in]    m    The model containing all DAG nodes.
 * \param[in]    mvs  The vector of moves.
 * \param[in]    mons The vector of monitors.
 */
Mcmc::Mcmc(const Model& m, const RbVector<Move> &mvs, const RbVector<Monitor> &mons) : MonteCarloSampler(m,mvs,mons)
{
    
}



/**
 * Destructor. Nothing to do here
 */
Mcmc::~Mcmc(void) {
    
    // all the magic happens in the base class.
    
}


Mcmc* Mcmc::clone( void ) const
{
    
    return new Mcmc( *this );
}


void Mcmc::monitor(unsigned long g)
{
    
    // Monitor
    for (size_t i = 0; i < monitors.size(); i++) 
    {
        monitors[i].monitor( g );
    }
    
}



unsigned long Mcmc::nextCycle(bool advanceCycle) {
    
#ifdef DEBUG_MCMC
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
#endif
    
    size_t proposals = size_t( round( schedule->getNumberMovesPerIteration() ) );
    for (size_t i=0; i<proposals; i++) 
    {
        
#ifdef DEBUG_MCMC
        double oldLnProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            oldLnProb += (*it)->getLnProbability();
        }
#endif

        // Get the move
        Move& theMove = schedule->nextMove( generation );

#ifdef DEBUG_MCMC
#ifdef DEBUG_MCMC_DETAILS
        std::cerr << "\nPerforming move " << theMove.getMoveName() << " on " << ( *theMove.getDagNodes().begin() )->getName() << std::endl;

        std::cerr << "\nValues before move " << std::endl << std::endl;

        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            if ( !(*it)->isConstant() )
            {
                std::cerr << (*it)->getName() << " <" << (*it) << "> :" << std::endl;
                (*it)->printValue( std::cerr, ", " );
                std::cerr << std::endl << std::endl;
            }
        }
#endif
#endif

        // Perform the move
        theMove.perform( chainHeat, false);
        
#ifdef DEBUG_MCMC
#ifdef DEBUG_MCMC_DETAILS
        std::cerr << "\nValues after move " << std::endl << std::endl;
        
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            if ( !(*it)->isConstant() )
            {
                std::cerr << (*it)->getName() << " <" << (*it) << "> :" << std::endl;
                (*it)->printValue( std::cerr, ", " );
                std::cerr << std::endl << std::endl;
            }
        }

        getchar();

        std::cerr << std::endl << "With shortcuts" << std::endl;
#endif
        double lnProb = 0.0;
        double lnLikelihoodProb = 0.0;
        double lnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            double pr = (*it)->getLnProbability();
            lnProb += pr;
            
#ifdef DEBUG_MCMC_DETAILS
            if ( (*it)->isStochastic() )
                std::cerr << (*it)->getName() << "<" << (*it) << "> returned lnprob = " << pr << std::endl;
#endif
            if ( (*it)->isClamped() )
            {
                lnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                lnPriorProb += (*it)->getLnProbability();
            }
        }

#ifdef DEBUG_MCMC_DETAILS
        std::cerr << std::endl << "After touching everything" << std::endl;
#endif
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->touch();
        }
        double touchedLnProb = 0.0;
        double touchedLnLikelihoodProb = 0.0;
        double touchedLnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            double pr = (*it)->getLnProbability();
            touchedLnProb += pr;
            
#ifdef DEBUG_MCMC_DETAILS
            if ( (*it)->isStochastic() )
                std::cerr << (*it)->getName() << "<" << (*it) << "> returned lnprob = " << pr << std::endl;
#endif
            if ( (*it)->isClamped() )
            {
                touchedLnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                touchedLnPriorProb += (*it)->getLnProbability();
            }
        }
        
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->keep();
            (*it)->touch();
        }
        double touchedAgainLnProb = 0.0;
        double touchedAgainLnLikelihoodProb = 0.0;
        double touchedAgainLnPriorProb = 0.0;
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            touchedAgainLnProb += (*it)->getLnProbability();
            if ( (*it)->isClamped() )
            {
                touchedAgainLnLikelihoodProb += (*it)->getLnProbability();
            }
            else
            {
                touchedAgainLnPriorProb += (*it)->getLnProbability();
            }
        }
        // Keep everything to be ready for next cycle
        for (std::vector<DagNode*>::iterator it = dagNodes.begin(); it != dagNodes.end(); ++it)
        {
            (*it)->keep();
        }
        
        if ( fabs(lnProb - touchedLnProb) > 1E-8 )
        {
            std::cout << "Probability        shortcut\t\t-\t\ttouched all\t\t-\t\ttouched all again" << std::endl;
            std::cout << "lnPriorProb      = " << lnPriorProb << "\t\t-\t\t" << touchedLnPriorProb << "\t\t-\t\t" << touchedAgainLnPriorProb << std::endl;
            std::cout << "lnLikelihoodProb = " << lnLikelihoodProb << "\t\t-\t\t" << touchedLnLikelihoodProb << "\t\t-\t\t" << touchedAgainLnLikelihoodProb << std::endl;
            std::cout << "lnProb           = " << lnProb << "\t\t-\t\t" << touchedLnProb << "\t\t-\t\t" << touchedAgainLnProb << std::endl;
            std::cout << "Failure occurred after move:\t" << theMove.getMoveName() << std::endl;

//            throw RbException("Error in MCMC probability computation.");
        }
#endif
        
    }
    
    
    // advance gen cycle if needed (i.e. run()==true, burnin()==false)
    if (advanceCycle)
    {
        generation++;
    }
    
    // gen number used for p(MC)^3
    return generation;
}


void Mcmc::run(size_t kIterations) {
    
    /* Let user know what we are doing */
    if ( generation == 0 )
        std::cout << "Running MCMC simulation for " << kIterations << " iterations" << std::endl;
    else
        std::cout << "Appending " << kIterations << " iterations to previous MCMC simulation of " << generation << " iterations" << std::endl;
    
    if ( scheduleType == "single" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves, with a" << std::endl;
        std::cout << "single move picked randomly per iteration" << std::endl;
    }
    else if ( scheduleType == "random" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a random" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    else if ( scheduleType == "sequential" )
    {
        std::cout << "The simulator uses " << moves.size() << " different moves in a sequential" << std::endl;
        std::cout << "move schedule with " << schedule->getNumberMovesPerIteration() << " moves per iteration" << std::endl;
    }
    
    // Initialize objects used in run
    initializeChain();
    initializeMonitors();
    
    if ( generation == 0 )
    {
        // Monitor
        startMonitors();
        monitor(0);
    }
    
    // reset the counters for the move schedules
    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {
        it->resetCounters();
    }
    
    // Run the chain
    for (int k=1; k<=kIterations; k++) 
    {
        nextCycle(true);
        
        // Monitor
        monitor(generation);
                        
    }
    
    
}

