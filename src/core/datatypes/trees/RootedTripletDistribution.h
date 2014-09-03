/**
 * @file
 * This file contains the declaration of a rooted triplet distribution,
 * that holds a vector of triplets along with their frequencies.
 *
 *
 * @brief Declaration of the RootedTripletDistribution
 *
 * (c) Copyright 2009- under GPL version 3
 * @date Last modified: $Date: 2012-07-05 16:47:08 +0200 (Thu, 05 Jul 2012) $
 * @author The RevBayes Development Core Team
 * @license GPL version 3
 * @version 1.0
 * @since 2011-04-13, version 1.0
 *
 * $Id: RootedTripletDistribution.h 1651 2014-08-31 14:47:08Z Boussau $
 */

#ifndef RootedTripletDistribution_H
#define RootedTripletDistribution_H

#include "Cloneable.h"
#include "MemberObject.h"
#include "Taxon.h"
#include "TopologyNode.h"
#include "TimeTree.h"

#include <map>
#include <vector>
#include <set>
#include <string>


namespace RevBayesCore {
    
    class RootedTripletDistribution {
        
    public:
    
        RootedTripletDistribution(void);                                                                           //!< Default constructor
        RootedTripletDistribution(const RootedTripletDistribution& t);                                             //!< Copy constructor
        virtual                                    ~RootedTripletDistribution(void);                                                                                                        //!< Destructor
        
        RootedTripletDistribution&                                   operator=(const RootedTripletDistribution& t);
        
        // Basic utility functions
        RootedTripletDistribution*                                   clone(void) const;                                                      //!< Clone object
        
        // RootedTripletDistribution functions
        void extractTriplets( TimeTree& t );                           //!< Getting the triplets from a tree and adding them to the tripletDistribution
        void extractTriplets( std::vector<TimeTree>& ts );             //!< Getting the triplets from a tree and adding them to the tripletDistribution

        size_t getNumberOfTrees() const;                                                                                     //!< Get the number of trees that were used to build the object
        size_t getNumberOfTriplets() const;                                                       //!< Get the number of triplets in the object
        void populateTripletDistribution ( TopologyNode* node, std::vector< size_t >& allTips ) ;
        
        void setSpecies ( std::vector< std::string > s );
        void setTaxa ( std::vector< Taxon > t );
        std::vector< std::string > getSpecies ( ) const;
        std::vector< Taxon > getTaxa ( ) const;
        
 //   protected:
        // Nothing
    private:
        void addAllTriplets(std::vector< size_t >& leftTips, std::vector< size_t >& rightTips) ; //!< Get all rooted triplets given vectors of left and right tips
        void addAllTripletsOneWay( std::vector< size_t >& leftTips, std::vector< size_t >& rightTips, size_t leftSize,size_t rightSize ); //!< Get rooted triplets given vectors of left and right tips, one way only

        
        //Members:
        std::vector< Taxon >                                                           taxa;
        std::vector< std::string >                                                  species;
        std::map < std::pair < size_t, std::pair < size_t, size_t > >, size_t >                         tripletDistribution;
        size_t                                                                numberOfTrees;
        std::set< Taxon >                                                           taxaSet;
        std::set< std::string >                                                  speciesSet;
        std::map< Taxon, size_t >                                              taxonToIndex;
        std::map< std::string, size_t >                                      speciesToIndex;
        bool                                                                    speciesOnly;
        
        
    };
    
    // Global functions using the class
    std::ostream& operator<<(std::ostream& o, const RootedTripletDistribution& x);       //!< Overloaded output operator
    
}


#endif

