/**
 * @file
 * This file contains the declaration of the RevBayes test suit.
 *
 *
 * @brief Declaration of the test suit
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-07-11
 *
 * $Id$
 */


#ifndef Test_H
#define Test_H

class Test {
    
public:
    Test(void);
    Test(int c, const char * v[]);
    virtual                    ~Test(void);                                                          //!< Virtual destructor
    bool                        performTests();
    
private:
    int argc;
    const char** argv;
    
};

#endif
