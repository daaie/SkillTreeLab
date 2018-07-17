// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_ZombieMove.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTBD_API UAnimNotifyState_ZombieMove : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:

	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

	virtual void Received_NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) ;

	virtual void Received_NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) ;
	
};
