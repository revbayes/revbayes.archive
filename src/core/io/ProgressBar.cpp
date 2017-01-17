#include "ProgressBar.h"

#include <iostream>

using namespace RevBayesCore;


/** Read Model Trace */
ProgressBar::ProgressBar( size_t m, size_t o ) :
    max( m ),
    num_stars( 0 ),
    offset( o )
{
    
}

void ProgressBar::finish( void )
{
    
    update( max );
    
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout.flush();
    
}

void ProgressBar::start( void )
{
    
    // Print progress bar (68 characters wide)
    std::cout << std::endl;
    std::cout << "Progress:" << std::endl;
    std::cout << "0---------------25---------------50---------------75--------------100" << std::endl;
    std::cout.flush();
    
}

void ProgressBar::update( size_t i )
{
    
    size_t progress = 68 * double(i-offset) / double(max-offset);
    if ( progress > num_stars )
    {
                
        for ( ; num_stars < progress; ++num_stars )
        {
            std::cout << "*";
        }
        std::cout.flush();
        
    }
    
}



