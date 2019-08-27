//
//  File.swift
//  macgui
//
//  Created by Svetlana Krasikova on 6/11/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Foundation
import Cocoa

class Connector: NSObject {
    var color: ConnectorColor
    var neighbor: Connectable?
    
    init(color: ConnectorColor, neighbor: Connectable){
        self.color = color
        self.neighbor = neighbor
    }
    init(color: ConnectorColor){
        self.color = color
        self.neighbor = nil
    }
    
    func setNeighbor(neighbor: Connectable){
        self.neighbor = neighbor
    }

    func getColor() -> NSColor {
        switch self.color {
        case .blue: return NSColor.blue
        case .green: return NSColor.green
        case .orange: return NSColor.orange
        case .red: return NSColor.red
        case .magenta: return NSColor.magenta
        }
    }
    
}
