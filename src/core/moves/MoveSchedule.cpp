#include "MoveSchedule.h"

using namespace RevBayesCore;

MoveSchedule::MoveSchedule(const std::vector<Move*> &m ) : moves( m ) {
    
}


MoveSchedule::~MoveSchedule() {
    // we own nothing
}
