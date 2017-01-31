#import <Cocoa/Cocoa.h>
#include <string>

#define DNA         0
#define RNA         1
#define AA          2
#define STANDARD    3
#define CONTINUOUS  4
#define MIXED       5



@interface RbDataCell : NSObject <NSCoding> {

	BOOL                               isDiscrete;
	size_t                             row;
	size_t                             column;
	int                                dataType;
	size_t                             numStates;
	BOOL                               isAmbig;
    BOOL                               isGapState;
	NSNumber*                          val;
}

@property (nonatomic)        BOOL      isDiscrete;
@property (nonatomic)        size_t    row;
@property (nonatomic)        size_t    column;
@property (nonatomic)        int       dataType;
@property (nonatomic)        size_t    numStates;
@property (nonatomic)        BOOL      isAmbig;
@property (nonatomic)        BOOL      isGapState;
@property (nonatomic,strong) NSNumber* val;

- (float)getContinuousState;
- (char)getDiscreteState;
- (char)getDiscreteStateWithLabels:(NSString*)labels;
- (char)interpretAsAminoAcid:(unsigned)x;
- (char)interpretAsDna:(unsigned)x;
- (char)interpretAsRna:(unsigned)x;
- (char)interpretAsStandard:(unsigned)x;
- (char)interpretAsStandard:(unsigned)x withLabels:(NSString*)labels;
- (id)initWithCell:(RbDataCell*)c;
- (void)setDiscreteStateTo:(int)x; 
- (unsigned)unsignedRepresentation;
- (unsigned)aaToUnsigned:(std::string)s;
- (unsigned)dnaToUnsigned:(std::string)s;
- (unsigned)rnaToUnsigned:(std::string)s;
- (unsigned)standardToUnsigned:(std::string)s;
- (unsigned)standardToUnsigned:(std::string)s withLabels:(NSString*)labels;

@end
