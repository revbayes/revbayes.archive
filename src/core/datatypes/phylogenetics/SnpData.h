//
//  SnpData.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/7/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__SnpData__
#define __revbayes_mlandis__SnpData__

#include <iostream>
#include <string>
#include <vector>


namespace RevBayesCore {
    
    class SnpData {
        
    public:
        //SnpData(void);
        //virtual                 ~SnpData(void);

        // get
        std::string getPopulationNames(int a) const { return populationNames[a]; }
        std::vector<std::string> getPopulationNames(void) { return populationNames; }
        int getNumPopulations(void) const { return numPopulations; }
        int getNumSnps(void) const { return numSnps; }
        int getNumIndividuals(int a) const { return numIndividuals[a]; }
        double getSnpFrequencies(int a, int b) const { return snpFrequencies[a][b]; }
        double getSnpFrequencies(std::string a, int b) const;
        const std::vector<double>& getSnpFrequencies(int a) const { return snpFrequencies[a]; }
        const std::vector<double>& getSnpFrequencies(std::string a) const;
        const std::vector<std::vector<double> >& getSnpFrequencies(void) const { return snpFrequencies; }
        
        int getNumSamples(int a, int b) const { return numSamples[a][b]; }
        const std::vector<int>& getNumSamples(int a) const { return numSamples[a]; }
        const std::vector<std::vector<int> >& getNumSamples(void) const { return numSamples; }
        int getNumAlleles(int a, int b) const { return numAlleles[a][b]; }
        const std::vector<int>& getNumAlleles(int a) const { return numAlleles[a]; }
        const std::vector<std::vector<int> >& getNumAlleles(void) const { return numAlleles; }
        
        // set
        void setNumPopulations(int a) { numPopulations = a; }
        void setNumSnps(int a) { numSnps = a; }
        void setPopulationNames(std::vector<std::string> a) { populationNames = a; }
        void setNumIndividuals(std::vector<int> a) { numIndividuals = a; }
        void setSnpFrequencies(std::vector<std::vector<double> > a) { snpFrequencies = a; }
        void setNumAlleles(std::vector<std::vector<int> > a) { numAlleles = a; }
        void setNumSamples(std::vector<std::vector<int> > a) { numSamples = a; }
        
    private:
        int numSnps;
        int numPopulations;
        std::vector<std::string> populationNames;
        std::vector<int> numIndividuals; // per population
        std::vector<std::vector<double> > snpFrequencies;
        std::vector<std::vector<int> > numAlleles;
        std::vector<std::vector<int> > numSamples;
        
    };
    

}

#endif /* defined(__revbayes_mlandis__SnpData__) */
