#import "TaxonBipartition.h"



@implementation TaxonBipartition

@synthesize partitionType;
@synthesize partitionName;

- (void)addNameToPartitionSet:(NSString*)nme {
    
    [taxaInPartition addObject:nme];
}

- (TaxonBipartition*)clone {
    
    return [[TaxonBipartition alloc] initWithTaxonBipartition:self];
}

- (void)encodeWithCoder:(NSCoder *)aCoder {

    [aCoder encodeInt:partitionType      forKey:@"partitionType"];
    [aCoder encodeObject:partitionName   forKey:@"partitionName"];
    [aCoder encodeObject:taxaInPartition forKey:@"taxaInPartition"];
}

- (NSString*)getNameOfTaxonInPartitionWithIndex:(int)idx {

    return [taxaInPartition objectAtIndex:idx];
}

- (id)init {

     if ( (self = [super init]) ) 
		{
        partitionType   = TIP_PARTS;
        partitionName   = [[NSString alloc] initWithString:@"Tip Nodes"];
		taxaInPartition = [[NSMutableArray alloc] init];
		}
    return self;
}

- (id)initWithTaxonBipartition:(TaxonBipartition*)bp {
    
    if ( (self = [super init]) ) 
		{
        partitionType   = [bp partitionType];
        partitionName   = [[NSString alloc] initWithString:[bp partitionName]];
		taxaInPartition = [[NSMutableArray alloc] init];
        for (int i=0; i<[bp numTaxaInPartition]; i++)
            {
            [self addNameToPartitionSet:[bp getNameOfTaxonInPartitionWithIndex:i]];
            }
		}
    return self;
}

- (id)initWithCoder:(NSCoder *)aDecoder {

    if ( (self = [super init]) ) 
		{
        partitionType   = [aDecoder decodeIntForKey:@"partitionType"];
        partitionName   = [aDecoder decodeObjectForKey:@"partitionName"];
        taxaInPartition = [aDecoder decodeObjectForKey:@"taxaInPartition"];
        /* JPHARC [partitionName retain];
        [taxaInPartition retain]; */
		}
	return self;
}

- (int)numTaxaInPartition {

    return (int)[taxaInPartition count];
}

@end
