#import <Cocoa/Cocoa.h>
#import "ToolData.h"
@class RbData;
@class WindowControllerCharacterMatrix;
@class WindowControllerReadData;



@interface ToolReadData : ToolData <NSCoding> {

	NSString*                          fileName;
	NSString*                          pathName;
	int                                matrixType;
    int                                dataAlignment;
    int                                dataFormat;
	int                                dataType;
	int                                dataTypeSimulated;
	int                                dataInterleaved;
	int                                numberOfTaxa;
	int                                numberOfCharacters;
    WindowControllerReadData*          controlWindow;
}

@property (readwrite,retain) NSString* fileName;
@property (readwrite,retain) NSString* pathName;
@property (readwrite)        int       dataAlignment;
@property (readwrite)        int       dataFormat;
@property (readwrite)        int       dataInterleaved;
@property (readwrite)        int       dataType;
@property (readwrite)        int       dataTypeSimulated;
@property (readwrite)        int       numberOfCharacters;
@property (readwrite)        int       numberOfTaxa;
@property (readwrite)        int       matrixType;

- (void)addBlankDataMatrix;
- (void)closeControlPanel;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (unsigned)missingForNumStates:(int)n;
- (BOOL)readDataFile;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
