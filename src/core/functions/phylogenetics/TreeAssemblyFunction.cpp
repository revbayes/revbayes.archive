#include "TreeAssemblyFunction.h"
#include "RbConstants.h"
#include "RbException.h"
#include "RbMathLogic.h"

using namespace RevBayesCore;

TreeAssemblyFunction::TreeAssemblyFunction(const TypedDagNode<Tree> *t, const TypedDagNode< RbVector<double> > *b, bool d, bool m) : TypedFunction<Tree>( NULL ),
    tau( t ),
    brlen( b ),
    divide( d ),
    multiply( m )
{

    if (tau->getValue().getNumberOfNodes() - 1 != brlen->getValue().size())
    {
        throw(RbException("Number of branches does not match the number of branch lengths"));
    }

    addParameter( tau );
    addParameter( brlen );
    
    update();
}


TreeAssemblyFunction::TreeAssemblyFunction(const TreeAssemblyFunction &f) : TypedFunction<Tree>( f ),
    tau( f.tau ),
    brlen( f.brlen ),
    divide( f.divide ),
    multiply( f.multiply)
{
    
    update();
}


TreeAssemblyFunction::~TreeAssemblyFunction( void )
{
    //delete value;
}



TreeAssemblyFunction* TreeAssemblyFunction::clone( void ) const
{
    return new TreeAssemblyFunction( *this );
}


void TreeAssemblyFunction::keep(DagNode *affecter)
{
    //delegate to base class
    TypedFunction< Tree >::keep( affecter );
    
//    touchedNodeIndices.clear();
}


void TreeAssemblyFunction::reInitialized( void )
{
    
}


void TreeAssemblyFunction::restore(DagNode *restorer)
{
    //delegate to base class
    TypedFunction< Tree >::restore( restorer );
    
//    touchedNodeIndices.clear();
}


void TreeAssemblyFunction::touch(DagNode *toucher)
{
    
    //delegate to base class
    TypedFunction< Tree >::touch( toucher );
    
    if ( toucher == brlen )
    {
        const std::set<size_t> &touchedIndices = toucher->getTouchedElementIndices();
        touchedNodeIndices.insert(touchedIndices.begin(), touchedIndices.end());
    }
    
}


void TreeAssemblyFunction::update( void )
{
    
    if ( touchedNodeIndices.size() > 0 )
    {
        const std::vector<double> &v = brlen->getValue();
        for (std::set<size_t>::iterator it = touchedNodeIndices.begin(); it != touchedNodeIndices.end(); ++it)
        {
            double br = tau->getValue().getNode(*it).getBranchLength();

            br = RbMath::isNan(br) ? 1.0 : br;

            if( multiply )
            {
                br *= v[*it];
            }
            else if( divide )
            {
                br /= v[*it];
            }
            else
            {
                br = v[*it];
            }

            value->getNode(*it).setBranchLength(br);
        }
        touchedNodeIndices.clear();
    }
    else
    {
        delete value;
        value = tau->getValue().clone();

        const std::vector<double> &v = brlen->getValue();
        for (size_t i = 0; i < v.size(); ++i)
        {
            double br = tau->getValue().getNode(i).getBranchLength();

            br = RbMath::isNan(br) ? 1.0 : br;

            if( multiply )
            {
                br *= v[i];
            }
            else if( divide )
            {
                br /= v[i];
            }
            else
            {
                br = v[i];
            }

            value->getNode(i).setBranchLength(br);
        }
        
    }
    
}



void TreeAssemblyFunction::swapParameterInternal(const DagNode *oldP, const DagNode *newP)
{

    if (oldP == tau)
    {
        tau = static_cast<const TypedDagNode<Tree>* >( newP );
        
        delete value;
        value = tau->getValue().clone();
        
    }
    else if (oldP == brlen)
    {
        brlen = static_cast<const TypedDagNode< RbVector<double> >* >( newP );
    }
    
}


