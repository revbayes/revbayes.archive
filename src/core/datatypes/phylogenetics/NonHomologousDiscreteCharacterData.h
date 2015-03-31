#ifndef NonHomologousDiscreteCharacterData_H
#define NonHomologousDiscreteCharacterData_H

#include "NonHomologousCharacterData.h"
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
    class NonHomologousDiscreteCharacterData : public NonHomologousCharacterData {
        
    public:
        NonHomologousDiscreteCharacterData();                                                                                        //!< Default constructor
        
        // Overloaded operators
        const DiscreteTaxonData<charType>&                  operator[](size_t i) const;                                                 //!< Subscript operator (const)
        bool                                                operator<(const NonHomologousDiscreteCharacterData& x) const;               //!< Less than operator
        
        // implemented methods of the Cloneable interface
        NonHomologousDiscreteCharacterData<charType>*       clone(void) const;
        
        // Container functions
        void                                                clear(void);
        
        // CharacterData functions
        void                                                addTaxonData(const AbstractTaxonData &obs);                                 //!< Add taxon data
        void                                                addTaxonData(const AbstractDiscreteTaxonData &obs);                         //!< Add discrete taxon data
        void                                                addTaxonData(const DiscreteTaxonData<charType> &obs);                       //!< Add taxon data
        NonHomologousDiscreteCharacterData&                 concatenate(const AbstractCharacterData &d);                                //!< Concatenate data matrices
        NonHomologousDiscreteCharacterData&                 concatenate(const NonHomologousCharacterData &d);                           //!< Concatenate data matrices
        NonHomologousDiscreteCharacterData&                 concatenate(const NonHomologousDiscreteCharacterData &d);                   //!< Concatenate data matrices
        void                                                excludeTaxon(size_t i);                                                     //!< Exclude taxon
        void                                                excludeTaxon(const std::string& s);                                         //!< Exclude taxon
        const charType&                                     getCharacter(size_t tn, size_t cn) const;                                   //!< Return a reference to a character element in the character matrix
        std::string                                         getDatatype(void) const;
        std::vector<double>                                 getEmpiricalBaseFrequencies(void) const;                                    //!< Compute the empirical base frequencies
        const std::set<size_t>&                             getExcludedCharacters(void) const;                                          //!< Returns the name of the file the data came from
        const std::string&                                  getFileName(void) const;                                                    //!< Returns the name of the file the data came from
        const std::string&                                  getFilePath(void) const;                                                    //!< Returns the name of the file the data came from
        size_t                                              getIndexOfTaxon(const std::string &n) const;                                //!< Get the index of the taxon with name 'n'.
        std::vector<size_t>                                 getNumberOfCharacters(void) const;                                          //!< Number of characters
        size_t                                              getNumberOfCharacters(size_t i) const;                                          //!< Number of characters
        size_t                                              getNumberOfStates(void) const;                                              //!< Get the number of states for the characters in this matrix
        size_t                                              getNumberOfTaxa(void) const;                                                //!< Number of taxa
        size_t                                              getNumberOfIncludedTaxa(void) const;                                        //!< Number of included taxa
        DiscreteTaxonData<charType>&                        getTaxonData(size_t tn);                                                    //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(size_t tn) const;                                              //!< Return a reference to a sequence in the character matrix
        DiscreteTaxonData<charType>&                        getTaxonData(const std::string &tn);                                        //!< Return a reference to a sequence in the character matrix
        const DiscreteTaxonData<charType>&                  getTaxonData(const std::string &tn) const;                                  //!< Return a reference to a sequence in the character matrix
        const std::vector<std::string>&                     getTaxonNames(void) const;                                                  //!< Get the names of the taxa
        const std::string&                                  getTaxonNameWithIndex(size_t idx) const;                                    //!< Returns the idx-th taxon name
        bool                                                isTaxonExcluded(size_t i) const;                                            //!< Is the taxon excluded
        bool                                                isTaxonExcluded(const std::string& s) const;                                //!< Is the taxon excluded
        void                                                restoreTaxon(size_t i);                                                     //!< Restore taxon
        void                                                restoreTaxon(const std::string& s);                                         //!< Restore taxon
        void                                                setFileName(const std::string &fn);                                         //!< Set the file name
        void                                                setFilePath(const std::string &fn);                                         //!< Set the file name
        void                                                setTaxonName(const std::string& currentName, const std::string& newName);   //!< Change the name of a taxon
        void                                                show(std::ostream &out);                                                    //!< Show the entire content
        void                                                updateNames();                                                              //!< Update the sequence names when individual taxa have changed names
        
        
    protected:
        // Utility functions
        size_t                                              indexOfTaxonWithName(const std::string& s) const;                           //!< Get the index of the taxon
        
        // Member variables
        std::set<size_t>                                    deletedTaxa;                                                                //!< Set of deleted taxa
        std::string                                         fileName;                                                                   //!< The path/filename from where this matrix originated
        std::string                                         filePath;                                                                   //!< The path/filename from where this matrix originated
        std::vector<std::string>                            sequenceNames;                                                              //!< names of the sequences
        
        std::map<std::string, DiscreteTaxonData<charType> > taxonMap;
        
    };
    
    
}


