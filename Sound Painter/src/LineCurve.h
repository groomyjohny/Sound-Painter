#pragma once
#include "MixerEvent.h"
#include <vector>

struct LineSegment
{
	double phaseBegin, phaseEnd, ampBegin, ampEnd;
	double getValueAtPhase(double p);
};

class LineCurve : public MixerEvent
{
public:
	LineCurve();
	virtual double getValueAtTime(double t); //used for real-time playback
	virtual double getPreciseValueAtTime(double t); //used for saving to a file
	void addSegment(const LineSegment& seg);
	void append(double phaseEnd, double ampEnd);
	const std::vector<LineSegment>& getSegments();
protected:
	std::vector<LineSegment> segments;
};