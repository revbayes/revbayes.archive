#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class RbData;
@class WindowControllerCharacterMatrix;
class CharacterData;


@interface ToolData : Tool <NSCoding> {

    WindowControllerCharacterMatrix*   dataInspector;	
	NSMutableArray*                    dataMatrices;
    NSString*                          dataWorkspaceName;
}

@property (readwrite,retain) NSString* dataWorkspaceName;

- (void)addMatrix:(RbData*)m;
- (RbData*)dataMatrixIndexed:(int)i;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)instantiateDataInCore;
- (void)instantiateDataInspector;
- (void)makeDataInspector;
- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(CharacterData*)cd;
- (NSString*)nameOfMatrixIndexed:(int)idx;
- (int)numDataMatrices;
- (void)removeAllDataMatrices;
- (void)removeDataInspector;
- (void)removeFilesFromTemporaryDirectory;

@end
