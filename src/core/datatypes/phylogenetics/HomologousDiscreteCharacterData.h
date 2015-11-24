#ifndef HomologousDiscreteCharacterData_H
#define HomologousDiscreteCharacterData_H

#include "AbstractHomologousDiscreteCharacterData.h"
#include "DiscreteCharacterState.h"
#include "MatrixReal.h"
#include "DiscreteTaxonData.h"

#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

namespace RevBayesCore {
    
    /**
     * Template class for all discrete character data objects.
     *
     * The discrete character data class is template class for all discrete character data objects.
     * Here the data is stored via a set of DiscreteTaxonData objects.
     * Several convenience functions are provided to access, store and delete the data.
     *
     * @copyright Copyright 2009-
     * @author The RevBayes Development Core Team (Sebastian Hoehna)
     * @since 2013-04-15, version 1.0
     */
    template<class charType>
    class HomologousDiscreteCharacterData : public AbstractHomologousDiscreteCharacterData {
        
    public:
                                                            HomologousDiscreteCharacterData();                                                                                        //!< Default constructor
        
        // Overloaded operators
        const DiscreteTaxonData<charType>&                  operator[](size_t i) const;                                                 //!< Subscript operator (const)
        bool                                                operator<(const HomologousDiscreteCharacterData& x) const;                  //!< Less than operator
        
        // implemented methods of the Cloneable interface
        HomologousDiscreteCharacterData<charType>*          clone(void) const;
        
        // implemented methods of the Serializable interface
        void                                                initFromFile( const std::string &dir, const std::string &fn );              //!< Read and resurrect this object from a file in its default format.
        void                                                initFromString( const std::string &s );                                     //!< Serialize (resurrect) the object from a string value

        // CharacterData functions
        MatrixReal                                          computeStateFrequencies(void) const;
        HomologousDiscreteCharacterData&                    concatenate(const HomologousDiscreteCharacterData &d);                      //!< Concatenate data matrices
        HomologousDiscreteCharacterData&                    concatenate(const AbstractCharacterData &d);                                //!< Concatenate data matrices
        HomologousDiscreteCharacterData&                    concatenate(const HomologousCharacterData &d);                              //!< Concatenate data matrices
        HomologousDiscreteCharacterData&                    concatenate(const AbstractHomologousDiscreteCharacterData &d);              //!< Concatenate data matrices
        void                                                excludeAllCharacters(void);                                                 //!< Exclude all characters
        void                                                excludeCharacter(size_t i);                                                 //!< Exclude character
        const charType&                                     getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                         getDataType(void) const;
        std::vector<double>                                 getEmpiricalBaseFrequencies(void) const;                                    //!< Compute the empirical base frequencies
        const std::set<size_t>&                             getExcludedCharacters(void) const;                                          //!< Returns the name of the file the data came from
        size_t                                              getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                                              getNumberOfIncludedCharacters(void) const;                                  //!< Number of characters
        size_t                                              getNumberOfInvariantSites(void) const;                                      //!< Number of invariant sites
        size_t                                              getNumberOfSegregatingSites(void) const;                                    //!< Compute the number of segregating sites
        size_t                                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        double                                              getAveragePaiwiseSequenceDifference(void) const;                                   //!< Get the average pairwise sequence distance.
        size_t                                              getMaxPaiwiseSequenceDifference(void) const;                                   //!< Get the average pairwise sequence distance.
        size_t                                              getMinPaiwiseSequenceDifference(void) const;                                   //!< Get the average pairwise sequence distance.
        DiscreteTaxonData<charType>&                        getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        DiscreteTaxonData<charType>&                        getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        
        double                                              maxGcContent(void) const;                                                   //!< Maximum GC-content of a sequence
        size_t                                              maxInvariableBlockLength(void) const;                                       //!< Maximum length of a block of invariant sites
        size_t                                              maxVariableBlockLength(void) const;                                         //!< Maximum length of a block of variant sites
        double                                              minGcContent(void) const;                                      //!< Number of invariant sites
        size_t                                              numInvariableSiteBlocks(void) const;                                      //!< Number of invariant sites

