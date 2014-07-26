#import <Cocoa/Cocoa.h>
#import "Parm.h"

#define SRC_FIXED_INDEX     0
#define SRC_NUMBER_MATRICES 1
#define SRC_NUMBER_CHARS    2
#define SRC_NUMBER_TAXA     3
#define SRC_BIPARTITIONS    4




@interface ParmPlate : Parm <NSCoding> {

	NSRect                   plateRect;
	NSSize                   minPlateSize;
	NSRange                  italicsRange;
    
	char                     indexLetter;
	int                      indexSource;
	int                      indexValue;
	int                      numElements;

    char                     startingIndexLetter;
    int                      startingIndexSource;
	int                      startingIndexValue;
    int                      startingNumElements;

    IBOutlet NSTextField*    indexRangeField;
    IBOutlet NSPopUpButton*  indexSelector;
    IBOutlet NSPopUpButton*  indexRangeSelector;
	IBOutlet NSTextField*    valueLabelField;
	IBOutlet NSTextField*    msgLabelField;
	IBOutlet NSBox*          msgSeparator;
}

@property (nonatomic) char    indexLetter;
@property (nonatomic) int     indexValue;
@property (nonatomic) NSRect  plateRect;
@property (nonatomic) int     numElements;
@property (nonatomic) NSSize  minPlateSize;
@property (nonatomic) int     indexSource;
@property (nonatomic) NSRange italicsRange;

- (IBAction)changeIndexValue:(id)sender;
- (IBAction)changeIndexSource:(id)sender;
- (NSString*)getEndingRangeForPlate;
- (void)initializeImage;
- (id)initWithScaleFactor:(float)sf andTool:(ToolModel*)t;
- (void)refreshWindow;
- (void)rememberSettings;
- (void)showControlPanel;
- (void)updateParameterSettings;

@end
