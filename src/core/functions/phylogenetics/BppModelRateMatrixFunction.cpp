#include "BppModelRateMatrixFunction.h"
#include "RbException.h"

#include <Bpp/Numeric/ParameterList.h>

using namespace RevBayesCore;
using namespace std;


BppModelRateMatrixFunction::BppModelRateMatrixFunction(std::map<std::string, const TypedDagNode< double > *>& sTDN, bpp::SubstitutionModel* pSM ) : TypedFunction<RateMatrix>( new BppRateMatrix( pSM )), _mN2D(), _mD2N()
{
  std::map<std::string, const TypedDagNode<double>* >::const_iterator it;

  for (it=sTDN.begin();it!=sTDN.end();it++){
    _mN2D[it->first]=it->second;
    _mD2N[it->second]=it->first;
    
    Function::addParameter(it->second);
  }
  
  update();
}

void BppModelRateMatrixFunction::addParameter(const DagNode *p)
{
  const TypedDagNode<double>* q=dynamic_cast<const TypedDagNode< double >* >(p);
  
  if (q){
    
    _mN2D[p->getName()]=q;
    
    _mD2N[q]=p->getName();
  
    Function::addParameter(p);
  }
}

void BppModelRateMatrixFunction::removeParameter(const DagNode *p)
{
  Function::removeParameter(p);
  
  std::map<const TypedDagNode<double>*, std::string>::iterator it=_mD2N.find(dynamic_cast<const TypedDagNode<double>* >(p));
  if (it!=_mD2N.end())
  {
    _mN2D.erase(_mN2D.find(it->second));
    _mD2N.erase(it);
  }
  
}

BppModelRateMatrixFunction::BppModelRateMatrixFunction(const BppModelRateMatrixFunction &n) : TypedFunction<RateMatrix>( n ), _mN2D(n._mN2D), _mD2N(n._mD2N)
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
  std::map<std::string, const TypedDagNode<double>* >::const_iterator it;

  for (it=_mN2D.begin();it!=_mN2D.end();it++)
    static_cast< BppRateMatrix*>(value)->setParameterValue(it->first,it->second->getValue());
  
  value->updateMatrix();
}



void BppModelRateMatrixFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{
  const TypedDagNode<double>* r=dynamic_cast<const TypedDagNode< double >* >(oldP);
  const TypedDagNode<double>* q=dynamic_cast<const TypedDagNode< double >* >(newP);

  if (q){
    string n=_mD2N[r];
  
    _mN2D[n]=q;
    _mD2N.erase(r);
    _mD2N[q]=n;
  
    static_cast< BppRateMatrix*>(value)->setParameterValue(n, q->getValue());
  }
  
}


