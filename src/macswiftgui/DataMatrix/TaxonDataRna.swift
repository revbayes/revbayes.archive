//
//  TaxonDataRna.swift
//
//  Created by John Huelsenbeck on 7/25/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds RNA sequence information for a taxon. This class relies heavily
// on the TaxonDataDna class from which it inherits. It simply overrides a few of the
// functions in its immediate base class, using `U' instead of `T'.

import Foundation



class TaxonDataRna : TaxonDataDna {

    // default initializer
    override init() {
    
        super.init()

        // initialize to type RNA
        dataType = DataType.RNA
    }
    
    // initialize from the taxon name and a string of nucleotides characters
    override init(name: String, nucleotideString: String) throws {
    
        do {
            try super.init(name: name, nucleotideString: nucleotideString)

            // initialize to type RNA
            dataType = DataType.RNA
            }
        catch {
            throw TaxonDataError.incorrectDataFormat
        }
    }

    // initialize from serialized data
    required init(from decoder: Decoder) throws {
    
        do {
        try super.init(from: decoder)
        }
        catch {
            throw TaxonDataError.decodingError
        }
    }

    override func copy(with zone: NSZone? = nil) -> Any {
    
        do {
            let copy = try TaxonDataRna(name: taxonName, nucleotideString: self.characterDataString())
            return copy
        }
        catch {
            return TaxonDataRna()
        }
    }

    // MARK: -

    // decode a vector of four Bool variables, returning the nucleotide using the IUPAC standard
    override func decodeNucleotideVector(nucVector: [Bool]) -> Character {
    
        let nuc: Character = super.decodeNucleotideVector(nucVector: nucVector)
        if nuc == "T" {
            return "U"
        }
        else {
            return nuc
        }
    }

    // return the IUPAC nucleotide character
    override func encodeNucleotide(nucChar: Character) -> Int {
    
        var rnaChar: Character = nucChar
        if nucChar == "U" {
            rnaChar = "T"
        }
        let nucCode: Int = super.encodeNucleotide(nucChar: rnaChar)
        return nucCode
    }

    // check that the state is valid
    override func isValidCharacterState(potentialCharacterState: Character) -> Bool {

        let validRnaCharacters = "ACGURYMKSWHBVDN?-"
        if validRnaCharacters.contains(potentialCharacterState) == false {
            return false
        }
        return true
    }

}
