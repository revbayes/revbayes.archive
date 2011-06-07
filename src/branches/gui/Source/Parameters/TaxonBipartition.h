#import <Cocoa/Cocoa.h>

#define ANY_PART       0
#define TIP_PARTS      1
#define INT_PARTS      2
#define ROOT_PART      3
#define USER_PART      4



@interface TaxonBipartition : NSObject <NSCoding> {

    int                     partitionType;
    NSString*               partitionName;
    NSMutableArray*         taxaInPartition;
}

@property (readwrite) int partitionType;
@property (readwrite,retain) NSString* partitionName;

- (void)addNameToPartitionSet:(NSString*)nme;
- (TaxonBipartition*)clone;
- (NSString*)getNameOfTaxonInPartitionWithIndex:(int)idx;
- (id)initWithTaxonBipartition:(TaxonBipartition*)bp;
- (int)numTaxaInPartition;

@end
