//
//  NavigatorViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/14/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

protocol NavigatorViewControllerDelegate: class {
    func navigatorViewController(viewController: NavigatorViewController,
                                  selectedAnalysis: Analysis?) -> Void
    }
    

class NavigatorViewController: NSViewController {
    

    @IBOutlet weak var actionButton: NSPopUpButton!
    @IBOutlet weak var arrayController : NSArrayController!
    @IBOutlet weak var analysesTableView: NSTableView!
    
    @objc dynamic var analyses: [Analysis] = [Analysis(name: "untitled analysis")]
    
    weak var delegate: NavigatorViewControllerDelegate?
    
    @IBAction func actionButtonClicked(_ sender: NSPopUpButton) {
        switch sender.selectedItem?.tag {
        case 1:
            addAnalysis()
        case 2:
            if let selectedForRemoval = arrayController.selectedObjects {
                arrayController.removeSelectedAnalyses(toRemove: selectedForRemoval as! [Analysis])
                if analyses.isEmpty {
                    addAnalysis()
                }
            }
        case 3:
            if let selectedAnalysis = arrayController.selectedObjects.first as! Analysis? {
                copyAnalysis(from: selectedAnalysis)
            }
        default:
            print("Switch case error")
            
        }
        setSelectedAnalysisToActive()
        
    }
   
   
    
    /**
     Trigger a call on the delegate to update the detail view if a new analysis is selected in the table view
     */
    @IBAction func selectAnalysis(sender: AnyObject){
        let selectedAnalysis = arrayController.selectedObjects.first as! Analysis?
        delegate?.navigatorViewController(viewController: self, selectedAnalysis: selectedAnalysis)
    }
    
    /**
     Trigger a call on the delegate to update the detail view if an analysis is added or deleted
     */
    
    func setSelectedAnalysisToActive(){
        let index: IndexSet = [arrayController.selectionIndex]
        analysesTableView.selectRowIndexes(index, byExtendingSelection: false)
        let selectedAnalysis = arrayController.selectedObjects.first as! Analysis?
        delegate?.navigatorViewController(viewController: self, selectedAnalysis: selectedAnalysis)
    }
    
    /**
     Add a new empty analysis
     */
    func addAnalysis() {
        let analysis = Analysis(name: getUniqueAnalysisName(prefix: "untitled analysis", isCopy: false))
        arrayController.addObject(analysis)
    }
    
    /**
     Copy an analysis
     */
    func copyAnalysis(from: Analysis) {
        let analysis = from.copy() as! Analysis
        let prefix = from.name
        analysis.name = getUniqueAnalysisName(prefix: prefix, isCopy: true)
        arrayController.addObject(analysis)
    }
    
 
    override func viewDidLoad() {
        super.viewDidLoad()
        let indices = IndexSet([0])
        analysesTableView.selectRowIndexes(indices, byExtendingSelection: false)
    }
    
    override func viewDidAppear() {
        super.viewDidAppear()
        setSelectedAnalysisToActive()

    }
}


