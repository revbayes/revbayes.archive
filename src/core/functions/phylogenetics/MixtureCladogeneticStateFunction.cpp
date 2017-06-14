#include "MixtureCladogeneticStateFunction.h"
#include "RbException.h"

#include <math.h>

using namespace RevBayesCore;

MixtureCladogeneticStateFunction::MixtureCladogeneticStateFunction(const TypedDagNode< Simplex > *mw, const TypedDagNode< RbVector<CladogeneticProbabilityMatrix> > *cp, unsigned nc, unsigned ns) : TypedFunction<CladogeneticProbabilityMatrix>( new CladogeneticProbabilityMatrix(0) ),
mixtureWeights( mw ),
cladoProbs( cp ),
numCharacters(nc),
numStates(ns),
numComponents( mw->getValue().size() )
{
    // add the lambda parameter as a parent
    addParameter( mixtureWeights );
    addParameter( cladoProbs );
    
    buildEventMap();
    
    update();
}

MixtureCladogeneticStateFunction::~MixtureCladogeneticStateFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}


void MixtureCladogeneticStateFunction::buildEventMap( void ) {
    
    std::map<std::vector<unsigned>, double> eventMapProbsMixture = this->getValue().getEventMap();
    
//    this->getValue()[it->first] = 0.0;
    for (size_t i = 0; i < numComponents; i++)
    {
        const CladogeneticProbabilityMatrix& cp = cladoProbs->getValue()[i];
        std::map<std::vector<unsigned>, double> emp = cp.getEventMap();
        std::map<std::vector<unsigned>, double>::iterator it;
        for (it = emp.begin(); it != emp.end(); it++)
        {
            eventMapProbsMixture[it->first] = 0.0;
        }
    }
    
    this->getValue().setEventMap(eventMapProbsMixture);
}



MixtureCladogeneticStateFunction* MixtureCladogeneticStateFunction::clone( void ) const
{
    return new MixtureCladogeneticStateFunction( *this );
}

std::map< std::vector<unsigned>, double >  MixtureCladogeneticStateFunction::getEventMap(double t)
{
    return this->getValue().getEventMap();
}

const std::map< std::vector<unsigned>, double >&  MixtureCladogeneticStateFunction::getEventMap(double t) const
{
    return this->getValue().getEventMap();
}

void MixtureCladogeneticStateFunction::update( void )
{
    
    // get the information from the arguments for reading the file
    const std::vector<double>& mp = mixtureWeights->getValue();

    std::map<std::vector<unsigned>, double> eventMapProbsMixture = this->getValue().getEventMap();
    
    //    this->getValue()[it->first] = 0.0;
    for (size_t i = 0; i < numComponents; i++)
    {
        const CladogeneticProbabilityMatrix& cp = cladoProbs->getValue()[i];
        std::map<std::vector<unsigned>, double> emp = cp.getEventMap();
        std::map<std::vector<unsigned>, double>::iterator it;
        for (it = emp.begin(); it != emp.end(); it++)
        {
            eventMapProbsMixture[it->first] = mp[i] * it->second;
//            std::vector<unsigned> v = it->first;
//            std::cout << i << " : " << v[0] << "," << v[1] << "," << v[2] << " = " << it->second << "\n";
        }
//        std::cout << "\n";
    }
    
    this->getValue().setEventMap(eventMapProbsMixture);
    
//#if 0
//    std::vector<double> probs(numEventTypes, 0.0);
//    for (size_t i = 0; i < eventTypes.size(); i++)
//    {
//        probs[ eventStringToStateMap[eventTypes[i]] ] = ep[i];
//    }
//    
//    // normalize tx probs
//    std::vector<double> z( numIntStates, 0.0 );
//    for (size_t i = 0; i < numIntStates; i++)
//    {
//        for (size_t j = 1; j < numEventTypes; j++)
//        {
//            //            std::cout << i << " " << j << " " << eventMapCounts[i][j] << " " << ep[j-1] << "\n";
//            //            size_t k = eventStringToStateMap[eventTypes[j]];
//            z[i] += eventMapCounts[i][j] * probs[j];
//        }
//        //        std::cout << "weight-sum[" << bitsToString(bits[i]) << "] = " << z[i] << "\n";
//    }
//    
//    std::map<std::vector<unsigned>, unsigned>::iterator it;
//    for (it = eventMapTypes.begin(); it != eventMapTypes.end(); it++)
//    {
//        const std::vector<unsigned>& idx = it->first;
//        double v = 1.0;
//        if (it->second != BiogeographicCladoEvent::SYMPATRY_NARROW) {
//            if (eventProbsAsWeightedAverages) {
//                v = probs[ it->second ] / z[ idx[0] ];
//                //                std::cout << bitsToString(bits[idx[0]]) << "->" << bitsToString(bits[idx[1]]) << "," << bitsToString(bits[idx[2]]) << "\t" << v << " = " << ep[it->second - 1] << " / " << z[ idx[0] ] << "\n";
//            }
//            else {
//                v = probs[ it->second ] / eventMapCounts[ idx[0] ][ it->second ];
//                //                            std::cout << idx[0] << "->" << idx[1] << "," << idx[2] << "\t" << ep[it->second - 1] << " " << eventMapCounts[ idx[0] ][ it->second ] << "\n";
//                
//            }
//            
//        }
//        
//        (*value)[ idx[0] ][ numIntStates * idx[1] + idx[2] ] = v;
//        eventMapProbs[ idx ] = v;
//    }
//    
//#endif
    return;
}



void MixtureCladogeneticStateFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
    
    if (oldP == mixtureWeights)
    {
        mixtureWeights = static_cast<const TypedDagNode< Simplex >* >( newP );
    }
    else if (oldP == cladoProbs)
    {
        cladoProbs = static_cast<const TypedDagNode< RbVector<CladogeneticProbabilityMatrix> >* >( newP );
    }
    
}
