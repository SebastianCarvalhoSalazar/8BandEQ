/*
  ==============================================================================

    Filtradora.h
    Created: 28 Sep 2020 1:33:00pm
    Author:  SEBASTIAN

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Filtering
{
public:
	Filtering()
	   : sampleRate(44100), 
		 lpCutOffFrequency(20), hpCutOffFrequency(120), bpCutOffFrequency(20), nCutOffFrequency(1000),
		 Q(0.7), a0 (0), a1(0), a2(0), b1(0), b2(0)
	{

	}

	~Filtering(){}

	void setSampleRate(float newSampleRate) 
	{
		sampleRate = newSampleRate;
	}

	void setCutOffFrequencylowPass(float newCutOffFrequencyLowPass)
	{
		lpCutOffFrequency = newCutOffFrequencyLowPass;
	}

	void setCutOffFrequencyHighPass(float newCutOffFrequencyHighPass)
	{
		hpCutOffFrequency = newCutOffFrequencyHighPass;
	}

	void setCutOffFrequencyBandPass(float newCutOffFrequencyBandPass)
	{
		bpCutOffFrequency = newCutOffFrequencyBandPass;
	}

	void setCutOffFrequencyNotch(float newCutOffFrequencyNotch)
	{
		nCutOffFrequency = newCutOffFrequencyNotch;
	}

	void setQ1BandPass(float newQBandPass1)
	{
		Q = newQBandPass1;
	}

	void setQLowPass(float newQLowPass)
	{
		Q = newQLowPass;
	}

	void setQHighPass(float newQhighPass)
	{
		Q = newQhighPass;
	}

	void setQNotch (float newQNotch)
	{
		Q = newQNotch;
	}

	float lpFilter(float xn)
	{
		if (isLowPass) 
		{
			// Low Pass Filter
			float thetaC = (juce::MathConstants<float>::twoPi * lpCutOffFrequency) / (sampleRate);
			float d = 1 / Q;
			float beta = 0.5 * ((1 - (d / 2) * sin(thetaC)) / (1 + (d / 2) * sin(thetaC)));
			float gamma = (0.5 + beta) * cos(thetaC);

			a0 = (0.5 + beta - gamma) / 2.0;
			a1 = (0.5 + beta - gamma);
			a2 = (0.5 + beta - gamma) / 2.0;

			b1 = -2 * gamma;
			b2 = 2 * beta;

			float yn = (a0 * xn) + (a1 * xr[0]) + (a2 * xr[1]) - (b1 * yr[0]) - (b2 * yr[1]);

			xr[1] = xr[0];
			xr[0] = xn;

			yr[1] = yr[0];
			yr[0] = yn;

			return yn;
		}
		else
		{
			return xn;
		}
	}

	float hpFilter(float xn)
	{
		if (isHighPass)
		{
			// High Pass Filter
			float thetaC = (juce::MathConstants<float>::twoPi * hpCutOffFrequency) / (sampleRate);
			float d = 1 / Q;
			float beta = 0.5 * ((1 - (d / 2) * sin(thetaC)) / (1 + (d / 2) * sin(thetaC)));
			float gamma = (0.5 + beta) * cos(thetaC);

			a0 = (0.5 + beta + gamma) / 2.0;
			a1 = -(0.5 + beta + gamma);
			a2 = (0.5 + beta + gamma) / 2.0;

			b1 = -2 * gamma;
			b2 = 2 * beta;

			float yn = (a0 * xn) + (a1 * xr[0]) + (a2 * xr[1]) - (b1 * yr[0]) - (b2 * yr[1]);

			xr[1] = xr[0];
			xr[0] = xn;

			yr[1] = yr[0];
			yr[0] = yn;

			return yn;
		}
		else
		{
			return xn;
		}
	}


	float bpFilter(float xn)
	{
		if (isBandPass)
		{
			// Band Pass Filter 1
			float K = tan((juce::MathConstants<float>::pi * bpCutOffFrequency) / (sampleRate));
			float alpha = pow(K, 2) * Q + K + Q;

			a0 = K / alpha;
			a1 = 0.0;
			a2 = -K / alpha;

			b1 = (2 * Q * (pow(K, 2) - 1)) / alpha;
			b2 = (pow(K, 2) * Q - K + Q) / alpha;

			float yn = (a0 * xn) + (a1 * xr[0]) + (a2 * xr[1]) - (b1 * yr[0]) - (b2 * yr[1]);

			xr[1] = xr[0];
			xr[0] = xn;

			yr[1] = yr[0];
			yr[0] = yn;

			return yn;
		}
		else
		{
			return xn;
		}
	}


	float nFilter(float xn)
	{
		if (isNotchFilter)
		{
			// Notch Filter
			float K = tan((juce::MathConstants<float>::pi * nCutOffFrequency) / (sampleRate));
			float alpha = pow(K, 2) * Q + K + Q;

			a0 = (Q * (pow(K, 2) + 1)) / alpha;
			a1 = (2 * Q * (pow(K, 2) - 1)) / alpha;
			a2 = (Q * (pow(K, 2) + 1)) / alpha;

			b1 = (2 * Q * (pow(K, 2) - 1)) / alpha;
			b2 = (pow(K, 2) * Q - K + Q) / alpha;

			float yn = (a0 * xn) + (a1 * xr[0]) + (a2 * xr[1]) - (b1 * yr[0]) - (b2 * yr[1]);

			xr[1] = xr[0];
			xr[0] = xn;

			yr[1] = yr[0];
			yr[0] = yn;

			return yn;
		}
		else
		{
			return xn;
		}
	}

	std::atomic<bool> isLowPass, isHighPass, isBandPass, isNotchFilter;
private:
	std::atomic<float> a0, a1, a2, b1, b2, Q;
	float xr[2] = { 0, 0 };
	float yr[2] = { 0, 0 };
	float sampleRate;
	float lpCutOffFrequency, hpCutOffFrequency, bpCutOffFrequency, nCutOffFrequency;

};