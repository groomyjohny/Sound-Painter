#pragma once
#include <string>
#include <vector>

struct Event
{
	double tBegin, tEnd, frequency, amplitude = 1;
};

class Mixer //only forward-iterating allowed!
{
public:
	Mixer() = default;
	Mixer(const std::string& path);	

	double getSample(double t);
	double getDuration();

	void saveToFile(std::string name);
private:
	std::vector<Event> events;
	std::vector<size_t> currActiveEventIndices;
	int lastStartedInd = 0;

	void advanceTimeTo(const double t);
	double sinWave(double fundamentalHz, double t, int harmonicCount);
};