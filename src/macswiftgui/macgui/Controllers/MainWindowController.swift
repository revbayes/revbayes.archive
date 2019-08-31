//
//  MainWindowController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 4/5/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class MainWindowController: NSWindowController{

    @IBOutlet weak var zoom: NSPopUpButton!

    
    @objc func changeZoomTitle(notification: Notification){
        let userInfo = notification.userInfo! as! [String : Float]
        let magnification = userInfo["magnification"]!/0.015
        let title = String(format:"%.0f", magnification)
        zoom.setTitle("\(title)%")
    }
    
    
    override func windowDidLoad() {
        super.windowDidLoad()
        
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(changeZoomTitle(notification:)),
                                               name: .didChangeMagnification,
                                               object: nil)
    }
    
}
