//
//  MainSplitViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/21/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class MainSplitViewController: NSSplitViewController, NavigatorViewControllerDelegate {

    var masterViewController: NavigatorViewController {
        let masterItem = splitViewItems[0]
        return masterItem.viewController as! NavigatorViewController
    }
    
    var detailViewController: AnalysisViewController {
        let detailItem = splitViewItems[1]
        return detailItem.viewController as! AnalysisViewController
    }
    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        masterViewController.delegate = self
    }
    
//    MARK: NavigatorViewControllerDelegate
    func navigatorViewController(viewController: NavigatorViewController, selectedAnalysis: Analysis?) {
        if let analysis = selectedAnalysis {
            detailViewController.addAnalysisToCanvas(analysis: analysis)
        } else {
            detailViewController.setCanvasToDefault()
        }
    }
    
}
