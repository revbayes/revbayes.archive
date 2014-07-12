//
//  MicrosatData.h
//  revbayes_mlandis
//
//  Created by Michael Landis on 12/7/12.
//  Copyright (c) 2012 Michael Landis. All rights reserved.
//

#ifndef __revbayes_mlandis__MicrosatData__
#define __revbayes_mlandis__MicrosatData__

#include <iostream>
#include <string>
#include <vector>


namespace RevBayesCore {
    
    class MicrosatData {
        
    public:
        
        // get [population][locus]
        std::string getPopulationNames(int a) const                         { return populationNames[a]; }
        std::vector<std::string> getPopulationNames(void)                   { return populationNames; }
        int getNumPopulations(void) const                                   { return numPopulations; }
        int getNumMicrosats(void) const                                     { return numMicrosats; }
        int getNumChromosomes(int a) const                                  { return numChromosomes[a]; }
        double getMicrosatMeans(int a, int b) const                         { return microsatMeans[a][b]; }
        double getMicrosatMeans(std::string a, int b) const;
        const std::vector<double>& getMicrosatMeans(int a) const           { return microsatMeans[a]; }
        const std::vector<double>& getMicrosatMeans(std::string a) const;
        const std::vector<std::vector<double> >& getMicrosatMeans(void) const { return microsatMeans; }
        
        int getNumSamples(int a, int b) const                               { return numSamples[a][b]; }
        const std::vector<int>& getNumSamples(int a) const                  { return numSamples[a]; }
        const std::vector<std::vector<int> >& getNumSamples(void) const     { return numSamples; }
        const std::vector<bool>& getOutgroup(void) const                    { return outgroup; }
        bool getOutgroup(int a) const                                       { return outgroup[a]; }
        
        // set
        void setNumPopulations(int a)                                       { numPopulations = a; }
        void setNumMicrosats(int a)                                         { numMicrosats = a; }
        void setPopulationNames(std::vector<std::string> a)                 { populationNames = a; }
        void setNumChromosomes(std::vector<int> a)                          { numChromosomes = a; }
        void setMicrosatMeans(std::vector<std::vector<double> > a)          { microsatMeans = a; }
        void setNumSamples(std::vector<std::vector<int> > a)                { numSamples = a; }
        void setOutgroup(std::vector<bool> a)                               { outgroup = a; }
        
    private:
        unsigned numMicrosats;
        unsigned numPopulations;
        std::vector<std::string> populationNames;
        std::vector<int> numChromosomes; // max N per population
        std::vector<std::vector<double> > microsatVars;
        std::vector<std::vector<double> > microsatMeans;
        std::vector<std::vector<int> > numSamples;
        std::vector<bool> outgroup;
        
    };
    
    
}

#endif /* defined(__revbayes_mlandis__MicrosatData__) */
