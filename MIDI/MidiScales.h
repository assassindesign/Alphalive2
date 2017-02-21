//
//  MidiScales.h
//  AlphaLearn
//
//  Created by Luke Saxton on 29/02/2016.
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

#ifndef MidiScales_h
#define MidiScales_h

/* 
    Various scales in semitones from root note format, -1 signifies the end of the array.
    Used by playableSphere in the mapSphere function. More scales could easily be added here, 
    although the current algorithmic mapping function only supports 7 note scales.
*/

namespace IntervalSpacings
{
    const char* ScaleNames[] = {"Major", "Natural Minor", "Harmonic Minor", "Hungarian", NULL};
    const int Major[] = {0, 2, 4, 5, 7, 9, 11, 12, -1};
    const int NaturalMinor[] = {0, 2, 3, 5, 7, 8, 10, 12, -1};
    const int HarmonicMinor[] = {0, 2, 3, 5, 7, 8, 11, 12, -1};
    const int Hungarian[] = {0, 2, 3, 6, 7, 8, 11, 12, -1};
}

/*
    These masks convert the default pad layout - (0-47) to different layouts. Need to find a way
    to make this more flexible for spheres of different sizes as this only works for the big 
    sphere.
*/
namespace SphereMask
{
    //const int SphereMask[] = {0,0,0,0,0,0,0,0, 6, 8, 10, 12, 14, 0, 2, 4, 3, 5, 7, 9, 11, 13, 15, 1, 8, 10, 12, 14, 16, 18, 20, 22, 21, 23, 9, 11, 13, 15, 17, 19, 0, 0, 0, 0, 0, 0, 0, 0, -1};
    const int OneRowMask[] = {0 ,1 , 2 ,3 ,4 ,5 ,6 ,7 ,7 ,8 , 9, 10, 11, 12, 13, 14, 14, 15, 16, 17, 18, 19, 20, 21, 21, 22, 23, 24, 25, 26, 27, 28, 28, 29, 30, 31, 32, 33, 34, 35, 35, 36, 37, 38, 39, 40, 41, 42, -1};
    
    const int TwoRowMask[] = {10,12,14,0,2,4,6,8,7,9,11,13,15,1,3,5,11,13,15,17,19,21,7,9,8,10,12,14,16,18,20,22,28,14,16,18,20,22,24,26,25,27,29,15,17,19,21,23, -1};
    
    const int CenterRowsMask[] = {0,0,0,0,0,0,0,0,7,9,11,13,15,1,3,5,4,6,8,10,12,14,0,2,8,10,12,14,16,18,20,22,21,7,9,11,13,15,17,19,0,0,0,0,0,0,0,0, -1};
    
    const int PadsMapped = 48;
}

#endif /* MidiScales_h */
