#include "BinaryState.h"

using namespace RevBayesCore;

/** Default constructor */
BinaryState::BinaryState(size_t n) : StandardState("0", "01")
{

}


/** Default constructor */
BinaryState::BinaryState(const std::string &s) : StandardState(s, "01")
{ 
}


