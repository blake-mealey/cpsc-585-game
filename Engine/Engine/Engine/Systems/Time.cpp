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

double Time::GetTimeMilliSeconds() {
	return timeValueSeconds * 1000;
}

double Time::operator-(Time _timeValueSeconds) {
	return timeValueSeconds - _timeValueSeconds.GetTimeSeconds();
}

double Time::operator+(Time _timeValueSeconds) {
	return timeValueSeconds + _timeValueSeconds.GetTimeSeconds();
}