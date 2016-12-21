#include "FrameCounter.h"



void FrameCounter::onFrame()
{
	m_frames++;
}

unsigned int FrameCounter::CountedFrames()
{
	return m_frames;
}