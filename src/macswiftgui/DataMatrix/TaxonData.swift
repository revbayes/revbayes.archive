//
//  TaxonData.swift
//
//  Created by John Huelsenbeck on 7/11/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds information for a single taxon. Specifically, an object of this
// type (which serves as a base class for classes that deal with specific data types)
// holds the taxon name, the number of character states, and the number of characters.
// The actual character data is stored in the Bitvector class, a base class for this
// class. The information for a specific character is stored in the bitvector. For example,
// a DNA sequence would be stored using five bits: four bits to store the nucleotide and one
// bit to indicate whether the character is a gap or not. (The details of how this is done
// are found in classes that inherit from this one.

import Foundation


enum DataType: String, Codable {

    case DNA = "DNA"
    case RNA = "RNA"
    case Protein = "Protein"
    case Standard = "Standard"
    case Continuous = "Continuous"
    case Unknown = "Unknown"
}



class TaxonData : Bitvector {

    public   var numCharacters: Int // the number of characters for this taxon
    internal var numStates: Int     // the number of character states for the characters
    internal var numFlags: Int      // the number of additional flags per character
    public   var taxonName: String  // the name of the taxon
    internal var dataType: DataType // the data type

    // MARK: -
    
    private enum CodingKeys: String, CodingKey {
    
        case numCharacters
        case numStates
        case numFlags
        case taxonName
        case dataType
    }

    enum TaxonDataError: Error {
    
        case encodingError
        case decodingError
        case incorrectDataFormat
        case copyError
        case incorrectStateInformation
        case concatenationError
   }

    // MARK: -

    // initialize with the number of states
    init(numStates: Int, numFlags: Int) {
    
        self.numCharacters = 0
        self.numStates = numStates
        self.numFlags = numFlags
        self.taxonName = ""
        self.dataType = .Unknown
        super.init(numElements: 0)
    }

    // initialize with the number of characters and states
    init(numCharacters: Int, numStates: Int, numFlags: Int) {
    
        self.numCharacters = numCharacters
        self.numStates = numStates
        self.numFlags = numFlags
        self.taxonName = ""
        self.dataType = .Unknown
        super.init(numElements: numCharacters*(numStates+numFlags) )
    }
    
    // initialize from serialized data
    required init(from decoder: Decoder) throws {
 
        print("TaxonData decoder")

        do {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            self.numCharacters = try container.decode(Int.self,    forKey: .numCharacters)
            self.numStates     = try container.decode(Int.self,    forKey: .numStates)
            self.numFlags      = try container.decode(Int.self,    forKey: .numFlags)
            self.taxonName     = try container.decode(String.self, forKey: .taxonName)
            self.dataType      = try container.decode(DataType.self, forKey: .dataType)
            try super.init(from: container.superDecoder())
        }
        catch {
            throw TaxonDataError.decodingError
        }
    }

    // encode the object for serialization
    override func encode(to encoder: Encoder) throws {

        do {
            var container = encoder.container(keyedBy: CodingKeys.self)
            try super.encode(to:container.superEncoder())
            
            try container.encode(numCharacters, forKey: .numCharacters)
            try container.encode(numStates,     forKey: .numStates)
            try container.encode(numFlags,      forKey: .numFlags)
            try container.encode(taxonName,     forKey: .taxonName)
            try container.encode(dataType,      forKey: .dataType)
        }
        catch {
            throw TaxonDataError.encodingError
        }
    }

    // MARK: -

    static func +(lhs: TaxonData, rhs: TaxonData) -> Any? {
    
        // DNA
        let tdDnaL = lhs as? TaxonDataDna
        if tdDnaL != nil {
            let tdDnaR = rhs as? TaxonDataDna
            if tdDnaR == nil {
                return nil
            }
            let newLhs = tdDnaL?.copy() as! TaxonDataDna
            let bvL = newLhs as Bitvector
            let bvR = rhs as Bitvector
            newLhs.numCharacters += rhs.numCharacters
            bvL += bvR
            return newLhs
        }
        
        // RNA
        let tdRnaL = lhs as? TaxonDataRna
        if tdRnaL != nil {
            let tdRnaR = rhs as? TaxonDataRna
            if tdRnaR == nil {
                return nil
            }
            let newLhs = tdRnaL?.copy() as! TaxonDataRna
            let bvL = newLhs as Bitvector
            let bvR = rhs as Bitvector
            newLhs.numCharacters += rhs.numCharacters
            bvL += bvR
            return newLhs
        }
        
        // protein
        let tdProteinL = lhs as? TaxonDataProtein
        if tdProteinL != nil {
            let tdProteinR = rhs as? TaxonDataProtein
            if tdProteinR == nil {
                return nil
            }
            let newLhs = tdProteinL?.copy() as! TaxonDataProtein
            let bvL = newLhs as Bitvector
            let bvR = rhs as Bitvector
            newLhs.numCharacters += rhs.numCharacters
            bvL += bvR
            return newLhs
        }
        
        // standard
        let tdStandardL = lhs as? TaxonDataStandard
        if tdStandardL != nil {
            let tdStandardR = rhs as? TaxonDataStandard
            if tdStandardR == nil {
                return nil
            }
            let newLhs = tdStandardL?.copy() as! TaxonDataStandard
            let bvL = newLhs as Bitvector
            let bvR = rhs as Bitvector
            newLhs.numCharacters += rhs.numCharacters
            bvL += bvR
            return newLhs
        }
        
        // continuous
        let tdContinuousL = lhs as? TaxonDataContinuous
        if tdContinuousL != nil {
            let tdContinuousR = rhs as? TaxonDataContinuous
            if tdContinuousR == nil {
                return nil
            }
            let newLhs = tdContinuousL?.copy() as! TaxonDataContinuous
            for x in tdContinuousR!.variables {
                newLhs.variables.append(x)
            }
            newLhs.numCharacters += rhs.numCharacters
            return newLhs
        }

        return nil
    }

