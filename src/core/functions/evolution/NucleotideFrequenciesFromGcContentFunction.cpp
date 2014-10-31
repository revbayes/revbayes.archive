#include "NucleotideFrequenciesFromGcContentFunction.h"
#include "RbException.h"

using namespace RevBayesCore;

NucleotideFrequenciesFromGcContentFunction::NucleotideFrequenciesFromGcContentFunction(const TypedDagNode< double > *Gc) : TypedFunction< RbVector<double> >( new RbVector<double>( 4 ) ), gc( Gc ) {
    // add the lambda parameter as a parent
    addParameter( gc );
    
    update();
}


NucleotideFrequenciesFromGcContentFunction::NucleotideFrequenciesFromGcContentFunction(const NucleotideFrequenciesFromGcContentFunction &n) : TypedFunction< RbVector<double> >( n ), gc( n.gc ) {
    // no need to add parameters, happens automatically
}


NucleotideFrequenciesFromGcContentFunction::~NucleotideFrequenciesFromGcContentFunction( void ) {
    // We don't delete the parameters, because they might be used somewhere else too. The model needs to do that!
}



NucleotideFrequenciesFromGcContentFunction* NucleotideFrequenciesFromGcContentFunction::clone( void ) const {
    return new NucleotideFrequenciesFromGcContentFunction( *this );
}


void NucleotideFrequenciesFromGcContentFunction::update( void ) {
    // get the information from the arguments for reading the file
    double gc_val = gc->getValue();
    
    // check the sizes of the simplices, to make certain that they are consistent
    // with a model with nStates states    
    
    std::vector<double>& m = *value;
    
    double gcOverTwo = gc_val/2;
    double OneMinusGcOverTwo = (1 - gc_val )/2 ;
    m[0] = OneMinusGcOverTwo;
    m[1] = gcOverTwo;
    m[2] = gcOverTwo;
    m[3] = OneMinusGcOverTwo;
}



void NucleotideFrequenciesFromGcContentFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP) {
    if (oldP == gc) {
        gc = static_cast<const TypedDagNode< double >* >( newP );
    }
}


