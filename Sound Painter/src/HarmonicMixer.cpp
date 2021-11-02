#include "HarmonicMixer.h"

HarmonicMixer::HarmonicMixer(int nHarmonics)
{
	this->nHarmonics = nHarmonics;
	events.resize(nHarmonics);
	for (auto& it : events) it.setAmplitude(0);
}

int HarmonicMixer::getHarmonicCount()
{
	return nHarmonics;
}

void HarmonicMixer::saveSoundToFile(std::string fileName)
{
	size_t p1 = fileName.find('.');
	static std::string noteNames[] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A","A#","B" };
	for (int i = 0; i < 128; ++i)
	{
		std::string noteName = noteNames[i % 12] + std::to_string(i / 12 - 1);
		std::string subname = fileName.substr(0, p1) + std::string(" ") + noteName +".wav";
		setFundamentalFrequency(440 * pow(2, (i - 69) / 12.0));
		Mixer::saveSoundToFile(subname);
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

double HarmonicMixer::getDuration()
{
	return 1;
}

void HarmonicMixer::clear()
{
	Mixer::clear();
}
