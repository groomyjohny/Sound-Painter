#include "LineCurve.h"

double LineSegment::getValueAtPhase(double p)
{
	if (p < phaseBegin || p > phaseEnd) return 0;
	double pp = (p - phaseBegin) / (phaseEnd - phaseBegin);
	return pp*ampBegin + (1-pp)*ampEnd;
}

LineCurve::LineCurve()
{
	segments = std::vector<LineSegment>();
}

double LineCurve::getValueAtTime(double t)
{
	return getPreciseValueAtTime(t);
}

double LineCurve::getPreciseValueAtTime(double t)
{
	if (segments.empty()) return 0;
	double l = segments.front().phaseBegin;
	double h = segments.back().phaseEnd;

	double acc = 0;
	int div = 0;
	double f = getFrequency();
	double truePhase = std::fmod(t, 1.0 / f)*f;
	double stretchedPhase = l + truePhase*(h - l);
	for (auto& it : segments)
	{
		double v = it.getValueAtPhase(stretchedPhase);
		acc += v;
		if (v > 0) div++;
	}
	return (acc / div * 2) - 1;
}

void LineCurve::addSegment(const LineSegment & seg)
{
	segments.emplace_back(seg);
}

void LineCurve::append(double phaseEnd, double ampEnd)
{
	LineSegment seg;
	bool e = segments.empty();
	seg.phaseBegin = e ? phaseEnd : segments.back().phaseEnd;
	seg.phaseEnd = phaseEnd;
	seg.ampBegin = e ? ampEnd : segments.back().ampEnd;
	seg.ampEnd = ampEnd;
	addSegment(seg);
}

const std::vector<LineSegment>& LineCurve::getSegments()
{
	return segments;
}
