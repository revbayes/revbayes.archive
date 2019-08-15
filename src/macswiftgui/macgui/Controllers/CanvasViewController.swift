//
//  DestinationViewController.swift
//  macgui
//
//  Created by Svetlana Krasikova on 2/22/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class CanvasViewController: NSViewController, NSWindowDelegate {

    weak var analysis: Analysis? {
        didSet{
            if let analysis = analysis {
                reset(analysis: analysis)
            }
        }
    }
    
 
    @IBOutlet weak var scrollView: NSScrollView!
    @IBOutlet weak var canvasView: CanvasView!
    @IBOutlet weak var transparentToolsView: TransparentToolsView!
    @IBOutlet weak var invitationLabel: NSTextField!
    @IBOutlet weak var canvasViewHeightConstraint: NSLayoutConstraint!
    @IBOutlet weak var canvasViewWidthConstraint: NSLayoutConstraint!
    

    
    enum Appearance {
        static let maxStickerDimension: CGFloat = 50.0
    }
    
    override func viewDidLoad() {
        super.viewDidLoad()
        canvasView.delegate = self
//        add CanvasViewController to the window delegate, so that it can be part of the responder chain
        if let window = NSApp.windows.first{
            window.delegate = self
        }
        scrollView.magnification = 1.5
        
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didChangeMagnification(_ :)),
                                               name: NSScrollView.didEndLiveMagnifyNotification,
                                               object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(changeCanvasObjectControllersSelection(notification:)),
                                               name: .didSelectCanvasObjectController,
                                               object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(deleteSelectedCanvasObjects(notification:)),
                                               name: .didSelectDeleteKey,
                                               object: nil)
        NotificationCenter.default.addObserver(self,
                                               selector: #selector(didConnectTools(notification:)),
                                               name: .didConnectTools,
                                               object: nil)
        

       
    }
    
    
    override func viewDidLayout() {
        super.viewDidLayout()
        canvasViewHeightConstraint.constant = scrollView.frame.size.height * 4
        canvasViewWidthConstraint.constant = scrollView.frame.size.width * 4
        
    }
    
    
    @IBAction func magnify(_ sender: NSPopUpButton) {
        switch sender.indexOfSelectedItem {
        case 1:
            scrollView.magnification = 0.375
            sender.setTitle("25%")
        case 2:
            scrollView.magnification = 0.75
            sender.setTitle("50%")
        case 3:
            scrollView.magnification = 1.125
            sender.setTitle("75%")
        case 4:
            scrollView.magnification = 1.5
            sender.setTitle("100%")
        case 5:
            scrollView.magnification = 1.875
            sender.setTitle("125%")
        case 6:
            scrollView.magnification = 2.25
            sender.setTitle("1500%")
        case 7:
            scrollView.magnification = 3.0
            sender.setTitle("200%")
        case 8:
            scrollView.magnification = 4.5
            sender.setTitle("300%")
        case 9:
            scrollView.magnification = 6.0
            sender.setTitle("400%")
        default:
            print("Switch case error!")
        }
    }
   
    @objc func didConnectTools(notification: Notification){
        let userInfo = notification.userInfo! as! [String: ConnectorItemView]
        if let color = userInfo["target"]?.arrowColor, let targetTool = userInfo["target"]?.delegate?.getTool(), let sourceTool = userInfo["source"]?.delegate?.getTool(){
            let toConnector = userInfo["target"]?.delegate?.getConnector() as! Connector
            toConnector.setNeighbor(neighbor: sourceTool as! Connectable)
            let fromConnector = userInfo["source"]?.delegate?.getConnector() as! Connector
            fromConnector.setNeighbor(neighbor: targetTool as! Connectable)
            let connection = Connection(to: toConnector, from: fromConnector)
            let arrowController = ArrowViewController(frame: canvasView.bounds, color: color, sourceTool: sourceTool as! Connectable, targetTool: targetTool as! Connectable, connection: connection)
            addChild(arrowController)
            analysis?.arrows.append(connection)
            canvasView.addSubview(arrowController.view, positioned: .below, relativeTo: transparentToolsView)
            reset(analysis: analysis!)
        }
    }
    
    
    @objc func didChangeMagnification(_ notification: Notification){
        NotificationCenter.default.post(name: .didChangeMagnification,
                                        object: self,
                                        userInfo: ["magnification": Float(scrollView.magnification)])
    }
    
