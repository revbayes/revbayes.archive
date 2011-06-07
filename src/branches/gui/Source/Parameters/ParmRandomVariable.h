#import <Cocoa/Cocoa.h>
#import "Parm.h"
@class Distribution;
@class ViewControllerRandomVariable;

#define BRANCH   0
#define NODE     1



@interface ParmRandomVariable : Parm <NSCoding> {

    NSPopUpButton*                            partitionSelector;
    NSSegmentedControl*                       nodeBranchSelector;
	NSTextField*                              distributionLabel;
	NSPopUpButton*                            distributionSelector;
	NSTextField*                              parameter1Label;
	NSPopUpButton*                            parameter1Selector;
	NSTextField*                              parameter2Label;
	NSPopUpButton*                            parameter2Selector;
	NSTextField*                              parameter3Label;
	NSPopUpButton*                            parameter3Selector;
	NSTextField*                              parameter4Label;
	NSPopUpButton*                            parameter4Selector;
    NSButton*                                 clampedValueButton;
    NSPopUpButton*                            clampedValueSelector;
    NSButton*                                 monitorVariableButton;
    NSStepper*                                vectorSizeStepper;
    NSTextField*                              vectorSizeField;
    NSTextField*                              vectorSizeLabel;
    
    IBOutlet NSObjectController*              objectController;
    IBOutlet NSBox*                           box;
    IBOutlet ViewControllerRandomVariable*    treePlateView;
    IBOutlet ViewControllerRandomVariable*    parameterView;
    IBOutlet ViewControllerRandomVariable*    treePlateVectorView;
    IBOutlet ViewControllerRandomVariable*    parameterVectorView;
    
    BOOL                                      isClamped;
    BOOL                                      appliesToNode;
    BOOL                                      monitorValue;
    NSMutableArray*                           probabilityModel;
    int                                       indexOfSelectedPartitionModel;
}

@property (readwrite) BOOL appliesToNode;
@property (readwrite) BOOL isClamped;
@property (readwrite) BOOL monitorValue;

- (IBAction)changeDistributionAction:(id)sender;
- (IBAction)changeIsClampedAction:(id)sender;
- (IBAction)changeClampledValueSelectionAction:(id)sender;
- (IBAction)changeDimensionsAction:(id)sender;
- (IBAction)changeMonitorValueAction:(id)sender;
- (IBAction)changeNodeOrBranchSelection:(id)sender;
- (IBAction)changePartitionSelectionAction:(id)sender;
- (IBAction)changeParameter1:(id)sender;
- (IBAction)changeParameter2:(id)sender;
- (IBAction)changeParameter3:(id)sender;
- (IBAction)changeParameter4:(id)sender;
- (void)chooseView;
- (NSArray*)getPotentialClampValues;
- (BOOL)hasPartitionModels;
- (void)initializeImage;
- (void)restoreWindowState;
- (void)saveWindowState;
- (void)showInformationForParameterIndexed:(int)idx withDistribution:(Distribution*)cd;

@end
