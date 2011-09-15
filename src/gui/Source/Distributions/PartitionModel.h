#import <Foundation/Foundation.h>
@class DistributionGui;
@class Parm;
@class TaxonBipartition;



@interface PartitionModel : NSObject <NSCoding> {
    
    DistributionGui*         distribution;
    TaxonBipartition*        partition;
    Parm*                    randomVariable;
}

@property (retain) TaxonBipartition* partition;
@property (retain) Parm* randomVariable;

- (DistributionGui*)distribution;
- (id)initWithParm:(Parm*)p;
- (void)setDistribution:(DistributionGui *)d;

@end
