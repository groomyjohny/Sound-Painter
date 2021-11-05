#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <iomanip>

#include "Mixer.h"
#include "MixerEvent.h"

#pragma pack(push, 1) //disable padding just for this struct
struct WavHeader
{
	uint32_t chunkId = 0x46464952; //magic number
	uint32_t chunkSize;
	uint32_t format = 0x45564157; //magic number
	uint32_t subchunk1Id = 0x20746d66; //magic number
	uint32_t subchunk1Size = 16; //constant
	uint16_t audioFormat = 1; //no compression PCM
	uint16_t numChannels = 1; //mono
	uint32_t sampleRate = 44100;
	uint32_t byteRate; //bytes per second
	uint16_t blockAlign; //bytes per sample for all channels
	uint16_t bitsBerSample = 16;
	uint32_t subchunk2Id = 0x61746164;
	uint32_t subchunk2Size;

	void finalize(uint32_t sampleCount)
	{
		blockAlign = numChannels * bitsBerSample / 8;
		byteRate = sampleRate * blockAlign;
		subchunk2Size = blockAlign * sampleCount;
		chunkSize = subchunk2Size + 44 - 8;
	}

	WavHeader(uint32_t sampleCount)
	{
		finalize(sampleCount);
	}
	WavHeader() = default;
};
#pragma pack(pop)

int16_t double2word(double n) //smash double in range[-1...1] to a int16
{
	int16_t w = 0x7FFF * n;
	if (n > 1 || n < -1)
	{
		std::cout << (std::string("Sample out of range: ") + std::to_string(n)) << "\n";
		if (n > 1) w = 0x7FFF;
		else w = 0xFFFF;
	}
	return w;
}

void Mixer::saveSoundToFile(std::string fileName)
{
	saveSoundToFile(std::filesystem::path(fileName));
}
void Mixer::saveSoundToFile(std::filesystem::path fileName)
{
	auto& mx = *this;
	double seconds = mx.getDuration();
	int sampleRate = 48000;
	int sampleCount = seconds * sampleRate;
	WavHeader wavHeader;
	wavHeader.sampleRate = sampleRate;
	wavHeader.finalize(sampleCount);

	std::ofstream f(fileName, std::ios::binary);
	f.write((char*)&wavHeader, sizeof wavHeader);
	for (int i = 0; i < sampleCount; ++i)
	{
		double t = i / double(sampleRate);
		double n = mx.getSample(t);
		int16_t sample = double2word(n);
		f.write((char*)&sample, sizeof sample);
	}
	f.close();
}
void Mixer::saveSpectrumToFile(std::string fileName)
{
	std::ofstream f(fileName);
	const int p = 15;
	f.precision(p);
	for (auto& it : this->events)
	{
		f  << std::setw(p + 5) << it.getFrequency() << std::setw(p+5) << it.getDbAmplitude() << "\n";
	}
}
void Mixer::addEvent(const MixerEvent & evt)
{
	events.emplace_back(evt);
}

void Mixer::clear()
{
	events.clear();
}
std::vector<float> Mixer::getSamplesFromUntil(double tBegin, double tEnd, SDL_AudioSpec spec)
{
	std::vector<float> ret;
	double secPerCycle = 1.0/spec.freq;

	for (double t = tBegin; t < tEnd; t += secPerCycle)
	{
		ret.push_back(this->getSample(t));
	}
	return ret;
}

Mixer::Mixer(std::string fileName)
{
	std::ifstream f(fileName);
	double frq, db;
	while (f >> frq >> db)
	{
		MixerEvent evt(frq, pow(10, db / 10));
		this->addEvent(evt);
	}
}

double Mixer::getSample(double t)
{
	if (events.empty()) return 0;

	double sum = 0;
	double maxAmplitude = 0;
	for (auto& it : events)
	{
		sum += it.getValueAtTime(t);
		maxAmplitude += it.getAmplitude();
	}
	return sum / maxAmplitude;
}

double Mixer::getDuration()
{
	return FILE_EXPORT_DURATION;
}

size_t Mixer::getEventCount()
{
	return events.size();
}
