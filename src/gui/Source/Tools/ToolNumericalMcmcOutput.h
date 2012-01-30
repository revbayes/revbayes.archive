//
//  ToolNumericalMcmcOutput.h
//  RevBayesGui
//
//  Created by Sebastian Hoehna on 3/22/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "Trace.h"
#import "Tool.h"

#import <vector>

@class WindowControllerNumberMatrix;


@interface ToolNumericalMcmcOutput : Tool <NSCoding> {
    
    IBOutlet NSTextField*                   mcmcTextField;
    
	IBOutlet NSButton*                      okButton;
	IBOutlet NSTabView*                     sourceTypeTab;
    
	IBOutlet NSPopUpButton*                 importFileTypeButton;
	IBOutlet NSPopUpButton*                 exportFileTypeButton;
    
    NSString*                               source;
	int                                     fileType;
    
    WindowControllerNumberMatrix*           dataInspector;	
    
    std::vector<RbPtr<Trace> >*             data;
    
}

@property (readwrite,assign) std::vector<RbPtr<Trace> >* data;
@property (readwrite,assign) NSString* source;

- (IBAction)cancelAction:(id)sender;
- (void)closeControlPanel;
- (void)execute;
- (void)tabView:(NSTabView*)tabView didSelectTabViewItem:(NSTabViewItem*)tabViewItem;
- (void)encodeWithCoder:(NSCoder *)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (IBAction)okButtonAction:(id)sender;
- (BOOL)readDataFile;
- (NSMutableAttributedString*)sendTip;
- (void)setControlsEnabledState;
- (void)setControlWindowSize;
- (void)setInletsAndOutlets;
- (void)showControlPanel;
- (void)showInspectorPanel;
- (BOOL)writeDataFile;

@end