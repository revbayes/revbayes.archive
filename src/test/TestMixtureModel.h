/**
 * @file
 * This file contains the declaration of the mixture model test class. 
 * This test runs an MCMC analysis on a mixture model with the data being normal distributed, 
 * a uniform prior on mu and an exponential prior on sigma and a mixture of mu parameters.
 *
 *
 * @brief Declaration of the mixture model test class
 *
 * (c) Copyright 2009-
 * @date Last modified: $Date$
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @since Version 1.0, 2012-04-17
 *
 * $Id$
 */


#ifndef TestMixtureModel_H
#define TestMixtureModel_H


class TestMixtureModel {
    
public:
    TestMixtureModel(int gen);
    virtual                    ~TestMixtureModel(void);                                                          //!< Virtual destructor
    
    bool                        run();
    
private:
    
    // members
    int                         mcmcGenerations;
};

#endif
