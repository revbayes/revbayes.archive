//
//  InfoButton.swift
//  macgui
//
//  Created by Svetlana Krasikova on 6/27/19.
//  Copyright © 2019 Svetlana Krasikova. All rights reserved.
//

import Cocoa

class InfoButton: NSButton {

    enum ButtonState {
        case idle
        case pressed
        case highlighted
    }
    
    private let shapeLayer = CAShapeLayer()
    
    var buttonState: ButtonState = .idle { didSet { needsLayout = true } }
    
    var mouseIsInside = false
    
    weak var delegate: InfoButtonDelegate?
    

    override func awakeFromNib() {
        let trackingArea = NSTrackingArea(rect: self.bounds, options:NSTrackingArea.Options.init(rawValue: 129), owner: self, userInfo: nil)
        self.addTrackingArea(trackingArea)
    }
    
    override func mouseEntered(with event: NSEvent) {
        mouseIsInside = true
        buttonState = .highlighted
    }
    
    override func mouseExited(with event: NSEvent) {
        mouseIsInside = false
        buttonState = .idle
    }

    override func mouseDown(with event: NSEvent) {
        buttonState = .pressed
    }
    
    
    override func mouseUp(with event: NSEvent) {
        if mouseIsInside {
            delegate?.infoButtonClicked()
        } else {
            buttonState = .idle
        }
    }
    
    override init(frame: NSRect) {
        super.init(frame: frame)
        commonInit()
    }
    
    required init?(coder decoder: NSCoder) {
        super.init(coder: decoder)
        commonInit()
    }
    
    private func commonInit() {
        wantsLayer = true
    }
    
    override func makeBackingLayer() -> CALayer {
        return shapeLayer
    }
    
    
    override func layout() {
        super.layout()
        setAppearanceForState()
        shapeLayer.path = CGPath(ellipseIn: bounds, transform: nil)
        if  let backingScaleFactor = self.window?.backingScaleFactor {
            shapeLayer.contentsScale = backingScaleFactor
            addLabel(scaleFactor: backingScaleFactor)
        }
    }
    
    private func addLabel(scaleFactor: CGFloat) {
        let textLayer = CATextLayer()
        textLayer.frame = bounds
        textLayer.contentsScale = scaleFactor
        textLayer.font = "Hoefler Text" as CFTypeRef
        textLayer.foregroundColor = NSColor.white.cgColor
        textLayer.fontSize = 7
        textLayer.string = "i"
        textLayer.allowsFontSubpixelQuantization = true
        textLayer.position = bounds.center()
        textLayer.alignmentMode = .center;
        shapeLayer.addSublayer(textLayer)
    }
    
    private func setAppearanceForState() {
        switch buttonState {
        case .pressed:
            shapeLayer.fillColor = NSColor.darkGray.cgColor
        case .highlighted:
            shapeLayer.fillColor = NSColor.darkGray.cgColor
        case .idle:
            shapeLayer.fillColor = NSColor.clear.cgColor
        }
    }
}

protocol InfoButtonDelegate: class {
    func infoButtonClicked()
}


