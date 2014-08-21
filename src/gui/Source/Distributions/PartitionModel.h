#import <Foundation/Foundation.h>
@class DistributionGui;
@class Parm;
@class TaxonBipartition;



@interface PartitionModel : NSObject <NSCoding> {
    
    DistributionGui*                           distribution;
    TaxonBipartition*                          partition;
    Parm*                                      randomVariable;
}

@property (nonatomic,strong) DistributionGui*  distribution;
@property (nonatomic,strong) TaxonBipartition* partition;
@property (nonatomic,strong) Parm*             randomVariable;

- (DistributionGui*)distribution;
- (id)initWithParm:(Parm*)p;
- (void)setDistribution:(DistributionGui *)d;

@end
