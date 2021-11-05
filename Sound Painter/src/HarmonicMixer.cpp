#include "HarmonicMixer.h"
#include <filesystem>
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

void HarmonicMixer::setHarmoicsCount(int n)
{
	int newEntries = n - nHarmonics;
	nHarmonics = n;
	events.resize(n);
	for (int i = 0; i < newEntries; ++i) events[n - i - 1].setAmplitude(0); //silence new harmonics
}

void HarmonicMixer::saveSoundToFile(std::string fileName)
{
	size_t p0 = fileName.find_last_of('/');
	size_t p1 = fileName.find('.');

	std::string baseName = fileName.substr(p0, p1 - p0);
	std::string folderPath = fileName.substr(0, p1);
	std::filesystem::create_directories(folderPath);
	using fs = std::filesystem::path;

	static std::wstring noteNames[] = { L"C", L"C♯", L"D", L"D♯", L"E", L"F", L"F♯", L"G", L"G♯", L"A",L"A♯", L"B" }; //unicode sharp signs for Polyphone
	for (int i = 0; i < 128; ++i)
	{
		std::wstring noteName = noteNames[i % 12] + std::to_wstring(i / 12 - 1);
		//std::wstring subname = folderPath + baseName + std::wstring(" ") + noteName + ".wav";
		auto subname = std::filesystem::path(folderPath).concat(baseName).concat(" ").concat(noteName).concat(".wav");
		setFundamentalFrequency(440 * pow(2, (i - 69) / 12.0));
		Mixer::saveSoundToFile(subname);
	}
}

void HarmonicMixer::saveSpectrumToFile(std::string fileName)
{
	Mixer::saveSpectrumToFile(fileName);
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
		events[i].setFrequency(frq*(i + 1));
	}
}

double HarmonicMixer::getDuration()
{
	return 1;
}

void HarmonicMixer::clear()
{
	for (auto& it : events) it.setAmplitude(0);
}
