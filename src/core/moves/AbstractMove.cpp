#include "AbstractMove.h"
#include "DagNode.h"
#include "RbException.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>


using namespace RevBayesCore;

AbstractMove::AbstractMove( double w, bool t ) : 
    weight( w ), 
    autoTuning( t ) 
{
    
}


AbstractMove::~AbstractMove( void )
{
    
}


//double Move::getAcceptanceRatio( void ) const {
//    return numTried / double( numAccepted );
//}



double AbstractMove::getUpdateWeight( void ) const {
    return weight;
}


//bool AbstractMove::isActive( unsigned long g ) const 
//{
//    
//    return true;
//}



//void Move::resetCounters( void ) {
//    numTried = 0;
//    numAccepted = 0;
//}




std::ostream& operator<<(std::ostream& o, const AbstractMove& x) {
    o << "Move";
    
    return o;
}

