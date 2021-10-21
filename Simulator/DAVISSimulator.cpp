#include "Simulatorbase.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <cmath>
#include <float.h>

void DAVISSimulator::resetMemory(int width, int height, int channels)
{
	/*
	 * reset the accumulators to fit a new size of image
	 * caution: all value saved will be erased 
	 */
	if(channels != 1)
	{
		std::cout << "Error to reset with channels" << channels << std::endl;
		return;
	}
	if(width <= 0 || height <= 0)
	{
		std::cout << "Bad width or height" << width << " " << height << std::endl;
		return;
	}
	DVSsave.resize(width * height, 0);
	Lastgray.resize(width * height, 0);
	accumulator.resize(width * height, 0);
}

int DAVISSimulator::SimulateEventFromImage(cv::Mat& img, std::ofstream& outstream)
{
	if(channels == 1)
	{
		SimulateoneChannel(img, outstream);
		SimulateImageBlur(img);
		return 0;
	}
	else
	{
		std::cout << "Error to simulate image with channels " << channels << std::endl
					<< "Please reset the simulator." << std::endl;
		return 1;
	}
}

void DAVISSimulator::SimulateoneChannel(cv::Mat& img, std::ofstream& outstream)
{
	if (frames == 0)
	{
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
				uint8_t _gray = img.at<uchar>(y, x);
                Lastgray[x * height + y] = DVSsave[x * height + y] = Clamp(_gray, 0, 255);
			}
		}
		frames++;
		return;
	}

	std::vector<Eventrecord> DVSEvents;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{		
			int idx = x * height + y;   // Pixel index
			int value = Clamp(img.at<uchar>(y, x), 0, 255); // Image value at this pixel

			// DVS part
			int DVSpolar = 0;
			int total_change = value - Lastgray[idx];

            // If the input image value represents a contrast difference large enough that we need to fire an event...
			if (value - DVSthres >= DVSsave[idx] || value + DVSthres <= DVSsave[idx])
			{
				DVSpolar = value > DVSsave[idx] ? 1 : -1;
				if (total_change == 0)
				{
					int strange_events = (Lastgray[idx] - DVSsave[idx]) * DVSpolar / DVSthres;
					for (int i = 0; i < strange_events; ++i)
					{
						Eventrecord dev(i, y, frames, DVSpolar);
						DVSEvents.push_back(dev);
						DVSsave[idx] = Lastgray[idx];
					}
				}
				else
				{
					int current_event = DVSsave[idx] + DVSthres * DVSpolar;
					double start_time = (current_event - Lastgray[idx]) / (double)total_change;
					while(start_time >= 0 && start_time <= 1)
					{
						Eventrecord dev(x, y, frames + start_time, DVSpolar);
						DVSEvents.push_back(dev);
						DVSsave[idx] = current_event;
						start_time += DVSthres * DVSpolar / (double)total_change;
						current_event += DVSthres * DVSpolar;
					}
				}
			}
			Lastgray[idx] = value;
		}
	}
	frames++;
	for (const auto dev : DVSEvents)
	{
		outstream << dev.x << " " << dev.y << " " << dev.t << " " << dev.p << ", ";
		D_event++;
	}
}

void DAVISSimulator::SimulateImageBlur(cv::Mat& img)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{		
			int idx = x * height + y;
			int value = Clamp(img.at<uchar>(y, x), 0, 255);
			accumulator[idx] += value;
		}
	}
	if (frames % merge_image_frames == 0)
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{		
				int idx = x * height + y;
				img.at<uchar>(y, x) = accumulator[idx] / merge_image_frames;
				accumulator[idx] = 0;
			}
		}
		char buffer[256];
		snprintf(buffer, 256, file_fmt.c_str(), frames / merge_image_frames);
		cv::imwrite(buffer, img);
	}
}