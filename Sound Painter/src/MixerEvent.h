#pragma once
#include <array>
class MixerEvent
{
public:
	MixerEvent() = default;
	MixerEvent(double frq, double amp = 1);
	virtual double getValueAtTime(double t); //used for real-time playback
	virtual double getPreciseValueAtTime(double t); //used for saving to a file

	virtual double getFrequency();
	virtual double getAmplitude();
	virtual double getDbAmplitude();

	virtual void setFrequency(double frq);
	virtual void setAmplitude(double amp);
	virtual void setDbAmplitude(double dbAmp);
protected:
	double argMult, amplitude = 1;
};