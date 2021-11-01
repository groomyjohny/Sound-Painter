#pragma once
#include <array>
class MixerEvent
{
public:
	MixerEvent(double frq, double amp = 1);
	double getValueAtTime(double t); //used for real-time playback
	double getPreciseValueAtTime(double t); //used for saving to a file

	double getFrequency();
	double getAmplitude();
	double getDbAmplitude();

	static const int LOOKUP_TABLE_SAMPLES = 65536;
	static std::array<double, LOOKUP_TABLE_SAMPLES> lookupTable;
private:
	double cycleTime, amplitude = 1;
};