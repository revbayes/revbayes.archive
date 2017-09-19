#import <Cocoa/Cocoa.h>
@class ModelView;
@class ToolModel;
@class WindowControllerModelBrowser;
@class WindowControllerModelSubmission;



@interface WindowControllerModel : NSWindowController {

	IBOutlet NSButton*                 okButton;
    IBOutlet NSButton*                 exportButton;
	IBOutlet NSButton*                 importButton;
    IBOutlet NSButton*                 possibleInletsButton0;
    IBOutlet NSButton*                 possibleInletsButton1;
    IBOutlet NSButton*                 possibleInletsButton2;
    IBOutlet NSButton*                 possibleInletsButton3;
    BOOL                               selectedInlets[4];
    ToolModel*                         myTool;
	IBOutlet ModelView*                modelViewPtr;
	IBOutlet NSButton*                 showParmNamesButton;      // check box determining whether the parameter names are displayed
    IBOutlet NSButton*                 toggleParameterPaletteButton;
	BOOL                               showParmNames;
	IBOutlet NSPopUpButton*            connectedSourcesSelector;
    IBOutlet NSView*                   palleteView;
}

@property (nonatomic) BOOL             showParmNames;

- (IBAction)changeInlets:(id)sender;
- (void)drawerClosed;
- (void)drawerOpened;
- (IBAction)exportButtonAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolModel*)t andParms:(NSMutableArray*)p;
- (IBAction)importButtonAction:(id)sender;
- (IBAction)okButtonAction:(id)sender;
- (void)setParmsPtr:(NSMutableArray*)p;
- (void)setSourceInformation;
- (IBAction)toggleShowParmNames:(id)sender;
- (void)updateInletSelector;
- (NSMutableArray*)variables;

@end
