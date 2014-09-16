#include "RbHelpFunction.h"

using namespace RevBayesCore;

const std::vector<RbHelpArgument>& RbHelpFunction::getArguments( void ) const
{
    // return a const reference to the internal value
    return arguments;
}


const std::string& RbHelpFunction::getAuthor( void ) const
{
    // return a const reference to the internal value
    return author;
}


const std::vector<std::string>& RbHelpFunction::getDescription( void ) const
{
    // return a const reference to the internal value
    return description;
}


const std::vector<std::string>& RbHelpFunction::getDetails( void ) const
{
    // return a const reference to the internal value
    return details;
}


const std::string& RbHelpFunction::getExample( void ) const
{
    // return a const reference to the internal value
    return example;
}


const std::string& RbHelpFunction::getName( void ) const
{
    // return a const reference to the internal value
    return name;
}


const std::vector<RbHelpReference>& RbHelpFunction::getReferences( void ) const
{
    // return a const reference to the internal value
    return references;
}


const std::string& RbHelpFunction::getReturnValue( void ) const
{
    // return a const reference to the internal value
    return returnValue;
}


const std::vector<std::string>& RbHelpFunction::getSeeAlso( void ) const
{
    // return a const reference to the internal value
    return seeAlso;
}


const std::string& RbHelpFunction::getTitle( void ) const
{
    // return a const reference to the internal value
    return title;
}


const std::string& RbHelpFunction::getUsage( void ) const
{
    // return a const reference to the internal value
    return usage;
}


