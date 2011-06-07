#import <Foundation/Foundation.h>
#import "ParmPlate.h"
@class ParmTree;



@interface ParmPlateTree : ParmPlate <NSCoding> {
    
}

- (ParmTree*)getOrderingTree;
- (BOOL)isConnectedToTree;
- (BOOL)isPlateConfigurationSensible:(int*)errNum;

@end
