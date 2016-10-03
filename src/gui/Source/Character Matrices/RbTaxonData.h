#import <Cocoa/Cocoa.h>
@class MatrixRowInformation;
@class RbDataCell;



@interface RbTaxonData : NSObject <NSCoding> {

	NSMutableArray*       data;
    int                   dataType;
	NSString*             taxonName;
}

@property (nonatomic)        int       dataType;
@property (nonatomic,strong) NSString* taxonName;

- (void)addObservation:(RbDataCell*)dc;
- (void)cleanName:(NSString*)nameStr;
- (RbDataCell*)dataCellIndexed:(int)idx;
- (id)initWithRbTaxonData:(RbTaxonData*)d;
- (id)initWithMatrixRowInformation:(MatrixRowInformation*)mr;
- (int)numCharacters;
- (void)print;
- (void)removeObervationFromEnd;

@end
