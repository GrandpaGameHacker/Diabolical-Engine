#pragma once
#include <string>
#include <iostream>
#include "../Logging/Logging.h"
#include <fstream>
#include <AL/al.h>

class AudioUtility
{
public:
	//check big vs little endian machine

	static ALenum ToAlFormat(short channels, short samples)
	{
		bool stereo = (channels > 1);

		switch (samples) {
		case 16:
			if (stereo)
				return AL_FORMAT_STEREO16;
			else
				return AL_FORMAT_MONO16;
		case 8:
			if (stereo)
				return AL_FORMAT_STEREO8;
			else
				return AL_FORMAT_MONO8;
		default:
			return -1;
		}
	}

	static bool IsBigEndian(void)
	{
		int a = 1;
		return !((char*)&a)[0];
	}

	static int ConvertToInt(char* buffer, int len)
	{
		int a = 0;

		if (!IsBigEndian())
		{
			for (int i = 0; i < len; ++i)
			{
				((char*)&a)[i] = buffer[i];
			}
		}
		else
		{
			for (int i = 0; i < len; ++i)
			{
				((char*)&a)[3 - i] = buffer[i];
			}
		}

		return a;
	}

	static char* LoadWAV(std::string filename, int& channels, int& sampleRate, int& bps, int& size)
	{
		char buffer[4];

		std::ifstream in(filename.c_str());
		in.read(buffer, 4);

		if (strncmp(buffer, "RIFF", 4) != 0)
		{
			LOGERROR("Not a valid WAV file, RIFF not found in header");
			return nullptr;
		}

		in.read(buffer, 4);//size of file. Not used. Read it to skip over it.  

		in.read(buffer, 4);//Format, should be WAVE

		if (strncmp(buffer, "WAVE", 4) != 0)
		{
			LOGERROR("Not a valid WAV file, WAVE not found in header");
			return nullptr;
		}

		in.read(buffer, 4);//Format Space Marker. should equal fmt (space)

		if (strncmp(buffer, "fmt ", 4) != 0)
		{
			LOGERROR("Not a valid WAV file, Format Marker not found in header");
			return nullptr;
		}

		in.read(buffer, 4);//Length of format data. Should be 16 for PCM, meaning uncompressed.

		if (ConvertToInt(buffer, 4) != 16)
		{
			LOGERROR("Not a valid WAV file, format length wrong in header");
			return nullptr;
		}

		in.read(buffer, 2);//Type of format, 1 = PCM

		if (ConvertToInt(buffer, 2) != 1)
		{
			LOGERROR("Not a valid WAV file, file not in PCM format");
			return nullptr;
		}

		in.read(buffer, 2);//Get number of channels. 

		//Assume at this point that we are dealing with a WAV file. This value is needed by OpenAL
		channels = ConvertToInt(buffer, 2);

		in.read(buffer, 4);//Get sampler rate. 

		sampleRate = ConvertToInt(buffer, 4);

		//Skip Byte Rate and Block Align. Maybe use later?
		in.read(buffer, 4);//Block Align
		in.read(buffer, 2);//ByteRate

		in.read(buffer, 2);//Get Bits Per Sample

		bps = ConvertToInt(buffer, 2);

		//Skip character data, which marks the start of the data that we care about. 
		in.read(buffer, 4);//"data" chunk. 

		in.read(buffer, 4); //Get size of the data

		size = ConvertToInt(buffer, 4);

		if (size < 0)
		{
			LOGERROR("Not a valid WAV file, size of file reports 0");
		}

		char* data = new char[size];

		in.read(data, size);//Read audio data into buffer, return.

		in.close();

		return data;
	}
};