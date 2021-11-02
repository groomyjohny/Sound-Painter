#include "MixerEvent.h"
#include <cmath>

MixerEvent::MixerEvent(double frq, double amp)
{
	this->setAmplitude(amp);
	this->setFrequency(frq);
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

void MixerEvent::setFrequency(double frq)
{
	this->argMult = frq * 2 * M_PI;
}

void MixerEvent::setAmplitude(double amp)
{
	this->amplitude = amp;
}

void MixerEvent::setDbAmplitude(double dbAmp)
{
	this->setAmplitude(pow(10, dbAmp / 10));
}
