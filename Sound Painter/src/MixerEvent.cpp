#include "MixerEvent.h"
#include <cmath>

MixerEvent::MixerEvent(double frq, double amp)
{
	this->amplitude = amp;
	this->frequency = frq;
}

double MixerEvent::getValueAtTime(double t)
{
	return getPreciseValueAtTime(t);
}

double MixerEvent::getPreciseValueAtTime(double t)
{
	return sin(2 * M_PI*getFrequency()*t)*getAmplitude();
}

double MixerEvent::getFrequency()
{
	return this->frequency;
}

double MixerEvent::getAmplitude()
{
	return amplitude;
}

double MixerEvent::getDbAmplitude()
{
	return log10(amplitude) * 10;
}
