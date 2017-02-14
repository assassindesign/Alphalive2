//
//  ScaleValue.h
//  Alphalive 2
//
//  Created by Luke Saxton on 01/08/2016.
//
//
//  Alphalive 2 is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License, version 2,
//  as published by the Free Software Foundation.
//
//  Alphalive 2 is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ScaleValue_h
#define ScaleValue_h

static double scaleValue (double value, double minValue, double maxValue, double minRange, double maxRange)
{
    return (((maxRange - minRange) *
             (value - minValue)) /
            (maxValue - minValue)) + minRange;
}

#endif /* ScaleValue_h */