#include "DiscreteCharacterState.h"
#include "DiscreteTaxonData.h"
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
RevBayesCore::NonHomologousDiscreteCharacterData<charType>::NonHomologousDiscreteCharacterData()
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
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::operator[]( const size_t i ) const
{
    
    return getTaxonData( i );
}



/**
 * Less than operator.
 */
template<class charType>
bool RevBayesCore::NonHomologousDiscreteCharacterData<charType>::operator<(const NonHomologousDiscreteCharacterData<charType> &x) const
{
    
    return sequenceNames.size() < x.sequenceNames.size();
}


/**
 * Add another character data object to this character data object.
 *
 * \param[in]    obsd    The CharacterData object that should be added.
 */
template<class charType>
RevBayesCore::NonHomologousDiscreteCharacterData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::concatenate(const AbstractCharacterData &obsd)
{
    
    const NonHomologousDiscreteCharacterData<charType>* rhs = dynamic_cast<const NonHomologousDiscreteCharacterData<charType>* >( &obsd );
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
RevBayesCore::NonHomologousDiscreteCharacterData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::concatenate(const NonHomologousCharacterData &obsd)
{
    
    const NonHomologousDiscreteCharacterData<charType>* rhs = dynamic_cast<const NonHomologousDiscreteCharacterData<charType>* >( &obsd );
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
RevBayesCore::NonHomologousDiscreteCharacterData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::concatenate(const NonHomologousDiscreteCharacterData<charType> &obsd)
{
    
    // check if both have the same number of taxa
    if ( sequenceNames.size() != obsd.getNumberOfTaxa() )
    {
        throw RbException("Cannot concatenate two character data objects with different number of taxa!");
    }
    
    std::vector<bool> used = std::vector<bool>(obsd.getNumberOfTaxa(),false);
    for (size_t i=0; i<sequenceNames.size(); i++ )
    {
        const std::string &n = sequenceNames[i];
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
    
    
    // return a reference to this object
    return *this;
}


/**
 * Add a sequence (TaxonData) to the character data object.
 *
 * \param[in]    obsd    The TaxonData object that should be added.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::addTaxonData(const AbstractTaxonData &obsd)
{
    
    if ( dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd ) == NULL )
    {
        throw RbException("Inserting wrong character type into CharacterData!!!");
    }
    
    // delegate the call to the specialized method
    addTaxonData( static_cast<const DiscreteTaxonData<charType>& >( obsd ) );
    
}


/**
 * Add a sequence (TaxonData) to the character data object.
 *
 * \param[in]    obsd    The TaxonData object that should be added.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::addTaxonData(const AbstractDiscreteTaxonData &obsd)
{
    
    if ( dynamic_cast<const DiscreteTaxonData<charType>* >( &obsd ) == NULL )
    {
        throw RbException("Inserting wrong character type into CharacterData!!!");
    }
    
    // delegate the call to the specialized method
    addTaxonData( static_cast<const DiscreteTaxonData<charType>& >( obsd ) );
    
}


/**
 * Add a sequence (TaxonData) to the character data object.
 *
 * \param[in]    obsd    The TaxonData object that should be added.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::addTaxonData(const DiscreteTaxonData<charType> &obs)
{
    
    // add the sequence name to the list
    sequenceNames.push_back( obs.getTaxonName() );
    
    // add the sequence also as a member so that we can access it by name
    taxonMap.insert( std::pair<std::string, DiscreteTaxonData<charType> >( obs.getTaxonName(), obs ) );
    
}



/**
 * Clear the object, that is, remove all TaxonData elements.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::clear( void )
{
    
    sequenceNames.clear();
    taxonMap.clear();
    
}


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'b'.
 *
 * \return A new copy of the object.
 */
template<class charType>
RevBayesCore::NonHomologousDiscreteCharacterData<charType>* RevBayesCore::NonHomologousDiscreteCharacterData<charType>::clone( void ) const
{
    
    return new NonHomologousDiscreteCharacterData<charType>(*this);
}



/**
 * Exclude a taxon.
 * We don't actually delete the taxon but instead mark it for exclusion.
 *
 * \param[in]    i    The index of the taxon that will be excluded.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::excludeTaxon(size_t i)
{
    
    if (i >= taxonMap.size())
    {
        std::stringstream o;
        o << "Only " << taxonMap.size() << " taxa in matrix";
        throw RbException( o.str() );
    }
    
    deletedTaxa.insert( i );
}


/**
 * Exclude a taxon.
 * We don't actually delete the taxon but instead mark it for exclusion.
 *
 * \param[in]    s    The name of the taxon that will be excluded.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::excludeTaxon(const std::string& s)
{
    
    for (size_t i = 0; i < getNumberOfTaxa(); i++)
    {
        if (s == sequenceNames[i] )
        {
            deletedTaxa.insert( i );
            break;
        }
    }
    
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
const charType& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getCharacter( size_t tn, size_t cn ) const
{
    
    if ( cn >= getNumberOfCharacters() )
        throw RbException( "Character index out of range" );
    
    return getTaxonData( tn )[cn];
}


/**
 * Get the data type of the character stored in this object.
 *
 * \return      The data type (e.g. DNA, RNA or Standard).
 */
template<class charType>
std::string RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getDatatype(void) const
{
    
    std::string dt = "";
    if ( sequenceNames.size() > 0 )
    {
        const DiscreteTaxonData<charType> &t = getTaxonData( sequenceNames[0] );
        if ( t.size() > 0 )
        {
            dt = t[0].getDatatype();
        }
        
    }
    
    return dt;
}



/**
 * Get the file name from whcih the character data object was read in.
 *
 * \return    The original file name.
 */
template<class charType>
const std::string& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getFileName(void) const
{
    
    return fileName;
}

/**
 * Get the file path from which the character data object was read in.
 *
 * \return    The original file path.
 */
template<class charType>
const std::string& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getFilePath(void) const
{
    
    return filePath;
}


/**
 * Get the index of the taxon with name 'n'.
 *
 * \par[in]     The name.
 * \return      The index.
 *
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getIndexOfTaxon(const std::string &n) const
{
    long pos = std::find(sequenceNames.begin(), sequenceNames.end(), n) - sequenceNames.begin();
    
    if ( pos == sequenceNames.size() )
    {
        return RbConstants::Size_t::inf;
    }
    
    return size_t( pos );
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
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfCharacters(size_t i) const
{
    
    if (getNumberOfTaxa() > i)
    {
        return getTaxonData(i).getNumberOfCharacters();
    }
    else
    {
        throw RbException("No taxon found for this index.");
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
std::vector<size_t> RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfCharacters( void ) const
{
    
    
    std::vector<size_t> indices;
    for (size_t i = 0; i < getNumberOfTaxa(); ++i)
    {
        indices.push_back( getTaxonData( i ).getNumberOfCharacters() );
    }
    
    return indices;
}


/**
 * Get the number of states for the characters in this object.
 * We assume that all of the characters in the matrix are of the same
 * type and have the same number of potential states.
 *
 * \return      The number of states for the characters.
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfStates(void) const
{
    
    // Get the first character in the matrix
    if ( getNumberOfTaxa() == 0 )
        return 0;
    
    const DiscreteTaxonData<charType>& sequence = getTaxonData( 0 );
    if ( sequence.getNumberOfCharacters() == 0 )
        return 0;
    
    return sequence[0].getNumberOfStates();
}


/**
 * Get the number of taxa currently stored in this object.
 *
 * \return       The number of taxa.
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfTaxa(void) const
{
    
    return sequenceNames.size();
}


/**
 * Get the number of included taxa currently stored in this object.
 *
 * \return       The number of included taxa.
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getNumberOfIncludedTaxa(void) const
{
    if (getNumberOfTaxa() > 0)
    {
        return getNumberOfTaxa() - deletedTaxa.size();
    }
    return 0;
    
}


/**
 * Get the taxon data object with index tn.
 *
 * \return     A const reference to the taxon data object at position tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn ) const
{
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, DiscreteTaxonData<charType> >::const_iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() )
    {
        return i->second;
    }
    else
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object at position tn.
 *
 * \return     A non-const reference to the taxon data object at position tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( size_t tn )
{
    
    if ( tn >= getNumberOfTaxa() )
        throw RbException( "Taxon index out of range" );
    
    const std::string& name = sequenceNames[tn];
    const typename std::map<std::string, DiscreteTaxonData<charType> >::iterator& i = taxonMap.find( name );
    
    if (i != taxonMap.end() )
    {
        return i->second;
    }
    else
    {
        throw RbException("Cannot find taxon '" + name + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object with name tn.
 *
 * \return     A non-const reference to the taxon data object with name tn.
 */
template<class charType>
const RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn ) const
{
    
    if ( tn == "" )
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const typename std::map<std::string, DiscreteTaxonData<charType> >::const_iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() )
    {
        return i->second;
    }
    else
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the taxon data object with name tn.
 *
 * \return     A const reference to the taxon data object with name tn.
 */
template<class charType>
RevBayesCore::DiscreteTaxonData<charType>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonData( const std::string &tn )
{
    
    
    if ( tn == "" )
    {
        throw RbException("Ambiguous taxon name.");
    }
    
    const typename std::map<std::string, DiscreteTaxonData<charType> >::iterator& i = taxonMap.find(tn);
    
    if (i != taxonMap.end() )
    {
        return i->second;
    }
    else
    {
        throw RbException("Cannot find taxon '" + tn + "' in the CharacterData matrix.");
    }
    
}


/**
 * Get the names of all taxa.
 *
 * \return     A vector of all taxon names.
 */
template<class charType>
const std::vector<std::string>& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonNames( void ) const
{
    
    return sequenceNames;
}



/**
 * Get the taxon name with index idx.
 *
 * \param[in]    idx    The position of the taxon.
 *
 * \return              The name of the taxon.
 */
template<class charType>
const std::string& RevBayesCore::NonHomologousDiscreteCharacterData<charType>::getTaxonNameWithIndex( size_t idx ) const
{
    
    return sequenceNames[idx];
}



/**
 * Get the index of the taxon with name s.
 *
 * \param[in]    s    The name of the taxon.
 *
 * \return            The index of the taxon.
 */
template<class charType>
size_t RevBayesCore::NonHomologousDiscreteCharacterData<charType>::indexOfTaxonWithName( const std::string& s ) const
{
    
    // search through all names
    for (size_t i=0; i<sequenceNames.size(); i++)
    {
        if (s == sequenceNames[i] )
        {
            return i;
        }
    }
    
    return RbConstants::Size_t::nan;
}


/**
 * Is the taxon excluded.
 *
 * \param[in]    idx    The position of the taxon in question.
 */
template<class charType>
bool RevBayesCore::NonHomologousDiscreteCharacterData<charType>::isTaxonExcluded(size_t i) const
{
    
    std::set<size_t>::const_iterator it = deletedTaxa.find( i );
    if ( it != deletedTaxa.end() )
        return true;
    
    return false;
}


/**
 * Is the taxon excluded?
 *
 * \param[in]    s    The name of the taxon in question.
 */
template<class charType>
bool RevBayesCore::NonHomologousDiscreteCharacterData<charType>::isTaxonExcluded(const std::string& s) const
{
    
    size_t i = indexOfTaxonWithName(s);
    std::set<size_t>::const_iterator it = deletedTaxa.find( i );
    if ( it != deletedTaxa.end() )
        return true;
    
    return false;
}


/**
 * Restore a taxon. We simply do not mark the taxon as excluded anymore
 *
 *
 * \param[in]    i    The position of the taxon in question.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::restoreTaxon(size_t i)
{
    
    if ( i >= getNumberOfTaxa() )
        return;
    
    deletedTaxa.erase( i );
    
}


/**
 * Restore a taxon. We simply do not mark the taxon as excluded anymore.
 *
 * \param[in]    s    The name of the taxon in question.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::restoreTaxon(const std::string& s)
{
    
    size_t i = indexOfTaxonWithName( s );
    
    deletedTaxa.erase( i );
    
}


/**
 * Set the original file name for this character data object.
 *
 * \param[in]    fn    The new file name.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::setFileName(const std::string& fn)
{
    
    fileName = fn;
    
}


/**
 * Set the original file name for this character data object.
 *
 * \param[in]    fn    The new file name.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::setFilePath(const std::string& fn)
{
    
    filePath = fn;
    
}


/**
 * Change the name of a taxon
 *
 * \param[in] currentName    self explanatory.
 * \param[in] newName        self explanatory.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::setTaxonName(const std::string& currentName, const std::string& newName)
{
    DiscreteTaxonData<charType> t = getTaxonData( currentName );
    t.setTaxonName(newName);
    
    size_t numTax = sequenceNames.size();
    for (size_t i = 0; i < numTax ; ++i)
    {
        if ( sequenceNames[i] == currentName) {
            sequenceNames[i] = newName;
            break;
        }
    }
    taxonMap.erase( currentName );
    taxonMap.insert( std::pair<std::string, DiscreteTaxonData<charType> >( newName, t ) );
    
}


/**
 * Print the content of the data matrix.
 */
template<class charType>
void RevBayesCore::NonHomologousDiscreteCharacterData<charType>::show(std::ostream &out)
{
    
    size_t nt = this->getNumberOfTaxa();
    for (size_t i=0; i<nt; i++)
    {
        
        const AbstractDiscreteTaxonData& taxonData = this->getTaxonData(i);
        std::string taxonName = this->getTaxonNameWithIndex(i);
        size_t nc = taxonData.getNumberOfCharacters();
        std::cout << "   " << taxonName << std::endl;
        std::cout << "   ";
        for (size_t j=0; j<nc; j++)
        {
            
            const CharacterState& o = taxonData[j];
            std::string s = o.getStringValue();
            
            std::cout << s << " ";
            if ( (j+1) % 100 == 0 && (j+1) != nc )
            {
                std::cout << std::endl << "   ";
            }
            
        }
        
        std::cout << std::endl;
    }
    
}


#endif

