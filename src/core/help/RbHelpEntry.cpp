#include "RbHelpEntry.h"

using namespace RevBayesCore;




const std::vector<std::string>& RbHelpEntry::getAliases( void ) const
{
    // return a const reference to the internal value
    return aliases;
}


const std::vector<std::string>& RbHelpEntry::getDescription( void ) const
{
    // return a const reference to the internal value
    return description;
}


const std::string& RbHelpEntry::getName( void ) const
{
    // return a const reference to the internal value
    return name;
}


const std::string& RbHelpEntry::getTitle( void ) const
{
    // return a const reference to the internal value
    return title;
}


void RbHelpEntry::setAliases(const std::vector<std::string> &a)
{
    // overwrite the internal value with the new value
    aliases = a;
}


void RbHelpEntry::setDescription(const std::vector<std::string> &d)
{
    // overwrite the internal value with the new value
    description = d;
}


void RbHelpEntry::setName(const std::string &n)
{
    // overwrite the internal value with the new value
    name = n;
}


void RbHelpEntry::setTitle(const std::string &t)
{
    // overwrite the internal value with the new value
    title = t;
}


