#include "MixerEvent.h"
#include <cmath>

MixerEvent::MixerEvent(double frq, double amp)
{
	this->amplitude = amp;
	this->argMult = frq*2*M_PI;
}

double MixerEvent::getValueAtTime(double t)
{
	return getPreciseValueAtTime(t);
}

double MixerEvent::getPreciseValueAtTime(double t)
{
	return sin(argMult*t)*getAmplitude();
}

double MixerEvent::getFrequency()
{
	return this->argMult/(2*M_PI);
}

double MixerEvent::getAmplitude()
{
	return amplitude;
}

double MixerEvent::getDbAmplitude()
{
	return log10(amplitude) * 10;
}
