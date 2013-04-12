/**
 * @file
 * This file contains the declaration of the RevLanguage test class. 
 * This test runs several test files using the RevLanguage environment.
 *
 *
 * @brief Declaration of the normal model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-04-17
 *
 * $Id$
 */


#ifndef TestRevLanguage_H
#define TestRevLanguage_H


class TestRevLanguage {
    
public:
    TestRevLanguage(void);
    virtual                    ~TestRevLanguage(void);                                                          //!< Virtual destructor
    
    bool                        run();
    
};

#endif
