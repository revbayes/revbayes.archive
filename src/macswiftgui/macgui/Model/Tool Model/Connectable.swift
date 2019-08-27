//
//  Connectable.swift
//  macgui
//
//  Created by Svetlana Krasikova on 4/9/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class Connectable: ToolObject {    
    var inlets: [Connector] = []
    var outlets: [Connector] = []
    
    var unconnectedInlets: [Connector] {
        get {
            return inlets.filter{$0.neighbor == nil}
        }
        
    }
    
    var unconnectedOutlets: [Connector] {
        get {
            return outlets.filter{$0.neighbor == nil}
        }
    }
    
    var connectedInlets: [Connector] {
        get {
            return inlets.filter{$0.neighbor != nil}
        }
    }
    
    var connectedOutlets: [Connector] {
        get {
            return outlets.filter{$0.neighbor != nil}
        }
    }
    
    var isConnected: Bool {
        get {
            for link in outlets {
                if link.neighbor == nil {return false}
            }
            for link in inlets {
                if link.neighbor == nil {return false}
            }
            return true
        }
    }
    
    func addNeighbor(color: ConnectorColor, neighbor: Connectable, linkType: LinkType){
        switch linkType {
        case .inlet:
             for (index, connector) in inlets.enumerated(){
                if connector.color == color && connector.neighbor == nil {
                    inlets[index].neighbor = neighbor
                }
            }
        case .outlet:
            for (index, connector) in outlets.enumerated(){
                if connector.color == color && connector.neighbor == nil {
                    outlets[index].neighbor = neighbor
                }
            }
        }

    }
    
    func removeNeighbor(neighbor: Connectable, linkType: LinkType){
        switch linkType {
        case .inlet:
            for (index, connector) in inlets.enumerated(){
                if connector.neighbor === neighbor {
                    inlets[index].neighbor = nil
                }
            }
        case .outlet:
            for (index, connector) in outlets.enumerated(){
                if connector.neighbor === neighbor {
                    outlets[index].neighbor = nil
                }
            }
        }
        
    }
    
    
    
}

enum LinkType {
    case inlet, outlet
}

enum ConnectorColor {
    case green, blue, red, orange, magenta
}
