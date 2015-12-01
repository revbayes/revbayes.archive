#import "RbDataCell.h"



@implementation RbDataCell

@synthesize isDiscrete;
@synthesize isAmbig;
@synthesize isGapState;
@synthesize row;
@synthesize column;
@synthesize dataType;
@synthesize numStates;
@synthesize val;

- (void)encodeWithCoder:(NSCoder *)aCoder {

	[aCoder encodeBool:isDiscrete    forKey:@"isDiscrete"];
	[aCoder encodeBool:isAmbig       forKey:@"isAmbig"];
    [aCoder encodeBool:isGapState    forKey:@"isGapState"];
	[aCoder encodeInt:int(row)       forKey:@"row"];
	[aCoder encodeInt:int(column)    forKey:@"column"];
	[aCoder encodeInt:dataType       forKey:@"dataType"];
	[aCoder encodeInt:int(numStates) forKey:@"numStates"];
	[aCoder encodeObject:val         forKey:@"val"];
}

- (float)getContinuousState {

	if (isDiscrete == NO)
		{
		NSNumber* n = [self val];
		float x = [n floatValue];
		return x;
		}
	return 0.0;
}

- (char)getDiscreteState {

	if (isDiscrete == YES)
		{
		NSNumber* n = [self val];
		unsigned int x = [n unsignedIntValue];
		if (dataType == DNA)
			{
			return [self interpretAsDna:x];
			}
		else if (dataType == RNA)
			{
			char v = [self interpretAsDna:x];
			if (v == 'T')
				v = 'U';
			return v;
			}
		else if (dataType == AA)
			{
			return [self interpretAsAminoAcid:x];
			}
		else if (dataType == STANDARD)
			{
			return [self interpretAsStandard:x];
			}
		return '*';
		}
	return '*';
}

- (char)getDiscreteStateWithLabels:(NSString*)labels {

	if (isDiscrete == YES)
		{
		NSNumber* n = [self val];
		unsigned int x = [n unsignedIntValue];
		if (dataType == DNA)
			{
			return [self interpretAsDna:x];
			}
		else if (dataType == RNA)
			{
			char v = [self interpretAsDna:x];
			if (v == 'T')
				v = 'U';
			return v;
			}
		else if (dataType == AA)
			{
			return [self interpretAsAminoAcid:x];
			}
		else if (dataType == STANDARD)
			{
			return [self interpretAsStandard:x withLabels:labels];
			}
		return '*';
		}
	return '*';
}

- (char)interpretAsAminoAcid:(unsigned)x {

    if (isGapState == YES)
        return '-';
	char aaCode[20] = { 'A', 'R', 'N', 'D', 'C', 'Q', 'E', 'G', 'H', 'I', 'L', 'K', 'M', 'F', 'P', 'S', 'T', 'W', 'Y', 'V' };
	char v = ' ';
	int nOn = 0;
	for (int i=0; i<20; i++)
		{
		unsigned mask = 1 << i ;
		if ( (x & mask) != 0 )
			{
			v = aaCode[i];
			nOn++;
			}
		}
	if (nOn > 1)
		return 'N';
	return v;
}

- (char)interpretAsDna:(unsigned)x {

    if (isGapState == YES)
        return '-';
	if (x == 1)
		return 'A';
	else if (x == 2)
		return 'C';
	else if (x == 3)
		return 'M';
	else if (x == 4)
		return 'G';
	else if (x == 5)
		return 'R';
	else if (x == 6)
		return 'S';
	else if (x == 7)
		return 'V';
	else if (x == 8)
		return 'T';
	else if (x == 9)
		return 'W';
	else if (x == 10)
		return 'Y';
	else if (x == 11)
		return 'H';
	else if (x == 12)
		return 'K';
	else if (x == 13)
		return 'D';
	else if (x == 14)
		return 'B';
	else if (x == 15)
		return 'N';
	return 'N';
}

