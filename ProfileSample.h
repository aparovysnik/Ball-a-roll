#pragma once
#include "IProfilerOutputHandler.h"
#include <string>
#include "SDL.h"
#include "LFont.h"

class ProfileSample
{
public:
	ProfileSample(std::string sampleName);
	~ProfileSample();

	static void Output();
	static vector<RenderObject*> PrintOnScreen(LFont* f, Vector3 pos);
	void InvalidateSample();
	static void ResetSample(std::string sampleName);
	static void ResetAll();

	static IProfilerOutputHandler *outputHandler;
	static Shader* shader;
	struct profileSample
	{
		profileSample()
		{
			bIsValid = false;
			dataCount = 0;
			averagePc = minPc = maxPc = -1;
		}

		bool bIsValid;    //whether or not this sample is valid to be used
		bool bIsOpen; 	//is this sample currently being profiled?
		unsigned int callCount; //number of times this sample has been executed
		std::string name; //name of the sample

		float startTime;  //starting time on the clock, in seconds
		float totalTime;  //total time recorded across all executions of this sample
		float childTime;  //total time taken by children of this sample

		int parentCount;  //number of parents this sample has
		//(useful for neat indenting)

		float averagePc;  //average percentage of game loop time taken up
		float minPc;      //minimum percentage of game loop time taken up
		float maxPc;      //maximum percentage of game loop time taken up
		unsigned long dataCount; //number of times values have been stored since
		//sample creation/reset
	};

protected:
	//index into the array of samples
	int iSampleIndex;
	int iParentIndex;

	static const int MAX_PROFILER_SAMPLES = 256;

	inline float GetTime(){ return ((float)SDL_GetTicks()) / 1000.0f; }

	static profileSample samples[MAX_PROFILER_SAMPLES];
	static int lastOpenedSample;
	static int openSampleCount;
	static float rootBegin, rootEnd;
};