        void                                                includeCharacter(size_t i);                                                 //!< Include character
        bool                                                isCharacterExcluded(size_t i) const;                                        //!< Is the character excluded
        bool                                                isCharacterResolved(size_t txIdx, size_t chIdx) const;                      //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        bool                                                isCharacterResolved(const std::string &tn, size_t chIdx) const;             //!< Returns whether the character is fully resolved (e.g., "A" or "1.32") or not (e.g., "AC" or "?")
        void                                                removeExludedCharacters(void);                                              //!< Remove all the excluded characters
        void                                                restoreCharacter(size_t i);                                                 //!< Restore character

    
    protected:
        // Utility functions
        bool                                                isCharacterConstant(size_t idx) const;                                      //!< Is the idx-th character a constant pattern?
        bool                                                isCharacterMissingOrAmbiguous(size_t idx) const;                            //!< Does the character have missing or ambiguous data?
        size_t                                              numConstantPatterns(void) const;                                            //!< The number of constant patterns
        size_t                                              numMissAmbig(void) const;                                                   //!< The number of patterns with missing or ambiguous characters
        
        // Member variables
        std::set<size_t>                                    deletedCharacters;                                                          //!< Set of deleted characters
        
    };
    
    
}


#include "DnaState.h"
#include "DiscreteCharacterState.h"
#include "DiscreteTaxonData.h"
#include "NclReader.h"
#include "RbConstants.h"
#include "RbException.h"

#include <cmath>
#include <fstream>
#include <sstream>
#include <string>


/**
 * Default constructor,
 * Does nothing except instanciating the object.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>::HomologousDiscreteCharacterData() 
{

}



/** 
 * Index (const) operator to access a TaxonData object at position i.
 *
 * \param[in]    i    The position of the TaxonData object.
 *
 * \return            The TaxonData object at position i.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::operator[]( const size_t i ) const 
{
    
    return getTaxonData( i );
}



/**
 * Less than operator.
 */
template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::operator<(const HomologousDiscreteCharacterData<charType> &x) const 
{
    
    return taxa.size() < x.taxa.size();
}




/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the object.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>* RevBayesCore::HomologousDiscreteCharacterData<charType>::clone( void ) const
{
    
    return new HomologousDiscreteCharacterData<charType>(*this);
}



/**
 * Compute the state frequencies per site.
 *
 * \return       A matrix of character frequencies where each column is a character and each row a taxon.
 */
template<class charType>
RevBayesCore::MatrixReal RevBayesCore::HomologousDiscreteCharacterData<charType>::computeStateFrequencies( void ) const
{
    
    charType tmp;
    size_t alphabetSize = tmp.getNumberOfStates();
    size_t numSequences = this->taxa.size();
    MatrixReal m(numSequences,alphabetSize);
    
    double MIN_THRESHOLD = 1E-3;
    for (size_t i = 0; i < numSequences; ++i)
    {
        const DiscreteTaxonData<charType>& seq = this->getTaxonData(i);
        size_t l = seq.size();
        double nonGapSeqLength = 0.0;
        std::vector<double> stateCounts(alphabetSize, MIN_THRESHOLD);
        for (size_t j = 0; j < l; ++j)
        {
            const charType& c = seq[j];
            
            if ( c.isMissingState() == true )
            {
                nonGapSeqLength++;
                
                size_t num_states = c.getNumberOfStates();
                
                for (size_t index = 0; index < num_states; ++index)
                {
                    stateCounts[index] += 1.0 / double( num_states );
                }
                
            }
            else if ( c.isGapState() == false )
            {
                nonGapSeqLength++;
                
                unsigned long state = c.getState();
                double numObservedStates = c.getNumberObservedStates();
                
                size_t index = 0;
                do
                {
                    
                    if ( (state & 1) == 1 )
                    {
                        // add a uniform probability of having observed each of the ambiguous characters
                        stateCounts[index] += 1.0 / numObservedStates;
                    }
                    state >>= 1;
                    ++index;
                } while ( state != 0 );
                
            }
            
            
        } // finished loop over sequence
        
        // set the observed state frequencies for this sequence into the matrix
        std::vector<double> &observedFreqs = m[i];
        for (size_t j = 0; j < alphabetSize; ++j)
        {
            observedFreqs[j] = stateCounts[j] / (nonGapSeqLength+20*MIN_THRESHOLD);
        }
    }
    
    return m;
}



