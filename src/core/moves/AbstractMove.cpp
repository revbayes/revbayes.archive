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



void AbstractMove::printSummary( std::ostream &o ) const {
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();
    
    o << std::fixed;
    o << std::setprecision(4);
    
    // print the name
    const std::string &n = getMoveName();
    size_t spaces = 40 - (n.length() > 40 ? 40 : n.length());
    o << n;
    for (size_t i = 0; i < spaces; ++i) {
        o << " ";
    }
    o << " ";
    
//    // print the DagNode name
//    const std::string &dn_name = (*nodes.begin())->getName();
//    spaces = 20 - (dn_name.length() > 20 ? 20 : dn_name.length());
//    o << dn_name;
//    for (size_t i = 0; i < spaces; ++i) {
//        o << " ";
//    }
//    o << " ";
    
    // print the weight
    int w_length = 4 - (int)log10(weight);
    for (int i = 0; i < w_length; ++i) {
        o << " ";
    }
    o << weight;
    o << " ";
    
    // print the number of tries
    int t_length = 9 - (int)log10(numTried);
    for (int i = 0; i < t_length; ++i) {
        o << " ";
    }
    o << numTried;
    o << " ";
    
    // print the number of accepted
    int a_length = 9;
    if (numAccepted > 0) a_length -= (int)log10(numAccepted);
    
    for (int i = 0; i < a_length; ++i) {
        o << " ";
    }
    o << numAccepted;
    o << " ";
    
    // print the acceptance ratio
    double ratio = numAccepted / (double)numTried;
    if (numTried == 0) ratio = 0;
    int r_length = 5;
    
    for (int i = 0; i < r_length; ++i) {
        o << " ";
    }
    o << ratio;
    o << " ";
    
//    printParameterSummary( o );
    
    o << std::endl;
    
    o.setf(previousFlags);
    o.precision(previousPrecision);
    
    
}


//void Move::resetCounters( void ) {
//    numTried = 0;
//    numAccepted = 0;
//}




std::ostream& operator<<(std::ostream& o, const AbstractMove& x) {
    o << "Move";
    
    return o;
}

