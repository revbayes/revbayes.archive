#import <Foundation/Foundation.h>



@interface ViewControllerRandomVariable : NSViewController {
    
    IBOutlet NSPopUpButton*       partitionSelector;
    IBOutlet NSSegmentedControl*  nodeBranchSelector;
	IBOutlet NSTextField*         distributionLabel;
	IBOutlet NSPopUpButton*       distributionSelector;
	IBOutlet NSTextField*         parameter1Label;
	IBOutlet NSPopUpButton*       parameter1Selector;
	IBOutlet NSTextField*         parameter2Label;
	IBOutlet NSPopUpButton*       parameter2Selector;
	IBOutlet NSTextField*         parameter3Label;
	IBOutlet NSPopUpButton*       parameter3Selector;
	IBOutlet NSTextField*         parameter4Label;
	IBOutlet NSPopUpButton*       parameter4Selector;
    IBOutlet NSButton*            clampedValueButton;
    IBOutlet NSPopUpButton*       clampedValueSelector;
	IBOutlet NSTextField*         nameField;
	IBOutlet NSTextField*         nameLabel;
    IBOutlet NSButton*            monitorVariableButton;
    IBOutlet NSStepper*           vectorSizeStepper;
    IBOutlet NSTextField*         vectorSizeField;
    IBOutlet NSTextField*         vectorSizeLabel;
}

@property (assign) NSPopUpButton*      partitionSelector;
@property (assign) NSSegmentedControl* nodeBranchSelector;
@property (assign) NSTextField*        distributionLabel;
@property (assign) NSPopUpButton*      distributionSelector;
@property (assign) NSTextField*        parameter1Label;
@property (assign) NSPopUpButton*      parameter1Selector;
@property (assign) NSTextField*        parameter2Label;
@property (assign) NSPopUpButton*      parameter2Selector;
@property (assign) NSTextField*        parameter3Label;
@property (assign) NSPopUpButton*      parameter3Selector;
@property (assign) NSTextField*        parameter4Label;
@property (assign) NSPopUpButton*      parameter4Selector;
@property (assign) NSButton*           clampedValueButton;
@property (assign) NSPopUpButton*      clampedValueSelector;
@property (assign) NSTextField*        nameField;
@property (assign) NSTextField*        nameLabel;
@property (assign) NSButton*           monitorVariableButton;
@property (assign) NSStepper*          vectorSizeStepper;
@property (assign) NSTextField*        vectorSizeField;
@property (assign) NSTextField*        vectorSizeLabel;

@end
