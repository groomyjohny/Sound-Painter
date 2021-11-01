#include "MixerEvent.h"
#include <cmath>

MixerEvent::MixerEvent(double frq, double amp)
{
	this->amplitude = amp;
	this->cycleTime = 1.0 / frq;
}

double MixerEvent::getValueAtTime(double t)
{
	double phase = std::fmod(t, this->cycleTime) / this->cycleTime;
	int ind = phase * LOOKUP_TABLE_SAMPLES;
	double n1 = lookupTable[ind];
	/*double n2 = lookupTable[(ind + 1) % LOOKUP_TABLE_SAMPLES]; //wrap around to 0th index
	double interpolated = n1 * (1 - phase) + n2 * phase; //linear interpolation between 2 neighbouring nodes*/
	return n1 * getAmplitude();
}

double MixerEvent::getPreciseValueAtTime(double t)
{
	return sin(2 * M_PI*getFrequency()*t)*getAmplitude();
}

double MixerEvent::getFrequency()
{
	return 1.0 / this->cycleTime;
}

double MixerEvent::getAmplitude()
{
	return amplitude;
}

double MixerEvent::getDbAmplitude()
{
	return log10(amplitude) * 10;
}
