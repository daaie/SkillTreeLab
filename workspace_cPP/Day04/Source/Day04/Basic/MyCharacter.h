// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class DAY04_API AMyCharacter : public ACharacter
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

	UFUNCTION()
		void TryIronsight();

	UFUNCTION()
		void TryCrouch();
	
	UFUNCTION()
		void MoveRight(float Value);

	UFUNCTION()
		void MoveForward(float Value);

	UFUNCTION()
		void Turn(float Value);

	UFUNCTION()
		void LookUp(float Value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UMySceneComponent* Weapon;
		//class UStaticMeshComponent* Weapon;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	class UArrowComponent* WeaponArrow;

	FRotator GetAimOffset() const; 
	// 다른데서 함수 내용을 바꾸면 컴파일안되
	// Const 반환값 함수명 -> 반환ㄱ밧을 바꾸면 에러난당
	// 매개변수 const-> 넘겨는 주는데 내용을바꾸면 에러난다.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float JogSpeed = 360.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float WalkSpeed = 150.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float SprintSpeed = 1500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ProneSpeed = 50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ProneSpringOffset = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float ProneEyeHeight;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float CurrentHP;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MaxHP = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAnimMontage* DeadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		FVector ProneSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		FVector NormalSpringArmPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
		FVector CrouchSpringArmPosition;



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsIronsight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsProne;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsProning;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsFire;


	UFUNCTION(BlueprintCallable)
		void Prone();

	UFUNCTION(BlueprintCallable)
		void Sprint();

	UFUNCTION(BlueprintCallable)
		void UnSprint();

	UFUNCTION(BlueprintCallable)
		void OnShot();

	UFUNCTION(BlueprintCallable)
		void LookAround();

	UFUNCTION(BlueprintCallable)
		void UndoLookAround();


	UFUNCTION(BlueprintCallable)
		void StartFire();

	UFUNCTION(BlueprintCallable)
		void StopFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* Explosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class UParticleSystem* AttackExplosion;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
		class USoundBase* ExplosionSound;

	FTimerHandle TimerHandle;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

};
