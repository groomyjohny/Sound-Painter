#pragma once
#include "Mixer.h"
class HarmonicMixer : public Mixer
{
public:
	HarmonicMixer(int nHarmonics);
	virtual int getHarmonicCount();
	virtual void saveSoundToFile(std::string fileName);
	virtual void saveSpectrumToFile(std::string fileName);
	virtual void setHarmonicAmplitude(int harmonicNumber, double amp);
	virtual void setHarmonicDbAmplitude(int harmonicNumber, double dbAmp);
	virtual void setFundamentalFrequency(double frq);
	virtual double getDuration();
	virtual void clear();
protected:
	int nHarmonics = 0;
	double fundamentalFrequency = 0;
};