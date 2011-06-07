#import <Foundation/Foundation.h>
@class Distribution;
@class Parm;
@class TaxonBipartition;



@interface PartitionModel : NSObject <NSCoding> {
    
    Distribution*            distribution;
    TaxonBipartition*        partition;
    Parm*                    randomVariable;
}

@property (retain) TaxonBipartition* partition;
@property (retain) Parm* randomVariable;

- (Distribution*)distribution;
- (id)initWithParm:(Parm*)p;
- (void)setDistribution:(Distribution *)d;

@end
