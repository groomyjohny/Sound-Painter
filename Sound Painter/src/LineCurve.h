#pragma once
#include <vector>
#include <mutex>

#include "MixerEvent.h"

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
	void addSegment(const LineSegment & seg, std::shared_ptr<std::mutex> mtx);
	void append(double phaseEnd, double ampEnd, std::shared_ptr<std::mutex> mtx);
	const std::vector<LineSegment>& getSegments();
protected:
	std::vector<LineSegment> segments;
};