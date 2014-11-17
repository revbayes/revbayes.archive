#import <Cocoa/Cocoa.h>



@interface ViewControllerRandomVariable : NSViewController {
    
    IBOutlet NSPopUpButton*        partitionSelector;
    IBOutlet NSSegmentedControl*   nodeBranchSelector;
	IBOutlet NSTextField*          distributionLabel;
	IBOutlet NSPopUpButton*        distributionSelector;
	IBOutlet NSTextField*          parameter1Label;
	IBOutlet NSPopUpButton*        parameter1Selector;
	IBOutlet NSTextField*          parameter2Label;
	IBOutlet NSPopUpButton*        parameter2Selector;
	IBOutlet NSTextField*          parameter3Label;
	IBOutlet NSPopUpButton*        parameter3Selector;
	IBOutlet NSTextField*          parameter4Label;
	IBOutlet NSPopUpButton*        parameter4Selector;
    IBOutlet NSButton*             clampedValueButton;
    IBOutlet NSPopUpButton*        clampedValueSelector;
	IBOutlet NSTextField*          nameField;
	IBOutlet NSTextField*          nameLabel;
    IBOutlet NSButton*             monitorVariableButton;
    IBOutlet NSStepper*            vectorSizeStepper;
    IBOutlet NSTextField*          vectorSizeField;
    IBOutlet NSTextField*          vectorSizeLabel;
}

@property (nonatomic,strong) NSPopUpButton*      partitionSelector;
@property (nonatomic,strong) NSSegmentedControl* nodeBranchSelector;
@property (nonatomic,strong) NSTextField*        distributionLabel;
@property (nonatomic,strong) NSPopUpButton*      distributionSelector;
@property (nonatomic,strong) NSTextField*        parameter1Label;
@property (nonatomic,strong) NSPopUpButton*      parameter1Selector;
@property (nonatomic,strong) NSTextField*        parameter2Label;
@property (nonatomic,strong) NSPopUpButton*      parameter2Selector;
@property (nonatomic,strong) NSTextField*        parameter3Label;
@property (nonatomic,strong) NSPopUpButton*      parameter3Selector;
@property (nonatomic,strong) NSTextField*        parameter4Label;
@property (nonatomic,strong) NSPopUpButton*      parameter4Selector;
@property (nonatomic,strong) NSButton*           clampedValueButton;
@property (nonatomic,strong) NSPopUpButton*      clampedValueSelector;
@property (nonatomic,strong) NSTextField*        nameField;
@property (nonatomic,strong) NSTextField*        nameLabel;
@property (nonatomic,strong) NSButton*           monitorVariableButton;
@property (nonatomic,strong) NSStepper*          vectorSizeStepper;
@property (nonatomic,strong) NSTextField*        vectorSizeField;
@property (nonatomic,strong) NSTextField*        vectorSizeLabel;

@end
