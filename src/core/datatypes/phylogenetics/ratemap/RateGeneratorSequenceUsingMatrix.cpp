#include "MatrixReal.h"
#include "RateGeneratorSequenceUsingMatrix.h"
#include "RateMatrix.h"
#include "RateMatrix_JC.h"
#include "RbVector.h"
#include "RbException.h"
#include "RbMathMatrix.h"
#include "TransitionProbabilityMatrix.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace RevBayesCore;

/** Construct rate matrix with n states */
RateGeneratorSequenceUsingMatrix::RateGeneratorSequenceUsingMatrix(size_t ns, size_t nc) : RateGeneratorSequence(ns, nc)
{

    rateMatrix = new RateMatrix_JC(ns);

}



/** Copy constructor */
RateGeneratorSequenceUsingMatrix::RateGeneratorSequenceUsingMatrix(const RateGeneratorSequenceUsingMatrix& m) : RateGeneratorSequence( m )
{

    rateMatrix = m.rateMatrix->clone();

}


/** Destructor */
RateGeneratorSequenceUsingMatrix::~RateGeneratorSequenceUsingMatrix(void)
{

    delete rateMatrix;

}


RateGeneratorSequenceUsingMatrix& RateGeneratorSequenceUsingMatrix::operator=(const RateGeneratorSequenceUsingMatrix &r)
{

    RateGeneratorSequence::operator=( r );

    if (this != &r)
    {

        delete rateMatrix;

        rateMatrix = r.rateMatrix->clone();

    }

    return *this;
}



std::ostream& RevBayesCore::operator<<(std::ostream& o, const RateGeneratorSequenceUsingMatrix& x)
{
    std::streamsize previousPrecision = o.precision();
    std::ios_base::fmtflags previousFlags = o.flags();

    o.setf(previousFlags);
    o.precision(previousPrecision);

    return o;
}


void RateGeneratorSequenceUsingMatrix::calculateTransitionProbabilities(TransitionProbabilityMatrix &P, double age) const
{
    const RateGenerator* rm = rateMatrix;

    rm->calculateTransitionProbabilities(age, 0, 1.0, P);
}



void RateGeneratorSequenceUsingMatrix::calculateTransitionProbabilities(TransitionProbabilityMatrix &P, double startAge, double endAge, double rate) const
{
    const RateGenerator* rm = rateMatrix;

    rm->calculateTransitionProbabilities(startAge, endAge, rate, P);
}


RateGeneratorSequenceUsingMatrix* RateGeneratorSequenceUsingMatrix::clone(void) const
{
    return new RateGeneratorSequenceUsingMatrix( *this );
}


double RateGeneratorSequenceUsingMatrix::getRate(size_t from, size_t to, double rate, double age) const
{

    const RateGenerator* rm = rateMatrix;

    double r = rm->getRate(from, to, age, rate);

    return r;

}


double RateGeneratorSequenceUsingMatrix::getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, double rate, double age) const
{
    size_t from_state = from[ to->getSiteIndex() ]->getState();
    size_t to_state = to->getState();

    const RateGenerator* rm = rateMatrix;

    double r = rm->getRate(from_state, to_state, age, rate);

    return r;

}



double RateGeneratorSequenceUsingMatrix::getRate(std::vector<CharacterEvent*> from, CharacterEvent* to, unsigned* counts, double rate, double age) const
{
    size_t from_state = from[ to->getSiteIndex() ]->getState();
    size_t to_state = to->getState();

    const RateGenerator* rm = rateMatrix;

    double r = rm->getRate(from_state, to_state, age, rate);

    return r;

}

double RateGeneratorSequenceUsingMatrix::getSiteRate(CharacterEvent* from, CharacterEvent* to, double r, double age) const
{

    double rate = 0.0;
    const RateGenerator* rm = rateMatrix;

    rate = rm->getRate(from->getState(), to->getState(), age, r);

    return rate;
}

double RateGeneratorSequenceUsingMatrix::getSiteRate(size_t from, size_t to, size_t charIdx, double r, double age) const
{

    double rate = 0.0;
    const RateGenerator* rm = rateMatrix;

    rate = rm->getRate(from, to, rate, r);

    return rate;
}

double RateGeneratorSequenceUsingMatrix::getSumOfRates(std::vector<CharacterEvent*> from, std::vector<size_t> counts, double rate, double age) const
{

    // get characters in each state
    if (counts.size()==0)
    {
        counts = std::vector<size_t>(this->numStates, 0);
        for (size_t i = 0; i < from.size(); i++)
        {
            counts[ from[i]->getState() ] += 1;
        }

    }

    // get rate matrix
    const RateGenerator* rm = rateMatrix;

    // get the rate of leaving the sequence-state
    double sum = 0.0;
    for (size_t i = 0; i < numStates; i++)
    {
        sum += -rm->getRate(i, i, age, 1.0) * counts[i];
    }

    // apply rate for branch
    sum *= rate;

    return sum;
}

double RateGeneratorSequenceUsingMatrix::getSumOfRates(std::vector<CharacterEvent*> from, double rate, double age) const
{

    std::vector<size_t> counts = std::vector<size_t>(this->numStates, 0);
    
    for (size_t i = 0; i < from.size(); i++)
    {
        counts[ from[i]->getState() ] += 1;
    }

    return getSumOfRates( from, counts, rate, age);
}


void RateGeneratorSequenceUsingMatrix::setRateMatrix(const RateGenerator* r)
{

    if (r != rateMatrix)
    {
        delete rateMatrix;
        rateMatrix = r->clone();
    }

}


void RateGeneratorSequenceUsingMatrix::updateMap(void)
{
    if (needsUpdate)
    {
        ; // do nothing ...

        needsUpdate = false;
    }
}
