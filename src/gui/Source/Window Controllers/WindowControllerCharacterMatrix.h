#import <Cocoa/Cocoa.h>
@class RbData;
@class ToolData;


@interface WindowControllerCharacterMatrix : NSWindowController {

	IBOutlet NSSplitView*              windowSplitView;
	IBOutlet NSScrollView*             dataScrollView;
	IBOutlet NSScrollView*             namesScrollView;
	IBOutlet NSPopUpButton*            matrixSelector;
    IBOutlet NSTextField*              numTaxaIndicator;
    IBOutlet NSTextField*              numCharIndicator;
    IBOutlet NSTextField*              dataTypeIndicator;
    IBOutlet NSTextField*              numExcludedTaxaIndicator;
    IBOutlet NSTextField*              numExcludedCharIndicator;
    IBOutlet NSButton*                 showInfoButton;

	NSFont*                            dataNamesFont;
	NSFont*                            headerFont;
	NSColor*                           headerBackground;
	NSColor*                           namesBackground;
	NSColor*                           dataTextColor;
	NSColor*                           namesTextColor;
	NSColor*                           headerTextColor;
	NSDictionary*                      aminoColorsDict;
	NSDictionary*                      nucleotideColorsDict;
	NSDictionary*                      standardColorsDict;

    ToolData*                          myTool;
    int                                numTaxa;
    int                                numChar;
	float                              cellWidth;
	float                              cellHeight;
	NSMutableArray*                    dataMatrices;
	NSMutableArray*                    taxaMatrices;
}

@property (readwrite) float cellWidth;
@property (readwrite) float cellHeight;

- (NSDictionary*)aminoColorsDict;
- (IBAction)changeMatrix:(id)sender;
- (IBAction)closeAction:(id)sender;
- (IBAction)helpButtonAction:(id)sender;
- (BOOL)initDataMatrixWithMatrix:(RbData*)matrix;
- (id)initWithTool:(id)t;
- (void)initStateColorDefaults;
- (float)lengthOfLongestName:(NSMatrix*)matrix;
- (NSDictionary*)nucleotideColorsDict;
- (void)setAminoColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray;
- (void)setNucleotideColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray;
- (void)setStandardColorsDictKeys:(NSArray*)keysArray forValues:(NSArray*)valueArray;
- (NSDictionary*)standardColorsDict;
- (void)synchronizedViewContentBoundsDidChange:(NSNotification*)notification;

@end
