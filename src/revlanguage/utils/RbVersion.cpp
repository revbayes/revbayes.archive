#include "RbVersion.h"
#include "GitVersion.h"

RbVersion::RbVersion( void )
{
    
}

std::string RbVersion::getDate( void ) const
{
    std::string date = build_date;
    return date;
}

std::string RbVersion::getGitBranch( void ) const
{
    std::string git_branch = build_git_branch;
    return git_branch;
}

std::string RbVersion::getGitCommit( void ) const
{
    std::string git_commit = build_git_sha;
    return git_commit;
}

std::string RbVersion::getVersion( void ) const
{
    return "1.0.4";
}


std::string RbVersion::getHeader( void ) const
{
    
    std::string header = "";
    header += "\n";
    header += "RevBayes version (" + getVersion() + ")\n";
    header += "Build from " + getGitBranch() + " (" + getGitCommit() + ") on " + getDate() + "\n";
    header += "\n";
    header += "Visit the website www.RevBayes.com for more information about RevBayes.\n";
    header += "\n";
    header += "RevBayes is free software released under the GPL license, version 3. Type 'license()' for details.\n";
    header += "\n";
    header += "To quit RevBayes type 'quit()' or 'q()'.\n";
    
    return header;
    
}



