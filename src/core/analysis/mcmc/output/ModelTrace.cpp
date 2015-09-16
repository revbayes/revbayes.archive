#include "ModelTrace.h"

#include "RbUtil.h"

#include <sstream>
#include <string>
#include <vector>

using namespace RevBayesCore;

ModelTrace::ModelTrace()
{
    
}


void ModelTrace::addObject(const std::string &s)
{
    values.push_back(s);
}


/** Clone function */
ModelTrace* ModelTrace::clone() const
{
    
    return new ModelTrace(*this);
}


void ModelTrace::removeObjectAtIndex (int index)
{
    // create a iterator for the vector
    std::vector<std::string>::iterator it = values.begin();
    
    //jump to the position to remove
    it += index;
    
    // remove the element
    values.erase(it);
}

void ModelTrace::removeLastObject()
{
    // remove object from list
    values.pop_back();
    
}


std::ostream& RevBayesCore::operator<<(std::ostream& o, const ModelTrace& x)
{

    o << x.getParameterName();
    o << " (";
    const std::vector<std::string>& values = x.getValues();
    for (std::vector<std::string>::const_iterator it = values.begin(); it != values.end(); ++it)
    {
        
        if ( it != values.begin() )
        {
            o << ", ";
        }
        o << *it;
    }
    o << ")";
    
    return o;
}





