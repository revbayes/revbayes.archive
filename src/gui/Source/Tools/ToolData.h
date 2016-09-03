#import <Cocoa/Cocoa.h>
#import "Tool.h"
#include <string>

@class RbData;
@class WindowControllerCharacterMatrix;

namespace RevBayesCore {

    class AbstractCharacterData;
}

@interface ToolData : Tool <NSCoding> {

    WindowControllerCharacterMatrix*   dataInspector;	
	NSMutableArray*                    dataMatrices;
    NSString*                          dataWorkspaceName;
    size_t                             numAligned;
    size_t                             numUnaligned;
}

@property (nonatomic,strong) NSString* dataWorkspaceName;
@property (nonatomic)        size_t    numAligned;
@property (nonatomic)        size_t    numUnaligned;

- (void)addMatrix:(RbData*)m;
- (NSMutableArray*)dataMatrices;
- (RbData*)dataMatrixIndexed:(size_t)i;
- (void)encodeWithCoder:(NSCoder*)aCoder;
- (BOOL)execute;
- (NSMutableArray*)getAlignedData;
- (NSMutableArray*)getUnalignedData;
- (id)initWithCoder:(NSCoder*)aDecoder;
- (id)initWithScaleFactor:(float)sf;
- (void)instantiateDataInCore;
- (void)makeDataInspector;
- (RbData*)makeNewGuiDataMatrixFromCoreMatrixWithAddress:(const RevBayesCore::AbstractCharacterData&)cd andDataType:(const std::string&)dt;
- (NSString*)nameOfMatrixIndexed:(int)idx;
- (size_t)numDataMatrices;
- (size_t)numAlignedMatrices;
- (size_t)numUnalignedMatrices;
- (void)readDataError:(NSString*)eName forVariableNamed:(NSString*)vName;
- (void)removeAllDataMatrices;
- (void)removeDataInspector;

@end