/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::concatenate(const AbstractCharacterData &obsd)
{
    
    const HomologousDiscreteCharacterData<charType>* rhs = dynamic_cast<const HomologousDiscreteCharacterData<charType>* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into CharacterData!!!");
    }
    
    
    return concatenate( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::concatenate(const HomologousCharacterData &obsd)
{
    
    const HomologousDiscreteCharacterData<charType>* rhs = dynamic_cast<const HomologousDiscreteCharacterData<charType>* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into CharacterData!!!");
    }
    
    
    return concatenate( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::concatenate(const AbstractHomologousDiscreteCharacterData &obsd)
{
    
    const HomologousDiscreteCharacterData<charType>* rhs = dynamic_cast<const HomologousDiscreteCharacterData<charType>* >( &obsd );
    if ( rhs == NULL )
    {
        throw RbException("Adding wrong character data type into CharacterData!!!");
    }
    
    
    return concatenate( *rhs );
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::HomologousDiscreteCharacterData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::concatenate(const HomologousDiscreteCharacterData<charType> &obsd)
{
    
    size_t sequenceLength = getNumberOfCharacters();
    
    // check if both have the same number of taxa
    if ( taxa.size() != obsd.getNumberOfTaxa() )
    {
        throw RbException("Cannot concatenate two character data objects with different number of taxa!");
    }
    
    std::vector<bool> used = std::vector<bool>(obsd.getNumberOfTaxa(),false);
    for (size_t i=0; i<taxa.size(); i++ )
    {
        const std::string &n = taxa[i].getName();
        DiscreteTaxonData<charType>& taxon = getTaxonData( n );
        
        size_t idx = obsd.getIndexOfTaxon( n );
        if ( idx != RbConstants::Size_t::inf)
        {
            used[idx] = true;
            taxon.concatenate( obsd.getTaxonData( n ) );
        }
        else
        {
            throw RbException("Cannot concatenate two character data objects because second character data object has no taxon with name '" + n + "n'!");
        }
    }
    
    for (size_t i=0; i<used.size(); i++)
    {
        if ( used[i] == false )
        {
            throw RbException("Cannot concatenate two character data objects because first character data object has no taxon with name '" + obsd.getTaxonNameWithIndex(i) + "n'!");
        }
    }
    
    const std::set<size_t> &exclChars = obsd.getExcludedCharacters();
    for (std::set<size_t>::const_iterator it = exclChars.begin(); it != exclChars.end(); ++it)
    {
        deletedCharacters.insert( *it + sequenceLength );
    }
    
    // return a reference to this object
    return *this;
}


/**
 * Exclude all characters.
 * We don't actually delete the characters but mark them for exclusion.
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::excludeAllCharacters(void)
{
    
    for (size_t i = 0; i < getTaxonData( 0 ).size(); ++i)
    {
        deletedCharacters.insert( i );
    }
    
}


/** 
 * Exclude a character.
 * We don't actually delete the character but mark it for exclusion.
 *
 * \param[in]    i    The position of the character that will be excluded.
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::excludeCharacter(size_t i) 
{
    
    if (i >= getTaxonData( 0 ).size() ) 
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.insert( i );
    
}



/** 
 * Get the cn-th character of the tn-th taxon.
 *
 * \param[in]    tn     The index/position of the taxon.
 * \param[in]    cn     The position of the character.
 *
 * \return              The cn-th character of the tn-th taxon. 
 */
template<class charType>
const charType& RevBayesCore::HomologousDiscreteCharacterData<charType>::getCharacter( size_t tn, size_t cn ) const 
{
    
    if ( cn >= getNumberOfCharacters() )
    {
        throw RbException( "Character index out of range" );
    }
    
    return getTaxonData( tn )[cn];
}


/**
 * Get the data type of the character stored in this object.
 *
 * \return      The data type (e.g. DNA, RNA or Standard).
 */
template<class charType>
std::string RevBayesCore::HomologousDiscreteCharacterData<charType>::getDataType(void) const
{
    
    std::string dt = "";
    if ( taxa.size() > 0 )
    {
        const DiscreteTaxonData<charType> &t = getTaxonData( taxa[0].getName() );
        if ( t.size() > 0 ) 
        {
            dt = t[0].getDataType();
        }
        
    }
    
    return dt;
}


/**
 * Get the set of excluded character indices.
 *
 * \return    The excluded character indices.
 */
template<class charType>
const std::set<size_t>& RevBayesCore::HomologousDiscreteCharacterData<charType>::getExcludedCharacters(void) const
{
    
    return deletedCharacters;
}


/**
 * Get the set of excluded character indices.
 *
 * \return    The excluded character indices.
 */
template<class charType>
std::vector<double> RevBayesCore::HomologousDiscreteCharacterData<charType>::getEmpiricalBaseFrequencies(void) const
{
    size_t nStates = this->getTaxonData(0)[0].getNumberOfStates();
    std::vector<double> ebf = std::vector<double>(nStates, 0.0);
    double total = 0.0;
    size_t nt = this->getNumberOfTaxa();
    for (size_t i=0; i<nt; i++)
    {
        
        const AbstractDiscreteTaxonData& taxonData = this->getTaxonData(i);
        size_t nc = taxonData.getNumberOfCharacters();
        for (size_t j=0; j<nc; j++)
        {
            const DiscreteCharacterState& o = taxonData[j];
            if ( o.isAmbiguous() == false )
            {
                ++total;
                ++ebf[o.getStateIndex()];
            }
        }
    }
    
    for (size_t i=0; i<nStates; ++i)
    {
        ebf[i] /= total;
    }

    
    return ebf;
}



/** 
 * Get the number of characters in taxon data object. 
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getNumberOfCharacters(void) const 
{
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(0).getNumberOfCharacters();
    }
    
    return 0;
}


/** 
 * Get the number of characters in taxon data object. 
 * This i regardless of whether the character are included or excluded.
 * For simplicity we assume that all taxon data objects contain the same number
 * of character and thus we simply return the number from the first taxon data object.
 *
 * \return    The total number of characters
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getNumberOfIncludedCharacters(void) const
{
    
    if (getNumberOfTaxa() > 0) 
    {
        return getTaxonData(0).getNumberOfCharacters() - deletedCharacters.size();
    }
    return 0;
}


/** 
 * Get the number of states for the characters in this object. 
 * We assume that all of the characters in the matrix are of the same
 * type and have the same number of potential states. 
 *
 * \return      The number of states for the characters.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getNumberOfStates(void) const 
{
    
    // Get the first character in the matrix
    if ( getNumberOfTaxa() == 0 )
    {
        return 0;
    }
    
    const DiscreteTaxonData<charType>& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
    {
        return 0;
    }
    
    return sequence[0].getNumberOfStates();
}


/**
 * Get the set of excluded character indices.
 *
 * \return    The excluded character indices.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getNumberOfInvariantSites(void) const
{
    size_t invSites = 0;
    size_t nt = this->getNumberOfTaxa();

    const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(0);
    size_t nc = firstTaxonData.getNumberOfCharacters();
    for (size_t j=0; j<nc; j++)
    {
        const DiscreteCharacterState& a = firstTaxonData[j];
        bool invariant = true;
        for (size_t i=1; i<nt; i++)
        {
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(i);
            const DiscreteCharacterState& b = secondTaxonData[j];

            invariant &= (a == b);
        }
        
        if (invariant)
        {
            ++invSites;
        }

    }
    
    return invSites;
}


/**
 * Get the set of excluded character indices.
 * We use the fact that the number of segregating sites is numSites - numInvariantSites.
 *
 * \return    The excluded character indices.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getNumberOfSegregatingSites(void) const
{
    const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(0);
    size_t nc = firstTaxonData.getNumberOfCharacters();
    
    return nc - getNumberOfInvariantSites();
}


/**
 * Get the average pairwise distance between the sequences.
 *
 * \return    The average pairwise distance.
 */
template<class charType>
double RevBayesCore::HomologousDiscreteCharacterData<charType>::getAveragePaiwiseSequenceDifference(void) const
{
    double pairwiseDistance = 0.0;
    size_t nt = this->getNumberOfIncludedTaxa();
    
    
    for (size_t i=0; i<(nt-1); i++)
    {
        
        const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(i);
        size_t nc = firstTaxonData.getNumberOfCharacters();
        
        for (size_t j=i+1; j<nt; j++)
        {
            
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(j);
            double pd = 0.0;
            
            for (size_t k=0; k<nc; k++)
            {
                const DiscreteCharacterState& a = firstTaxonData[k];
                const DiscreteCharacterState& b = secondTaxonData[k];
                if (a != b)
                {
                    ++pd;
                }
            }
        
            pairwiseDistance += pd;
            
        } // end loop over all second taxa
    
    } // end loop over all first taxa
    
    pairwiseDistance *= 2.0 / (nt * (nt - 1.0 ) );
    
    return pairwiseDistance;
}


/**
 * Get the maximum pairwise distance between the sequences.
 *
 * \return    The max pairwise distance.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getMaxPaiwiseSequenceDifference(void) const
{
    size_t max_pd = 0.0;
    size_t nt = this->getNumberOfIncludedTaxa();
    
    
    for (size_t i=0; i<(nt-1); i++)
    {
        
        const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(i);
        size_t nc = firstTaxonData.getNumberOfCharacters();
        
        for (size_t j=i+1; j<nt; j++)
        {
            
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(j);
            size_t pd = 0.0;
            
            for (size_t k=0; k<nc; k++)
            {
                const DiscreteCharacterState& a = firstTaxonData[k];
                const DiscreteCharacterState& b = secondTaxonData[k];
                if (a != b)
                {
                    ++pd;
                }
            }
            
            if ( max_pd < pd )
            {
                max_pd = pd;
            }
            
        } // end loop over all second taxa
        
    } // end loop over all first taxa
    
    
    return max_pd;
}


/**
 * Get the minimum pairwise distance between the sequences.
 *
 * \return    The min pairwise distance.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::getMinPaiwiseSequenceDifference(void) const
{
    size_t min_pd = RbConstants::Size_t::max;
    size_t nt = this->getNumberOfIncludedTaxa();
    
    
    for (size_t i=0; i<(nt-1); i++)
    {
        
        const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(i);
        size_t nc = firstTaxonData.getNumberOfCharacters();
        
        for (size_t j=i+1; j<nt; j++)
        {
            
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(j);
            size_t pd = 0.0;
            
            for (size_t k=0; k<nc; k++)
            {
                const DiscreteCharacterState& a = firstTaxonData[k];
                const DiscreteCharacterState& b = secondTaxonData[k];
                if (a != b)
                {
                    ++pd;
                }
            }
            
            if ( min_pd > pd )
            {
                min_pd = pd;
            }
            
        } // end loop over all second taxa
        
    } // end loop over all first taxa
    
    
    return min_pd;
}


/** 
 * Get the taxon data object with index tn.
 *
 * \return     A const reference to the taxon data object at position tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn ) const 
{
    
    if ( tn >= getNumberOfTaxa() )
    {
        throw RbException( "Taxon index out of range" );
    }
    
    const std::string& name = taxa[tn].getName();
    const typename std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() ) 
    {
        return static_cast<const DiscreteTaxonData<charType>&>(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the HomologousDiscreteCharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object at position tn.
 *
 * \return     A non-const reference to the taxon data object at position tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn ) 
{
    
    if ( tn >= getNumberOfTaxa() )
    {
        throw RbException( "Taxon index out of range" );
    }
    
    const std::string& name = taxa[tn].getName();
    const typename std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() ) 
    {
        return static_cast<DiscreteTaxonData<charType>&>(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + name + "' in the HomologousDiscreteCharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object with name tn.
 *
 * \return     A non-const reference to the taxon data object with name tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn ) const 
{
    
    if ( tn == "" ) 
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const typename std::map<std::string, AbstractTaxonData* >::const_iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() ) 
    {
        return static_cast<const DiscreteTaxonData<charType>&>(*i->second);
    }
    else 
    {
        throw RbException("Cannot find taxon '" + tn + "' in the HomologousDiscreteCharacterData matrix.");
    }
    
}


/** 
 * Get the taxon data object with name tn.
 *
 * \return     A const reference to the taxon data object with name tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::HomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn ) 
{
    
    
    if ( tn == "" ) 
    {
        throw RbException("Ambiguous taxon name.");
    }
	
    const typename std::map<std::string, AbstractTaxonData* >::iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() ) 
    {
        return static_cast< DiscreteTaxonData<charType>& >( *i->second );
    }
    else 
    {
        throw RbException("Cannot find taxon '" + tn + "' in the HomologousDiscreteCharacterData matrix.");
    }
    
}


/**
 * Include a character.
 * Since we didn't actually deleted the character but marked it for exclusion
 * we can now simply remove the flag.
 *
 * \param[in]    i    The position of the character that will be included.
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::includeCharacter(size_t i)
{
    
    if (i >= getTaxonData( 0 ).size() )
    {
        std::stringstream o;
        o << "Only " << getNumberOfCharacters() << " characters in matrix";
        throw RbException( o.str() );
    }
    
    
    deletedCharacters.erase( i );
    
}


/**
 * Initialize this object from a file
 *
 * \param[in]   idx    The site at which we want to know if it is constant?
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::initFromFile(const std::string &dir, const std::string &fn)
{
    RbFileManager fm = RbFileManager(dir, fn + ".nex");
    
    // get an instance of the NCL reader
    NclReader reader = NclReader();
    
    std::string myFileType = "nexus";
    std::string dType = this->getDataType();
    
    std::string suffix = "|" + dType;
    suffix += "|noninterleaved";
    myFileType += suffix;
            
            // read the content of the file now
    std::vector<AbstractCharacterData*> m_i = reader.readMatrices( fm.getFullFileName(), myFileType );
    
    if ( m_i.size() < 1 )
    {
        throw RbException("Could not read character data matrix from file \"" + fm.getFullFileName() + "\".");
    }
    
    HomologousDiscreteCharacterData<charType> *coreM = static_cast<HomologousDiscreteCharacterData<charType> *>( m_i[0] );
    
    *this = *coreM;
    
    delete coreM;
    
}


/**
 * Initialize this object from a file
 *
 * \param[in]   idx    The site at which we want to know if it is constant?
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::initFromString(const std::string &s)
{
    throw RbException("Cannot initialize a discrete character data matrix from a string.");
}



/** 
 * Is this character pattern constant at site idx?
 * 
 * \param[in]   idx    The site at which we want to know if it is constant?
 */
template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::isCharacterConstant(size_t idx) const 
{
    
    const CharacterState* f = NULL;
    for ( size_t i=0; i<getNumberOfTaxa(); i++ ) 
    {
        if ( isTaxonExcluded(i) == false ) 
        {
            if ( f == NULL )
                f = &getCharacter( i, idx );
            else 
            {
                const CharacterState* s = &getCharacter( i , idx );
                if ( (*f) != (*s) )
                    return false;
            }
        }
    }
    
    return true;
}


/** 
 * Is the character excluded?
 *
 * \param[in]    i   The position of the character.
 */
template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::isCharacterExcluded(size_t i) const 
{
    
	std::set<size_t>::const_iterator it = deletedCharacters.find( i );
	if ( it != deletedCharacters.end() )
    {
		return true;
    }
    
    return false;
}


template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::isCharacterResolved(size_t txIdx, size_t chIdx) const {

    const RevBayesCore::DiscreteTaxonData<charType>& td = getTaxonData(txIdx);
    return td.isCharacterResolved(chIdx);
}

template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::isCharacterResolved(const std::string &tn, size_t chIdx) const {

    const RevBayesCore::DiscreteTaxonData<charType>& td = getTaxonData(tn);
    return td.isCharacterResolved(chIdx);
}

/** 
 * Does the character have missing or ambiguous characters?
 *
 * \param[in]    idx    The position of the character in question.
 */
template<class charType>
bool RevBayesCore::HomologousDiscreteCharacterData<charType>::isCharacterMissingOrAmbiguous(size_t idx) const 
{
    
    for ( size_t i=0; i<getNumberOfTaxa(); i++ )
    {
        if ( isTaxonExcluded(i) == false )
        {
            const CharacterState& c = getCharacter( i, idx );
            if ( c.isGapState() == true || c.isAmbiguous() == true )
                return true;
        }
    }
    
    return false;
}


/**
 * Compute the maximum gc content for any taxon
 *
 * \return    The max GC content.
 */
template<class charType>
double RevBayesCore::HomologousDiscreteCharacterData<charType>::maxGcContent( void ) const
{
    double max_gc = 0;
    size_t nt = this->getNumberOfTaxa();
    
    DnaState G = DnaState("G");
    DnaState C = DnaState("C");
    
    for (size_t i=0; i<nt; i++)
    {
        int num_gc = 0;
        const AbstractDiscreteTaxonData& taxonData = this->getTaxonData(i);
        size_t nc = taxonData.getNumberOfCharacters();
        for (size_t j=0; j<nc; j++)
        {
            const DiscreteCharacterState& b = taxonData[j];
            if ( b == G || b == C )
            {
                ++num_gc;
            }
            
        }
        
        double this_gc_proportion = double(num_gc) / double(nc);
        if ( max_gc < this_gc_proportion )
        {
            max_gc = this_gc_proportion;
        }
        
    }
    
    return max_gc;
}


/**
 * Compute the maximum length of an invariable block.
 *
 * \return    The max block length.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::maxInvariableBlockLength(void) const
{
    size_t max_length = 0;
    size_t nt = this->getNumberOfTaxa();
    
    const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(0);
    size_t nc = firstTaxonData.getNumberOfCharacters();
    
    size_t this_block_length = 0;
    for (size_t j=0; j<nc; j++)
    {
        const DiscreteCharacterState& a = firstTaxonData[j];
        bool invariant = true;
        for (size_t i=1; i<nt; i++)
        {
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(i);
            const DiscreteCharacterState& b = secondTaxonData[j];
            
            invariant &= (a == b);
        }
        
        if ( invariant == true )
        {
            ++this_block_length;
        }
        else
        {
            if ( this_block_length > max_length )
            {
                max_length = this_block_length;
            }
            this_block_length = 0;
            
        }
        
    }
    
    return max_length;
}


/**
 * Compute the maximum length of a variable block.
 *
 * \return    The max block length.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::maxVariableBlockLength(void) const
{
    size_t max_length = 0;
    size_t nt = this->getNumberOfTaxa();
    
    const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(0);
    size_t nc = firstTaxonData.getNumberOfCharacters();
    
    size_t this_block_length = 0;
    for (size_t j=0; j<nc; j++)
    {
        const DiscreteCharacterState& a = firstTaxonData[j];
        bool invariant = true;
        for (size_t i=1; i<nt; i++)
        {
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(i);
            const DiscreteCharacterState& b = secondTaxonData[j];
            
            invariant &= (a == b);
        }
        
        if ( invariant == false )
        {
            ++this_block_length;
        }
        else
        {
            if ( this_block_length > max_length )
            {
                max_length = this_block_length;
            }
            this_block_length = 0;
            
        }
        
    }
    
    return max_length;
}


/**
 * Compute the minimum gc content for any taxon
 *
 * \return    The min GC content.
 */
template<class charType>
double RevBayesCore::HomologousDiscreteCharacterData<charType>::minGcContent( void ) const
{
    double min_gc = 1;
    size_t nt = this->getNumberOfTaxa();
    
    DnaState G = DnaState("G");
    DnaState C = DnaState("C");
    
    for (size_t i=0; i<nt; i++)
    {
        int num_gc = 0;
        const AbstractDiscreteTaxonData& taxonData = this->getTaxonData(i);
        size_t nc = taxonData.getNumberOfCharacters();
        for (size_t j=0; j<nc; j++)
        {
            const DiscreteCharacterState& b = taxonData[j];
            if ( b == G || b == C )
            {
                ++num_gc;
            }
            
        }
        
        double this_gc_proportion = double(num_gc) / double(nc);
        if ( min_gc > this_gc_proportion )
        {
            min_gc = this_gc_proportion;
        }
        
    }
    
    return min_gc;
}


/**
 * Compute the maximum length of an invariable block.
 *
 * \return    The max block length.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::numInvariableSiteBlocks(void) const
{
    size_t num_blocks = 0;
    size_t nt = this->getNumberOfTaxa();
    
    const AbstractDiscreteTaxonData& firstTaxonData = this->getTaxonData(0);
    size_t nc = firstTaxonData.getNumberOfCharacters();
    bool previous_site_was_invariable = false;
    for (size_t j=0; j<nc; j++)
    {
        const DiscreteCharacterState& a = firstTaxonData[j];
        bool invariant = true;
        for (size_t i=1; i<nt; i++)
        {
            const AbstractDiscreteTaxonData& secondTaxonData = this->getTaxonData(i);
            const DiscreteCharacterState& b = secondTaxonData[j];
            
            invariant &= (a == b);
        }
        
        if ( previous_site_was_invariable == false && invariant == true )
        {
            ++num_blocks;
        }
        
        previous_site_was_invariable = invariant;
        
    }
    
    return num_blocks;
}



/** 
 * Calculates and returns the number of constant characters.
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::numConstantPatterns( void ) const 
{
    
    size_t nc = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
    {
        if ( isCharacterExcluded(i) == false && isCharacterConstant(i) == true )
            nc++;
    }
    
    return nc;
}


/** 
 * Returns the number of characters with missing or ambiguous data
 */
template<class charType>
size_t RevBayesCore::HomologousDiscreteCharacterData<charType>::numMissAmbig(void) const 
{
    
    size_t nma = 0;
    for (size_t i=0; i<getNumberOfCharacters(); i++)
    {
        if ( isCharacterExcluded(i) == false && isCharacterMissingOrAmbiguous(i) == true )
            nma++;
    }
    
    return nma;
}


/**
 * Remove all the excluded character.
 *
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::removeExludedCharacters( void )
{
    
    for (typename std::map<std::string, AbstractTaxonData* >::iterator it = taxonMap.begin(); it != taxonMap.end(); ++it)
    {
        it->second->removeCharacters( deletedCharacters );
    }
    
    deletedCharacters.clear();
    
}


/** 
 * Restore a character. We simply do not mark the character as excluded anymore.
 *
 * \param[in]    i    The position of the character to restore.
 */
template<class charType>
void RevBayesCore::HomologousDiscreteCharacterData<charType>::restoreCharacter(size_t i) 
{
    
    if (i >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    deletedCharacters.erase( i );
    
}



#endif

