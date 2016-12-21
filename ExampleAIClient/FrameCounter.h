#pragma once
class FrameCounter
{
public:
	static FrameCounter& getInstance()
	{
		static FrameCounter    instance;
		return instance;
	}
	void onFrame();
	unsigned int CountedFrames();
private:
	FrameCounter() {}
	FrameCounter(FrameCounter const&);              // Don't Implement.
	void operator=(FrameCounter const&); // Don't implement

	unsigned int m_frames;
};