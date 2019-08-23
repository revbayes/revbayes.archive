//
//  ReadData.swift
//  macgui
//
//  Created by Svetlana Krasikova on 5/6/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ReadData: Connectable {
   
    override init(image: NSImage, frameOnCanvas: NSRect) {
        super.init(image: image, frameOnCanvas: frameOnCanvas)
        self.name = "Read Data Tool"
        let green = Connector(color:ConnectorColor.green)
        let blue = Connector(color:ConnectorColor.blue)
        self.inlets = []
        self.outlets = [green, blue]
    }
}
