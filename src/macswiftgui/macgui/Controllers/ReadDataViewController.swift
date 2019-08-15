//
//  ReadDataViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 7/6/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class ReadDataViewController: NSViewController {
    
    @IBAction func cancelButtonPushed(_ sender: Any) {
        NotificationCenter.default.post(name: .dismissToolSheet, object: self)
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
    }
    
}
