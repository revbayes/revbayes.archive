#include "RbHelpType.h"

using namespace RevBayesCore;


const std::string& RbHelpType::getAuthor( void ) const
{
    // return a const reference to the internal value
    return author;
}


const std::vector<std::string>& RbHelpType::getDescription( void ) const
{
    // return a const reference to the internal value
    return description;
}


const std::vector<std::string>& RbHelpType::getDetails( void ) const
{
    // return a const reference to the internal value
    return details;
}


const std::string& RbHelpType::getName( void ) const
{
    // return a const reference to the internal value
    return name;
}


const std::vector<RbHelpReference>& RbHelpType::getReferences( void ) const
{
    // return a const reference to the internal value
    return references;
}


const std::string& RbHelpType::getTitle( void ) const
{
    // return a const reference to the internal value
    return title;
}


const std::string& RbHelpType::getUsage( void ) const
{
    // return a const reference to the internal value
    return usage;
}


void RbHelpType::setAuthor(const std::string &a)
{
    // overwrite the internal value with the new value
    author = a;
}

void RbHelpType::setDescription(const std::vector<std::string> &d)
{
    // overwrite the internal value with the new value
    description = d;
}


void RbHelpType::setDetails(const std::vector<std::string> &d)
{
    // overwrite the internal value with the new value
    details = d;
}


void RbHelpType::setName(const std::string &n)
{
    // overwrite the internal value with the new value
    name = n;
}


void RbHelpType::setReferences(const std::vector<RbHelpReference> &r)
{
    // overwrite the internal value with the new value
    references = r;
}


void RbHelpType::setTitle(const std::string &t)
{
    // overwrite the internal value with the new value
    title = t;
}


void RbHelpType::setUsage(const std::string &u)
{
    // overwrite the internal value with the new value
    usage = u;
}
