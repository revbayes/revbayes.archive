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

@property (nonatomic,strong) NSString* fileName;
@property (nonatomic,strong) NSString* pathName;
@property (nonatomic)        int       dataAlignment;
@property (nonatomic)        int       dataFormat;
@property (nonatomic)        int       dataInterleaved;
@property (nonatomic)        int       dataType;
@property (nonatomic)        int       dataTypeSimulated;
@property (nonatomic)        int       numberOfCharacters;
@property (nonatomic)        int       numberOfTaxa;
@property (nonatomic)        int       matrixType;

- (void)addBlankDataMatrix;
- (void)closeControlPanelWithCancel;
- (void)closeControlPanelWithOK;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (unsigned)missingForNumStates:(int)n;
- (BOOL)readDataFile;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;

@end
