// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P38Rocket.generated.h"

UCLASS()
class CPPEXAM_API AP38Rocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AP38Rocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnBeginOverLap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UProjectileMovementComponent* Movement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class USoundBase* ExplosionSound;
	
};
