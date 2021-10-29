#include "Mixer.h"
#include <fstream>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <iostream>

const double PI = 3.14159265359;

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

void Mixer::saveToFile(std::string name)
{
	auto& mx = *this;
	double seconds = mx.getDuration();
	int sampleRate = 48000;
	int sampleCount = seconds * sampleRate;
	WavHeader wavHeader;
	wavHeader.sampleRate = sampleRate;
	wavHeader.finalize(sampleCount);

	std::ofstream f(name, std::ios::binary);
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
void Mixer::clear()
{
	events.clear();
	currActiveEventIndices.clear();
}
void Mixer::addEvent(const Event & evt)
{
	events.emplace_back(evt);
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
Mixer::Mixer(const std::string & path)
{
	std::ifstream inp(path);
	std::string line;
	std::stringstream ss;

	bool dbAmplitude = false;
	bool spectrumMode = false;
	double spectrumModeDuration = 0;

	while (std::getline(inp, line))
	{
		if (line.empty() || line[0] == ';') continue; //ignore empty lines or comments
		if (line == "amplitude-db") { dbAmplitude = true; continue; }
		if (line == "amplitude-normal") { dbAmplitude = false; continue; }
		if (line == "spectrum")
		{
			dbAmplitude = true;
			spectrumMode = true;
			continue;
		}

		ss.str(line);
		ss.clear();
		Event event;

		if (spectrumMode)
		{
			if (spectrumModeDuration == 0)
			{
				ss >> spectrumModeDuration;
				continue;
			}
			else
			{
				event.tBegin = 0;
				event.tEnd = spectrumModeDuration;
				ss >> event.frequency;
			}
		}
		else
		{
			ss >> event.tBegin >> event.tEnd >> event.frequency;
		}

		double amplitudeRead;
		if (ss >> amplitudeRead)
		{
			double amplitudeNormalValue = amplitudeRead;
			if (dbAmplitude) amplitudeNormalValue = sqrt(pow(10, amplitudeRead / 10));
			event.amplitude = amplitudeNormalValue;
		}

		events.emplace_back(event);
	}

	std::sort(events.begin(), events.end(), [&](const Event& e1, const Event& e2) {return e1.tBegin < e2.tBegin; }); //sort event by ascending begin time
}

double Mixer::sinWave(double fundamentalHz, double t, int harmonicCount)
{
	double acc = 0;
	double maxVol = 0;
	for (int i = 1; i <= harmonicCount; ++i)
	{
		acc += sin(2 * PI * fundamentalHz*i*t) / (i*i);
		maxVol += 1.0 / (i * i);
	}
	return acc / maxVol;
}

double pureSinWave(double fundamentalHz, double t)
{
	return sin(2 * PI*fundamentalHz*t);
}

double Mixer::getSample(double t)
{
	this->advanceTimeTo(t);
	if (currActiveEventIndices.empty()) return 0;

	double sum = 0;
	double maxAmplitude = 0;
	for (auto i : currActiveEventIndices)
	{
		sum += pureSinWave(events[i].frequency, t) * events[i].amplitude;
		maxAmplitude += events[i].amplitude;
	}
	return sum / maxAmplitude;
}

double Mixer::getDuration()
{
	if (events.empty()) return 0;
	double m = events[0].tEnd;
	for (auto& it : events) m = std::max(m, it.tEnd);
	return m;
}

void Mixer::advanceTimeTo(const double t)
{
	for (int i = 0; i < currActiveEventIndices.size(); ++i) //remove expired events
	{
		size_t ind = currActiveEventIndices[i];
		if (events[ind].tEnd <= t && events[ind].tEnd != -1)
		{
			currActiveEventIndices.erase(currActiveEventIndices.begin() + i);
			--i;
		}
	}

	for (; lastStartedInd < events.size(); ++lastStartedInd)
	{
		Event& e = events[lastStartedInd];
		if (e.tBegin <= t && e.tEnd > t || e.tBegin == -1 || e.tEnd == -1)
		{
			currActiveEventIndices.push_back(lastStartedInd);
		}
	}
}
