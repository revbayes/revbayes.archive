#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class RbData;
@class WindowControllerCharacterMatrix;
@class WindowControllerReadData;



@interface ToolReadData : Tool <NSCoding> {

	NSString*                          fileName;
	NSString*                          pathName;
    NSString*                          dataWorkspaceName;
	int                                matrixType;
    int                                dataAlignment;
    int                                dataFormat;
	int                                dataType;
	int                                dataTypeSimulated;
	int                                dataInterleaved;
	int                                numberOfTaxa;
	int                                numberOfCharacters;
    WindowControllerReadData*          controlWindow;
    WindowControllerCharacterMatrix*   dataInspector;	
	NSMutableArray*                    dataMatrices;
}

@property (readwrite,retain) NSString* fileName;
@property (readwrite,retain) NSString* pathName;
@property (readwrite,retain) NSString* dataWorkspaceName;
@property (readwrite) int dataAlignment;
@property (readwrite) int dataFormat;
@property (readwrite) int dataInterleaved;
@property (readwrite) int dataType;
@property (readwrite) int dataTypeSimulated;
@property (readwrite) int numberOfCharacters;
@property (readwrite) int numberOfTaxa;
@property (readwrite) int matrixType;
@property (readwrite,assign) NSMutableArray* dataMatrices;

- (void)addMatrix:(RbData*)m;
- (void)closeControlPanel;
- (RbData*)dataMatrixIndexed:(int)i;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)instantiateDataInCore;
- (NSString*)nameOfMatrixIndexed:(int)idx;
- (int)numDataMatrices;
- (void)removeAllDataMatrices;
- (NSMutableAttributedString*)sendTip;
- (void)showControlPanel;
- (void)showInspectorPanel;

@end