//    deselect all tool controllers except the one that sent the notification
    @objc func changeCanvasObjectControllersSelection(notification: Notification){
        for childController in children {
            if childController .isKind(of: CanvasObjectViewController.self) &&
                childController !== notification.object as! CanvasObjectViewController {
                    (childController as! CanvasObjectViewController).viewSelected = false
                }
            }
        }
    
    @objc func deleteSelectedCanvasObjects(notification: NSNotification){
        var numConnectionsToDelete = 0
        for childController in children {
            if childController .isKind(of: ArrowViewController.self) &&
                (childController as! CanvasObjectViewController).viewSelected == true {
                numConnectionsToDelete = numConnectionsToDelete + 1
            }
        }
        if numConnectionsToDelete > 0 {
            let alert = NSAlert()
            if numConnectionsToDelete > 1 {
                alert.messageText = "Warning: Removing connections between tools"
                alert.informativeText = "Removing connections can lead to loss of information in downstream tools"
            } else {
                alert.messageText = "Warning: Removing a connection between tools"
                alert.informativeText = "Removing a connection can lead to loss of information in downstream tools"
            }
            
            alert.addButton(withTitle: "Delete")
            alert.addButton(withTitle: "Cancel")
            
            let result = alert.runModal()
            switch result {
                case NSApplication.ModalResponse.alertFirstButtonReturn:
                    for childController in children {
                        if childController .isKind(of: CanvasToolViewController.self) &&
                            (childController as! CanvasObjectViewController).viewSelected == true {
                            removeCanvasObjectView(canvasObjectViewController: childController as! CanvasObjectViewController)
                        }
                    }
                    for childController in children {
                        if childController .isKind(of: ArrowViewController.self) &&
                            (childController as! CanvasObjectViewController).viewSelected == true {
                            removeCanvasObjectView(canvasObjectViewController: childController as! CanvasObjectViewController)
                        }
                }
                default: break
            }
        }
        else {
            for childController in children {
                if childController .isKind(of: CanvasToolViewController.self) &&
                    (childController as! CanvasObjectViewController).viewSelected == true {
                    removeCanvasObjectView(canvasObjectViewController: childController as! CanvasObjectViewController)
                }
            }
        }
            reset(analysis: analysis!)
    }

    
    func reset(analysis: Analysis){
        for subview in canvasView.subviews{
            if subview.isKind(of: CanvasObjectView.self) {
                subview.removeFromSuperview()
            }
        }
        for child in children{
            if child.isKind(of: CanvasObjectViewController.self) {
                child.removeFromParent()
            }
        }
        if analysis.isEmpty(){
            invitationLabel.isHidden = false
        } else {
            invitationLabel.isHidden = true
            for tool in analysis.tools {
                addToolView(tool: tool)
            }
            for connection in analysis.arrows {
                addArrowView(connection: connection)
            }
        }
    }
    
    func addArrowView(connection: Connection){
        let color = connection.from.getColor()
        if let sourceTool = connection.to.neighbor, let targetTool = connection.from.neighbor {
            let arrowController = ArrowViewController(frame: canvasView.bounds, color: color, sourceTool: sourceTool, targetTool: targetTool, connection: connection)
            addChild(arrowController)
            canvasView.addSubview(arrowController.view, positioned: .below, relativeTo: transparentToolsView)
        }
    }
    
    func addToolView(tool: ToolObject){
        guard let canvasToolViewController = NSStoryboard.load(.canvasTool) as? CanvasToolViewController else {return}
        canvasToolViewController.tool = tool
        addChild(canvasToolViewController)
        canvasView.addSubview(canvasToolViewController.view)
    }
    
    func addCanvasTool(image: NSImage, frame: NSRect, name: String){
        if let analysis = analysis {
            let newTool = initToolObjectWithName(name, image: image, frame: frame)
            analysis.tools.append(newTool)
            addToolView(tool: newTool)
        }
    }
    
    
    func removeToolFromAnalysisOld(toolViewController: CanvasToolViewController){
        if let analysis = analysis, let index = analysis.tools.firstIndex(of: toolViewController.tool!) {
            let arrowViewControllers = findArrowControllersByTool(tool: toolViewController.tool!)
            for arrowViewController in arrowViewControllers {
                removeCanvasObjectView(canvasObjectViewController: arrowViewController)
            }
            analysis.tools.remove(at: index)
        }
    }
    
    func removeToolFromAnalysis(toolViewController: CanvasToolViewController){
        if let analysis = analysis, let index = analysis.tools.firstIndex(of: toolViewController.tool!) {
            let arrowViewControllers = findArrowControllersByTool(tool: toolViewController.tool!)
            for arrowViewController in arrowViewControllers {
                removeCanvasObjectView(canvasObjectViewController: arrowViewController)
            }
            analysis.tools.remove(at: index)
        }
    }
    
    func findArrowControllersByTool(tool: ToolObject) -> [ArrowViewController] {
        var arrowControllers: [ArrowViewController] = []
        for child in children {
            if child.isKind(of: ArrowViewController.self){
                if (child as! ArrowViewController).ownedBy(tool: tool) {
                    arrowControllers.append(child as! ArrowViewController)
                }
            }
        }
        return arrowControllers
    }
    
    func removeConnectionFromAnalysis(arrowViewController: ArrowViewController){
        if let analysis = analysis, let index = analysis.arrows.firstIndex(of: arrowViewController.connection!) {
            arrowViewController.willDeleteView()
            analysis.arrows.remove(at: index)
        }
    }
    
    func removeCanvasObjectView(canvasObjectViewController: CanvasObjectViewController) {
        if canvasObjectViewController.isKind(of: ArrowViewController.self){
            removeConnectionFromAnalysis(arrowViewController: canvasObjectViewController as! ArrowViewController)
        } else {
            if canvasObjectViewController.isKind(of: CanvasToolViewController.self){
                removeToolFromAnalysis(toolViewController: canvasObjectViewController as! CanvasToolViewController)
            }
        }
        canvasObjectViewController.view.removeFromSuperview()
        canvasObjectViewController.removeFromParent()
    }
    
}

