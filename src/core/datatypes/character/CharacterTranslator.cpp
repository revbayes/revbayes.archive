#include "CharacterTranslator.h"
#include "RbException.h"



using namespace RevBayesCore;

RevBayesCore::AbstractDiscreteTaxonData* CharacterTranslator::translateCharacters(const RevBayesCore::AbstractDiscreteTaxonData &d, const std::string &type)
{
    AbstractDiscreteTaxonData *trans_taxon_data = NULL;

//    options.push_back( "DNA" );
//    options.push_back( "RNA" );
//    options.push_back( "AA" );
//    options.push_back( "Pomo" );
//    options.push_back( "Protein" );
//    options.push_back( "Standard" );
//    options.push_back( "NaturalNumbers" );
//    options.push_back( "Restriction" );
    
    if ( type == "DNA" )
    {
        trans_taxon_data = translateToDna( d );
    }
    else if ( type == "RNA" )
    {
        trans_taxon_data = translateToRna( d );
    }
    else if ( type == "AA" )
    {
        if ( d.getCharacter(0).getDataType() == "DNA" )
        {
            trans_taxon_data = translateToAaFromDna( dynamic_cast< const DiscreteTaxonData<DnaState>& >(d) );
        }
        else
        {
            trans_taxon_data = translateToAa( d );
        }
    }
    else if ( type == "Protein" )
    {
        trans_taxon_data = translateToAa( d );
    }
    else
    {
        throw RbException("Cannot translate character data object of type \""+ d.getCharacter(0).getDataType() +"\" into type \"" + type + "\"" );
    }
    
    return trans_taxon_data;
}


DiscreteTaxonData<DnaState>* CharacterTranslator::translateToDna(const AbstractDiscreteTaxonData &d)
{
    throw RbException("Cannot translate character data object of type \""+ d.getCharacter(0).getDataType() +"\" into type \"DNA\"" );
    
    return NULL;
}


DiscreteTaxonData<RnaState>* CharacterTranslator::translateToRna(const AbstractDiscreteTaxonData &d)
{
    throw RbException("Cannot translate character data object of type \""+ d.getCharacter(0).getDataType() +"\" into type \"RNA\"" );
    
    return NULL;
}


DiscreteTaxonData<AminoAcidState>* CharacterTranslator::translateToAa(const AbstractDiscreteTaxonData &d)
{
    throw RbException("Cannot translate character data object of type \""+ d.getCharacter(0).getDataType() +"\" into type \"AA\"" );
    
    return NULL;
}


DiscreteTaxonData<AminoAcidState>* CharacterTranslator::translateToAaFromDna(const DiscreteTaxonData<DnaState> &d)
{
    size_t length = d.getNumberOfCharacters();
    
    DiscreteTaxonData<AminoAcidState> *trans_taxon_data = new DiscreteTaxonData<AminoAcidState>( d.getTaxon() );
    
    for (size_t i=0; i<(length-2); i+=3)
    {
        std::string codon_string = d.getCharacter(i).getStringValue();
        codon_string += d.getCharacter(i+1).getStringValue();
        codon_string += d.getCharacter(i+2).getStringValue();
        CodonState cs = CodonState( codon_string );
        
        trans_taxon_data->addCharacter( cs.getAminoAcidState() );
    }
    
    return trans_taxon_data;
}


