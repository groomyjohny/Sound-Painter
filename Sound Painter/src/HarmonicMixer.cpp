#include "HarmonicMixer.h"

HarmonicMixer::HarmonicMixer(int nHarmonics)
{
	this->nHarmonics = nHarmonics;
	events.resize(nHarmonics);
	for (auto& it : events) it.setAmplitude(0);
}

void HarmonicMixer::saveSoundToFile(std::string fileName)
{
	for (int i = 0; i < 127; ++i)
	{
		setFundamentalFrequency(440 * pow(2, (i - 69) / 12.0));
		Mixer::saveSoundToFile(std::to_string(i) + " " + fileName);
	}
}

void HarmonicMixer::saveSpectrumToFile(std::string fileName)
{
}

void HarmonicMixer::setHarmonicAmplitude(int harmonicNumber, double amp)
{
	events[harmonicNumber].setAmplitude(amp);
}

void HarmonicMixer::setHarmonicDbAmplitude(int harmonicNumber, double dbAmp)
{
	double amp = pow(10, dbAmp / 10);
	this->setHarmonicAmplitude(harmonicNumber, amp);
}

void HarmonicMixer::setFundamentalFrequency(double frq)
{
	this->fundamentalFrequency = frq;
	for (int i = 0; i < nHarmonics; ++i)
	{
		events[i].setFrequency(frq*i);
	}
}

void HarmonicMixer::clear()
{
	Mixer::clear();
}
