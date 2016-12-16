#include "RbSettings.h"
#include "RbUtil.h"
#include "StringUtilities.h"
#include "RlUserInterface.h"

#if defined (RB_MPI)
#include <mpi.h>
#endif

using namespace RevLanguage;


UserInterface::UserInterface( void ) :
    processID( 0 )
{
#if defined (RB_MPI)
        processID = MPI::COMM_WORLD.Get_rank();
#endif
}

UserInterface::UserInterface( const UserInterface &u ) :
    processID( u.processID )
{
}

/** Ask user a yes/no question */
bool UserInterface::ask(std::string msg)
{

    std::string answer, dummy;
    std::cout << RevBayesCore::RbUtils::PAD << (msg + "? (yes/no) ");     // not using RBOUT or output because we do not want a newline
    std::cin >> answer;
    for (size_t i=0; i<answer.size(); i++)
    {
        answer[i] = char( tolower(answer[i])) ;
    }
    
    while (answer!="y" && answer!="yes" && answer!="n" && answer!="no") 
    {
        std::getline(std::cin, dummy);
        std::cout << std::endl;
		RBOUT("Please answer yes or no.");
        std::cout << RevBayesCore::RbUtils::PAD << (msg + "? (yes/no) "); // see above for choice of std::cout

        std::cin >> answer;
        for (size_t i=0; i<answer.size(); i++)
        {
            answer[i] = char( tolower(answer[i]) );
        }
        
    }
    std::getline(std::cin, dummy);

    if (answer[0] == 'y')
    {
        return true;
    }
    else
    {
        return false;
    }
    
}


/** Print a message and a newline */
void UserInterface::output(std::string msg)
{

    if ( processID == 0 )
    {
        std::string pad = "   ";
        std::cout << StringUtilities::formatStringForScreen( msg, pad, pad, RbSettings::userSettings().getLineWidth() );
    }
    
}


/** Print a message and a newline without the padding */
void UserInterface::output(std::string msg, const bool hasPadding)
{

    if ( processID == 0 )
    {
        if (hasPadding == true)
        {
            output(msg);
        }
        else
        {
            std::cout << msg << std::endl;
        }
        
    }
}


/** Convert to string and then call output to print message string */
void UserInterface::output(std::ostringstream msg) {

    output( msg.str() );
}

