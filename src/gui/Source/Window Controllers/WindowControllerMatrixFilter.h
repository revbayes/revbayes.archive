#import <Cocoa/Cocoa.h>
@class RbData;
@class ToolMatrixFilter;



@interface WindowControllerMatrixFilter : NSWindowController {

    IBOutlet NSTabView*           excludeTypeButton;
	IBOutlet NSSegmentedControl*  numberOfOutletsButton1;
	IBOutlet NSSegmentedControl*  numberOfOutletsButton2;
    
    IBOutlet NSPopUpButton*       taxaMatrixSelector;
    IBOutlet NSScrollView*        includedTaxaView;
    IBOutlet NSScrollView*        excludedTaxaView;
    IBOutlet NSPopUpButton*       includedTaxaSetsButton;
    IBOutlet NSPopUpButton*       excludedTaxaSetsButton;
    IBOutlet NSButton*            includeTaxaButton;
    IBOutlet NSButton*            excludeTaxaButton;
    
    IBOutlet NSPopUpButton*       charMatrixSelector;
    IBOutlet NSScrollView*        includedCharView;
    IBOutlet NSScrollView*        excludedCharView;
    IBOutlet NSPopUpButton*       includedCharSetsButton;
    IBOutlet NSPopUpButton*       excludedCharSetsButton;
    IBOutlet NSButton*            includeCharButton;
    IBOutlet NSButton*            excludeCharButton;

	NSMutableArray*               includedCharMatrices;
	NSMutableArray*               excludedCharMatrices;
	NSMutableArray*               includedTaxaMatrices;
	NSMutableArray*               excludedTaxaMatrices;
	int                           numberOfOutlets;
    
	IBOutlet NSButton*            okButton;
    ToolMatrixFilter*             myTool;
}

@property (nonatomic) int         numberOfOutlets;

- (IBAction)cancelButtonAction:(id)sender;
- (IBAction)changeCharMatrix:(id)sender;
- (IBAction)changeTaxaMatrix:(id)sender;
- (IBAction)excludeCharButtonAction:(id)sender;
- (IBAction)excludeCharSetsButtonAction:(id)sender;
- (IBAction)excludeTaxaButtonAction:(id)sender;
- (IBAction)excludeTaxaSetsButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (IBAction)includeCharButtonAction:(id)sender;
- (IBAction)includeCharSetsButtonAction:(id)sender;
- (IBAction)includeTaxaButtonAction:(id)sender;
- (IBAction)includeTaxaSetsButtonAction:(id)sender;
- (id)initWithTool:(ToolMatrixFilter*)t;
- (float)lengthOfLongestName:(RbData*)matrix;
- (IBAction)okButtonAction:(id)sender;
- (void)setControlWindowSize;
- (void)setMatrixList;
- (BOOL)setCharMatrixInfo:(RbData*)matrix;
- (BOOL)setTaxaMatrixInfo:(RbData*)matrix;
- (void)updateButtons;

@end
