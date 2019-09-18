//
//  SheetViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 7/1/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class SheetViewController: NSViewController {
    
    enum Appearance {
        static let modalSheetWidth: CGFloat = 330.0
        static let modalSheetHight: CGFloat = 330.0
    }
    weak var tool: ToolObject?
    
    
    lazy var tabViewController: NSTabViewController = {
        return NSStoryboard.load(StoryBoardName.modalSheetTabView)
        }() as! NSTabViewController
    

    override func updateViewConstraints() {
        super.updateViewConstraints()
    }

    override func viewWillLayout() {
        self.preferredContentSize = NSSize(width: Appearance.modalSheetWidth, height: Appearance.modalSheetHight)
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        self.title = tool?.name
        addContentController()
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didPushCancel(notification:)),
                                               name: .dismissToolSheet,
                                               object: nil)
    }
    
    @objc func didPushCancel(notification: NSNotification) {
        presentingViewController?.dismiss(self)
    }
    
    func addContentController(){
        let tabIndex = findTabIndex()
        tabViewController.selectedTabViewItemIndex = tabIndex
    }

    func findTabIndex() -> Int {
        for (index, tabItem) in tabViewController.tabViewItems.enumerated() {
            let name = getToolName()
            if tabItem.identifier as! String == name {
                return index
            }
        }
        return 0
    }
    
    func getToolName() -> String {
        switch tool {
        case _ as ReadData:
            return StoryBoardName.readData.rawValue
        default:
            return StoryBoardName.readData.rawValue
        }

    }
    
}
