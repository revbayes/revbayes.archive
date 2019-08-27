//
//  Connection.swift
//  macgui
//
//  Created by Svetlana Krasikova on 6/5/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Foundation
import Cocoa


class Connection: NSObject {
    var to: Connector
    var from: Connector
    
    init(to: Connector, from: Connector){
        self.to = to
        self.from = from
    }
    
    
}
