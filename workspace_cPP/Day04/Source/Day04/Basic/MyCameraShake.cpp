// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"


UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.5f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.05f;

	RotOscillation.Pitch.Amplitude = 5.0f;
	RotOscillation.Pitch.Frequency = 1.0f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}