// MARK: - Methods for handling drag and drop from tool view to canvas
extension CanvasViewController: CanvasViewDelegate {
    
    func selectContentView(width: CGFloat) {
        NSColor.lightGray.set()
        let path = NSBezierPath(rect: scrollView.documentVisibleRect)
        path.lineWidth = width
        path.stroke()
    }
    
    func processImageURLs(_ urls: [URL], center: NSPoint) {
        for (_,url) in urls.enumerated() {
            if let image = NSImage(contentsOf: url){
                let name = url.lastPathComponent
                processImage(image, center: center, name: name)
            }
        }
    }
    
    func processImage(_ image: NSImage, center: NSPoint, name: String) {
        invitationLabel.isHidden = true
        let constrainedSize = image.aspectFitSizeForMaxDimension(Appearance.maxStickerDimension)
        let frame = NSRect(x: center.x - constrainedSize.width/2, y: center.y - constrainedSize.height/2, width: constrainedSize.width, height: constrainedSize.height)
        addCanvasTool(image: image, frame: frame, name: name)
    }
    
    func isMouseDownOnArrowView(event: NSEvent, point: NSPoint) -> Bool {
        for childController in children {
            if childController.isKind(of: ArrowViewController.self){
                let arrowViewController = childController as! ArrowViewController
                let arrowView = ((arrowViewController.view) as! ArrowView)
                if arrowView.clickAreaContains(point: point) {
                    arrowView.mouseDown(with: event)
                    return true
                }
            }
        }
        return false
    }
    
    func mouseDownOnCanvasView() {
        for childController in children {
            if childController.isKind(of: CanvasObjectViewController.self){
                if (childController as! CanvasObjectViewController).viewSelected {
                    (childController as! CanvasObjectViewController).viewSelected = false
                }
            }
        }
    }
    
}