    // override the += operator allowing a character data string to be appended to the current sequence
    static func +=(lhs: TaxonData, newVal: String) throws {
    
        if lhs.dataType == DataType.Continuous {
            print("Cannot append a string to continouos character data")
            throw TaxonDataError.concatenationError
        }

        // process the string, removing spaces and making certain the characters are uppercase
        var potentialCharacterStateString : String = newVal.uppercased()
        potentialCharacterStateString = potentialCharacterStateString.replacingOccurrences(of: " ", with: "")
        
        // check that the character state sequence is valid
        guard lhs.isValidCharacterStateString(potentialCharacterStateString:potentialCharacterStateString) == true else {
            print("Invalid character state string")
            throw TaxonDataError.concatenationError
        }
        
        // the string containing the character state sequence is good, so we can now add it
        lhs.numCharacters += potentialCharacterStateString.count
        for c in potentialCharacterStateString {
            lhs.addCharacter(characterState: c)
        }
        
    }

    // override the += operator allowing concatenation of two taxon data
    static func +=(lhs: TaxonData, rhs: TaxonData) throws {
    
        // check that they have the same name
        guard lhs.taxonName == rhs.taxonName else {
            print("Cannot concatenate taxon information if the names differ (\(lhs.taxonName) and \(rhs.taxonName))")
            throw TaxonDataError.concatenationError
        }
        
        // check that the data are of the same type
        guard lhs.dataType == rhs.dataType else {
            print("Cannot concatenate taxon information of different types")
            throw TaxonDataError.concatenationError
        }
        
        // special checks if the data type is Standard
        if lhs.dataType == DataType.Standard {
            let tdL = lhs as! TaxonDataStandard
            let tdR = rhs as! TaxonDataStandard
            
            // check that the state sets are the same
            guard tdL.possibleCharacterStates.count == tdR.possibleCharacterStates.count else {
                print("Cannot concatenate taxon information with different possible states")
                throw TaxonDataError.concatenationError
            }
            for c in tdL.possibleCharacterStates {
                guard tdR.possibleCharacterStates.contains(c) == true else {
                    print("Cannot concatenate taxon information with different possible states")
                    throw TaxonDataError.concatenationError
                }
            }
        }
        
        // concatenate
        if lhs.dataType == DataType.Continuous {
            let lhsCont = lhs as! TaxonDataContinuous
            let rhsCont = rhs as! TaxonDataContinuous
            for x in rhsCont.variables {
                lhsCont.variables.append(x)
            }
            lhs.numCharacters += rhs.numCharacters
        }
        else {
            let bvL = lhs as Bitvector
            let bvR = rhs as Bitvector
            lhs.numCharacters += rhs.numCharacters
            bvL += bvR
        }
    }
    
    // sensibly print the object's information
    override var description: String {
    
        let str: String = taxonName + " (" + self.getDataType() + ") \"" + characterDataString() + "\" [\(numCharacters)]\n \(bitString())"
        return str
    }

    subscript(index: Int) -> Character {
    
        get {
            if index > 0 && index < numCharacters {
                let c : Character = getCharacterState(index: index)
                return c
            }
            else {
                return " "
            }
        }
        
        set(newValue) {
        
        }
    }

    // MARK: -

    func addCharacter(characterState: Character) {

    }
    
    // get the character string (this should be overrided by classes inheriting from this one
    func characterDataString() -> String {
    
        return ""
    }

    // return the data type
    func getDataType() -> String {
    
        return dataType.rawValue
    }
    
    func getCharacterState(index: Int) -> Character {
    
        return " "
    }

    // return whether the character is a gap, or not
    func isGap(dataChar: Character) -> Bool {
    
        if dataChar == "-" {
            return true
        }
        return false
    }

    func isSameStateSpace(taxonData: TaxonData) -> Bool {
    
        return true
    }
    
    func isValidCharacterState(potentialCharacterState: Character) -> Bool {
    
        return false
    }

    // check that the sequence contained in the string represents a valid DNA sequence
    func isValidCharacterStateString(potentialCharacterStateString: String) -> Bool {
    
        for c in potentialCharacterStateString {
            if isValidCharacterState(potentialCharacterState: c) == false {
                return false
            }
        }
        return true
    }

    func setCharacterState(characterIdx: Int, characterState: Character) {
    
    }
    
}
