//
//  ImageFile.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/8/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa


struct ImageFile {
    var url: URL
    var image: NSImage {
        return NSImage(byReferencing: url)
    }
    var name: String {
        return url.lastPathComponent
    }
    
    init(url: URL){
        self.url = url
    }
}
