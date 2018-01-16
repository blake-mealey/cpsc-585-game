#include "Time.h"

#include <iostream>
using namespace std;

Time::Time() {
	timeValueSeconds = 0;
}

Time::Time(double _timeValueSeconds) {
	timeValueSeconds = _timeValueSeconds;
}

double Time::GetTimeSeconds() {
	return timeValueSeconds;
}

/*
Converts stored time value (seconds) to milliseconds
*/
double Time::GetTimeMilliSeconds() {
	return timeValueSeconds * 1000;
}

/*
Overloaded operator to allow for subtraction between Time data types
*/
double Time::operator-(Time _timeValueSeconds) {
	return timeValueSeconds - _timeValueSeconds.GetTimeSeconds();
}

/*
Overloaded operator to allow for addition between Time data types
*/
double Time::operator+(Time _timeValueSeconds) {
	return timeValueSeconds + _timeValueSeconds.GetTimeSeconds();
}