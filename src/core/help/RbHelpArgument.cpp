#include "RbHelpArgument.h"

using namespace RevBayesCore;

const std::string& RbHelpArgument::getArgumentType( void ) const
{
    
    return argumentType;
}


const std::string& RbHelpArgument::getDefaultValue( void ) const
{
    
    return defaultValue;
}


const std::string& RbHelpArgument::getDescription( void ) const
{
    
    return description;
}


const std::string& RbHelpArgument::getLabel( void ) const
{
    
    return label;
}


const std::vector<std::string>& RbHelpArgument::getOptions( void ) const
{
    
    return options;
}


const std::string& RbHelpArgument::getValueType( void ) const
{
    
    return valueType;
}


void RbHelpArgument::setArgumentType(const std::string &at)
{
    
    argumentType = at;
    
}


void RbHelpArgument::setDefaultValue(const std::string &df)
{
    
    defaultValue = df;
    
}


void RbHelpArgument::setDescription(const std::string &d)
{
    
    description = d;
    
}


void RbHelpArgument::setLabel(const std::string &l)
{
    
    label = l;
    
}


void RbHelpArgument::setOptions(const std::vector<std::string> &o)
{
    
    options = o;
    
}


void RbHelpArgument::setValueType(const std::string &vt)
{
    
    valueType = vt;
    
}
