//
//  Bootstrap.swift
//  macgui
//
//  Created by Svetlana Krasikova on 4/15/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class Bootstrap: Connectable {
    
    override init(image: NSImage, frameOnCanvas: NSRect) {
        super.init(image: image, frameOnCanvas: frameOnCanvas)
        self.name = "Bootstrap Tool"
        let green1 = Connector(color:ConnectorColor.green)
        let green2 = Connector(color:ConnectorColor.green)
        self.inlets = [green1]
        self.outlets = [green2]
    }
    
}
