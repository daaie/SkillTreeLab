// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class PROJECTBD_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ItemGet();

	void Inventory();

	class AMasterItem* GetClosestItem();

	void TryIronsight();

	UFUNCTION()
	void TryCrouch();


	UFUNCTION()
	void Turn(float Value);

	UFUNCTION()
	void LookUp(float Value);

	UFUNCTION()
	void MoveRight(float Value);

	UFUNCTION()
	void MoveForward(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UWeaponComponent* Weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UPawnNoiseEmitterComponent* NoiseEmitter;

	UFUNCTION(BlueprintCallable, Category="Aim")
	FRotator GetAimOffset() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Info")
	bool bIsIronsight = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float WalkSpeed = 150.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float JogSpeed = 360.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float SprintSpeed = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float ProneSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	bool bIsSprint = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	bool bIsProne = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	bool bIsProning = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	bool bIsFire = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FVector NormalSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FVector CrouchSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FVector ProneSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CurrentHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UAnimMontage* DeadAnimation;


	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void UnSprint();

	UFUNCTION(BlueprintCallable)
	void LookAround();

	UFUNCTION(BlueprintCallable)
	void UndoLookAround();


	UFUNCTION(BlueprintCallable)
	void TryProne();

	UFUNCTION(BlueprintCallable)
	void StartFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void OnShot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class USoundBase* ExplosionSound;

	FTimerHandle ShootTimeHandle;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	TArray<class AMasterItem*> CanPickupList;

	UFUNCTION(BlueprintCallable)
	void AddPickupItemList(class AMasterItem* Item);

	UFUNCTION(BlueprintCallable)
	void RemovePickupItemList(class AMasterItem* Item);

	void ViewItemTooltip();
};
