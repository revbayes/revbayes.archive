#import <Cocoa/Cocoa.h>
#include <vector>
@class ToolReadData;
class CharacterMatrix;



@interface WindowControllerReadData : NSWindowController <NSTabViewDelegate> {

    ToolReadData*                      myTool;
	NSString*                          fileName;
	NSString*                          pathName;
    
	IBOutlet NSTabView*                matrixTypeTab;
	IBOutlet NSButton*                 okButton;
	IBOutlet NSSegmentedControl*       numberOfOutletsButton1;
	IBOutlet NSPopUpButton*            dataTypeButton1;
	IBOutlet NSPopUpButton*            dataFormatButton;
	IBOutlet NSPopUpButton*            dataAlignmentButton;
	IBOutlet NSSegmentedControl*       interleavedFormatButton;
	IBOutlet NSTextField*              dataTypeField;
	IBOutlet NSTextField*              dataAlignmentField;
	IBOutlet NSTextField*              interleavedFormatField;
	
	IBOutlet NSSegmentedControl*       numberOfOutletsButton2;
	IBOutlet NSPopUpButton*            dataTypeButton2;
	IBOutlet NSTextField*              numberOfTaxaField;
	IBOutlet NSTextField*              numberOfCharactersField;
		
    int                                dataAlignment;
    int                                dataFormat;
	int                                dataType;
	int                                dataTypeSimulated;
	int                                dataInterleaved;
	int                                numberOfTaxa;
	int                                numberOfCharacters;
    int                                numberOfStates;
	int                                numberOfOutlets;
}

@property (readwrite) int dataAlignment;
@property (readwrite) int dataFormat;
@property (readwrite) int dataInterleaved;
@property (readwrite) int dataType;
@property (readwrite) int dataTypeSimulated;
@property (readwrite) int numberOfCharacters;
@property (readwrite) int numberOfTaxa;
@property (readwrite) int numberOfStates;
@property (readwrite) int numberOfOutlets;
@property (retain) NSString* fileName;
@property (retain) NSString* pathName;

- (void)addBlankDataMatrix;
- (IBAction)cancelAction:(id)sender;
- (IBAction)changeDataType:(id)sender;
- (IBAction)changeBlankDataType:(id)sender;
- (IBAction)changeFileFormat:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolReadData*)t;
- (unsigned)missingForNumStates:(int)n;
- (IBAction)okButtonAction:(id)sender;
- (BOOL)readDataFile;
- (void)setControlsEnabledState;
- (void)setControlWindowSize;
- (void)setToolValues;
- (BOOL)validateMenuItem:(NSMenuItem*)item;

@end
