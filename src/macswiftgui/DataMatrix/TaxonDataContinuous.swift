//
//  TaxonDataContinuous.swift
//
//  Created by John Huelsenbeck on 8/5/19.
//  Copyright © 2019 John Huelsenbeck. All rights reserved.
//
// This class holds continuous (i.e., morphological) character data for a taxon. The
// characters are stored as an array of doubles. The inheritance from Bitvector is a side
// affect of inheriting from TaxonData. The functionality of the Bitvector class is not
// used.

import Foundation


class TaxonDataContinuous : TaxonData, NSCopying {

    internal var variables : [Double]

    // MARK: -

    private enum CodingKeys: String, CodingKey {
    
        case variables
    }

    // MARK: -

    init() {
    
        self.variables = []
        super.init(numStates: 0, numFlags: 0)

        taxonName = ""
        numCharacters = 0
        dataType = DataType.Continuous
    }

    init(name: String, characterDataVector: [Double]) throws {

        self.variables = []
        for x in characterDataVector {
            self.variables.append(x)
        }
        super.init(numStates: 0, numFlags: 0)

        taxonName = name
        numCharacters = self.variables.count
        dataType = DataType.Continuous
    }
    
    required init(from decoder: Decoder) throws {
    
        do {
            let container = try decoder.container(keyedBy: CodingKeys.self)
            self.variables = try container.decode([Double].self, forKey: .variables)
            try super.init(from: container.superDecoder())
        }
        catch {
            throw TaxonDataError.decodingError
        }
    }

    override func encode(to encoder: Encoder) throws {

        do {
            var container = encoder.container(keyedBy: CodingKeys.self)
            try container.encode(variables, forKey: .variables)
            try super.encode(to:container.superEncoder())
        }
        catch {
            throw TaxonDataError.encodingError
        }
    }
    
    func copy(with zone: NSZone? = nil) -> Any {
    
        do {
            let copy = try TaxonDataContinuous(name: taxonName, characterDataVector: self.variables)
            return copy
        }
        catch {
            return TaxonDataContinuous()
        }
    }

    // MARK: -

    static func +=(lhs: TaxonDataContinuous, newVal: Double) throws {
    
        lhs.variables.append(newVal)
        lhs.numCharacters += 1
    }
}
