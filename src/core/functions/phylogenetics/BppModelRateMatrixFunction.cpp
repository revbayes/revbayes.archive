#include "BppModelRateMatrixFunction.h"
#include "RbException.h"

#include <Bpp/Numeric/ParameterList.h>

using namespace RevBayesCore;
using namespace std;


BppModelRateMatrixFunction::BppModelRateMatrixFunction(std::map<std::string, const TypedDagNode< double > *>& sTDN, bpp::SubstitutionModel* pSM ) : TypedFunction<RateMatrix>( new BppRateMatrix( pSM ))
{
  std::map<std::string, const TypedDagNode<double>* >::const_iterator it;

  for (it=sTDN.begin();it!=sTDN.end();it++)
  {
    this->addParameter(it->second);
    static_cast< BppRateMatrix*>(value)->setParameterValue(it->first,it->second->getValue());
  }
  
  update();
}


BppModelRateMatrixFunction::BppModelRateMatrixFunction(const BppModelRateMatrixFunction &n) : TypedFunction<RateMatrix>( n )
{
}


BppModelRateMatrixFunction::~BppModelRateMatrixFunction( void )
{
}



BppModelRateMatrixFunction* BppModelRateMatrixFunction::clone( void ) const
{
  return new BppModelRateMatrixFunction( *this );
}


void BppModelRateMatrixFunction::update( void )
{
  value->updateMatrix();
}



void BppModelRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
  static_cast< BppRateMatrix*>(value)->setParameterValue(oldP->getName(), (static_cast<const TypedDagNode<double>*>(newP))->getValue());
}


