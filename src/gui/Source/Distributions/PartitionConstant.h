#import <Foundation/Foundation.h>
@class Parm;
@class TaxonBipartition;
@class Value;



@interface PartitionConstant : NSObject <NSCoding> {
        
        Value*                                 value;
        TaxonBipartition*                      partition;
        Parm*                                  constantParm;
    }
    
@property (nonatomic,strong) TaxonBipartition* partition;
@property (nonatomic,strong) Parm*             constantParm;

- (PartitionConstant*)clone;
- (id)initWithParm:(Parm*)p;
- (id)initWithPartitionConstant:(PartitionConstant*)pc;
- (void)setValue:(Value*)v;
- (Value*)value;

@end