- (char)interpretAsStandard:(unsigned)x {

	char stCode[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	char v = ' ';
	int nOn = 0;
	for (int i=0; i<10; i++)
		{
		unsigned mask = 1 << i ;
		if ( (x & mask) != 0 )
			{
			v = stCode[i];
			nOn++;
			}
		}
	if (nOn > 1)
		{
		return 'N';
		}
	return v;
}

- (char)interpretAsStandard:(unsigned)x withLabels:(NSString*)labels {
    
    char stCode[32];
    for (size_t i=0; i<[labels length]; i++)
        stCode[i] = [labels characterAtIndex:i];

	char v = ' ';
	int nOn = 0;
	for (size_t i=0; i<[labels length]; i++)
		{
		unsigned mask = 1 << i ;
		if ( (x & mask) != 0 )
			{
			v = stCode[i];
			nOn++;
			}
		}
	if (nOn > 1)
		{
		return 'N';
		}
	return v;
}

- (id)init {

    if ( (self = [super init]) ) 
		{
		val = nil;
        
        // initialize some variables
		isDiscrete  = YES;
		row         = 0;
		column      = 0;
		dataType    = DNA;
		numStates   = 4;
		isAmbig     = NO;
        isGapState  = NO;
		}
    return self;
}

- (id)initWithCell:(RbDataCell*)c {

    if ( (self = [super init]) )
		{
        // initialize some variables
		val         = nil;
		isDiscrete  = YES;
		row         = 0;
		column      = 0;
		dataType    = DNA;
		numStates   = 4;
		isAmbig     = NO;
        isGapState  = NO;

		// copy information from the other RbDataCell object
		if (c != nil)
			{
			isDiscrete = [c isDiscrete];
			row        = [c row];
			column     = [c column];
			dataType   = [c dataType];
			numStates  = [c numStates];
			isAmbig    = [c isAmbig];
            isGapState = [c isGapState];
			if (isDiscrete == YES)
				{
				unsigned x = [[c val] unsignedIntValue];
				NSNumber* n = [NSNumber numberWithUnsignedInt:x];
				[self setVal:n];
				}
			else 
				{
				double x = [[c val] doubleValue];
				NSNumber* n = [NSNumber numberWithDouble:x];
				[self setVal:n];
				}
			}
		}
    return self;
}

- (id)initWithCoder:(NSCoder*)aDecoder {

    if ( (self = [super init]) ) 
		{
		isDiscrete = [aDecoder decodeBoolForKey:@"isDiscrete"];
		isAmbig    = [aDecoder decodeBoolForKey:@"isAmbig"];
        isGapState = [aDecoder decodeBoolForKey:@"isGapState"];
		row        = [aDecoder decodeIntForKey:@"row"];
		column     = [aDecoder decodeIntForKey:@"column"];
		dataType   = [aDecoder decodeIntForKey:@"dataType"];
		numStates  = [aDecoder decodeIntForKey:@"numStates"];
		val        = [aDecoder decodeObjectForKey:@"val"];
		}
	return self;
}

- (void)setDiscreteStateTo:(int)x {

    if ([self isDiscrete] == YES)
        {
        if (x == -1)
            {
            unsigned binaryRep = 0;
            for (size_t i=0; i<numStates; i++)
                {
                unsigned mask = 1 << i ;
                binaryRep |= mask;
                }
            NSNumber* n = [NSNumber numberWithUnsignedInt:binaryRep];
            [self setVal:n];
            [self setIsAmbig:YES];
            }
        else 
            {
            unsigned binaryRep = (unsigned)pow(2.0, (double)x);
            NSNumber* n = [NSNumber numberWithUnsignedInt:binaryRep];
            [self setVal:n];
            [self setIsAmbig:NO];
            }
        }
}

- (unsigned)unsignedRepresentation {

    unsigned x = 0;
	if (isDiscrete == YES)
		{
		NSNumber* n = [self val];
		x = [n unsignedIntValue];
        }
    return x;
}

@end
