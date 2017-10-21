//------------------------------------------------------------------------------
// global settings for abstract ASRABS design
//
//------------------------------------------------------------------------------

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

// period to send ticks from tick counters to ecu (in ms)
#define TICKPERIOD 100

// minimal speed for which ABS is active, 1666 mm/s = 1.7 m/s = 6 km/h
#define ABSACTIVE 166

// maximally admittable slack is 13 %
#define lambda_abs 13

// maximally admittable skip is 13 %
#define lambda_asr 13

// plus_a just exceeds zero
#define plus_a 2

//  max. wheel decceleration is 14 m/s2 = 14000 mm/s2 = 140 mm/(100ms)2
#define minus_a -140

// +A = 10 * g = 10 * 9.81 [m/s2] = 98100 [mm/s2] = 981 [mm/(100ms)2]
#define plus_A 981

// estimated decceleration on optimal ground, used for reference speed estimation, 1680 [mm/s2] = 17 [mm/(100ms)2]
#define AREF -17

// v = s/t = Ticks/50 * 2 * pi * r = 3770 * Ticks [mm/TICKPERIODms]
#define VELOFACTOR 3770/TICKPERIOD

// size of the array for temporary saved ticks
#define TASIZE 4

//#define DEBUG

#endif

