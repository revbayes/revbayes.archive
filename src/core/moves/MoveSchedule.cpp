#include "MoveSchedule.h"

using namespace RevBayesCore;

MoveSchedule::MoveSchedule(const RbVector<Move> &m ) : moves( m )
{
    
}


MoveSchedule::~MoveSchedule() {
    // we own nothing
}
