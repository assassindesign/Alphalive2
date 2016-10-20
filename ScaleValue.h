//
//  ScaleValue.h
//  Alphalive 2
//
//  Created by Luke Saxton on 01/08/2016.
//
//

#ifndef ScaleValue_h
#define ScaleValue_h

static double scaleValue (double value, double minValue, double maxValue, double minRange, double maxRange)
{
    return (((maxRange - minRange) *
             (value - minValue)) /
            (maxValue - minValue)) + minRange;
}

#endif /* ScaleValue_h */
