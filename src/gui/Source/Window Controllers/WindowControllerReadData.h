#import <Cocoa/Cocoa.h>
#include <vector>
@class ToolReadData;
class CharacterData;



@interface WindowControllerReadData : NSWindowController <NSTabViewDelegate> {

    ToolReadData*                      myTool;
	NSString*                          fileName;
	NSString*                          pathName;
    
	IBOutlet NSTabView*                matrixTypeTab;
	IBOutlet NSButton*                 okButton;
	IBOutlet NSPopUpButton*            dataTypeButton1;
	IBOutlet NSPopUpButton*            dataFormatButton;
	IBOutlet NSPopUpButton*            dataAlignmentButton;
    IBOutlet NSPopUpButton*            fileFormatDeterminationMethod;
	IBOutlet NSSegmentedControl*       interleavedFormatButton;
    IBOutlet NSTextField*              dataFormatField;
	IBOutlet NSTextField*              dataTypeField;
	IBOutlet NSTextField*              dataAlignmentField;
	IBOutlet NSTextField*              interleavedFormatField;
	
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
    BOOL                               isDataFormatAutomaticallyDetermined;
}

@property (nonatomic)        int       dataAlignment;
@property (nonatomic)        int       dataFormat;
@property (nonatomic)        int       dataInterleaved;
@property (nonatomic)        int       dataType;
@property (nonatomic)        int       dataTypeSimulated;
@property (nonatomic)        int       numberOfCharacters;
@property (nonatomic)        int       numberOfTaxa;
@property (nonatomic)        int       numberOfStates;
@property (nonatomic,strong) NSString* fileName;
@property (nonatomic,strong) NSString* pathName;
@property (nonatomic)        BOOL      isDataFormatAutomaticallyDetermined;

- (IBAction)cancelAction:(id)sender;
- (IBAction)changeDataType:(id)sender;
- (IBAction)changeBlankDataType:(id)sender;
- (IBAction)changeFileFormat:(id)sender;
- (IBAction)changeFileFormatDeterminationMethod:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (id)initWithTool:(ToolReadData*)t;
- (BOOL)isBlankMatrixOfDataType:(NSString*)dt;
- (BOOL)makeBlankMatrix;
- (IBAction)okButtonAction:(id)sender;
- (void)setControlsEnabledState;
- (void)setControlWindowSize;
- (void)setToolValues;
- (BOOL)validateMenuItem:(NSMenuItem*)item;

@end
