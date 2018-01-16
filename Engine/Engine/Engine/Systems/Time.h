#pragma once

class Time{
public:
	Time();
	Time(double _timeValueSeconds);

	double GetTimeSeconds();
	double GetTimeMilliSeconds();

	double operator-(Time _timeValueSeconds);
	double operator+(Time _timeValueSeconds);

private:
	double timeValueSeconds;
};