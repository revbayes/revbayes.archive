#import <Cocoa/Cocoa.h>
#import "Tool.h"
@class RbData;
@class WindowControllerCharacterMatrix;

namespace RevBayesCore {
    class AbstractCharacterData;
}

@interface ToolData : Tool <NSCoding> {

    WindowControllerCharacterMatrix*   dataInspector;	
	NSMutableArray*                    dataMatrices;
    NSString*                          dataWorkspaceName;
    int                                numAligned;
    int                                numUnaligned;
}

@property (readwrite,retain) NSString* dataWorkspaceName;
@property (readwrite)        int       numAligned;
@property (readwrite)        int       numUnaligned;

- (void)addMatrix:(RbData*)m;
- (NSMutableArray*)dataMatrices;
- (RbData*)dataMatrixIndexed:(int)i;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (NSMutableArray*)getAlignedData;
- (NSMutableArray*)getUnalignedData;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)instantiateDataInCore;
- (void)makeDataInspector;
- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(const RevBayesCore::AbstractCharacterData&)cd;
- (NSString*)nameOfMatrixIndexed:(int)idx;
- (int)numDataMatrices;
- (int)numAlignedMatrices;
- (int)numUnalignedMatrices;
- (void)readDataError:(NSString*)eName forVariableNamed:(NSString*)vName;
- (void)removeAllDataMatrices;
- (void)removeDataInspector;

@end
