#include "DagNode.h"
#include "FileMonitor.h"
#include "PowerPosteriorAnalysis.h"
#include "MoveSchedule.h"
#include "RandomMoveSchedule.h"
#include "RandomNumberFactory.h"
#include "RandomNumberGenerator.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbFileManager.h"
#include "RbOptions.h"
#include "SequenctialMoveSchedule.h"

#include <cmath>
#include <typeinfo>

using namespace RevBayesCore;

PowerPosteriorAnalysis::PowerPosteriorAnalysis(const Model& m, const RbVector<Move> &mvs, const std::string &fn) : Cloneable( ),
    filename( fn ),
    model( m ),
    powers(),
    sampleFreq( 100 )
{
    
}


PowerPosteriorAnalysis::~PowerPosteriorAnalysis(void)
{
    
}



PowerPosteriorAnalysis* PowerPosteriorAnalysis::clone( void ) const
{
    
    return new PowerPosteriorAnalysis( *this );
}


void PowerPosteriorAnalysis::run(size_t gen)
{
    
    // create the directory if necessary
    RbFileManager f = RbFileManager(filename);
    f.createDirectoryForFile();
    
    std::fstream outStream;
    outStream.open( filename.c_str(), std::fstream::out);
    outStream << "state\t" << "power\t" << "likelihood" << std::endl;
    
    std::cout << "Running power posterior ..." << std::endl;
    
    std::vector<DagNode *>& dagNodes = model.getDagNodes();
    
    // reset the counters for the move schedules
    for (RbIterator<Move> it = moves.begin(); it != moves.end(); ++it)
    {
        it->resetCounters();
    }
    
    size_t burnin = size_t( ceil( 0.25*gen ) );
    
    size_t printInterval = size_t( round( fmax(1,gen/20.0) ) );
    size_t digits = size_t( ceil( log10( powers.size() ) ) );
    
    /* Run the chain */
    RandomMoveSchedule schedule = RandomMoveSchedule(&moves);
    for (size_t i = 0; i < powers.size(); ++i)
    {
        double p = powers[i];
        std::cout << "Step ";
        for (size_t d = size_t( ceil( log10( i+1.1 ) ) ); d < digits; d++ )
        {
            std::cout << " ";
        }
        std::cout << (i+1) << " / " << powers.size();
        std::cout << "\t\t";
        
        for (size_t k=1; k<=gen; k++)
        {
            
            if ( k % printInterval == 0 )
            {
                std::cout << "**";
                std::cout.flush();
            }
            
            size_t proposals = size_t( round( schedule.getNumberMovesPerIteration() ) );
            for (size_t j=0; j<proposals; j++)
            {
                /* Get the move */
                Move& theMove = schedule.nextMove(k);
                
                theMove.perform( p, true );
                
            }
            
            // sample the likelihood
            if ( k > burnin && k % sampleFreq == 0 )
            {
                // compute the joint likelihood
                double likelihood = 0.0;
                for (std::vector<DagNode*>::const_iterator n = dagNodes.begin(); n != dagNodes.end(); ++n)
                {
                    if ( (*n)->isClamped() )
                    {
                        likelihood += (*n)->getLnProbability();
                    }
                }
                
                outStream << k << "\t" << p << "\t" << likelihood << std::endl;
            }
            
        }
        
        std::cout << std::endl;
        
    }
    
    outStream.close();
    
    
}


void PowerPosteriorAnalysis::setPowers(const std::vector<double> &p)
{
    powers = p;
}


void PowerPosteriorAnalysis::setSampleFreq(size_t sf)
{
    sampleFreq = sf;
}

