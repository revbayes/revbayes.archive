//
//  YRKSpinningProgressIndicator.h
//
//  Copyright 2009 Kelan Champagne. All rights reserved.
//


@interface YRKSpinningProgressIndicator : NSView {

    int         _position;
    int         _numFins;
    NSColor**   _finColors;
    
    BOOL        _isAnimating;
    BOOL        _isFadingOut;
    NSTimer*    _animationTimer;
	NSThread*   _animationThread;
    
    NSColor*    _foreColor;
    NSColor*    _backColor;
    BOOL        _drawsBackground;
    
    BOOL        _displayedWhenStopped;
    BOOL        _usesThreadedAnimation;

    BOOL        _isIndeterminate;
    double      _currentValue;
    double      _maxValue;
}

@property (nonatomic,strong)                             NSColor* color;
@property (nonatomic,strong)                             NSColor* backgroundColor;
@property (nonatomic,weak)                               BOOL     drawsBackground;
@property (nonatomic,weak,getter=isDisplayedWhenStopped) BOOL     displayedWhenStopped;
@property (nonatomic,weak)                               BOOL     usesThreadedAnimation;
@property (nonatomic,weak,getter=isIndeterminate)        BOOL     indeterminate;
@property (nonatomic,weak)                               double   doubleValue;
@property (nonatomic,weak)                               double   maxValue;

- (void)stopAnimation:(id)sender;
- (void)startAnimation:(id)sender;

@end
