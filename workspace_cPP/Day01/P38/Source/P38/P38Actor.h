// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P38Actor.generated.h"
//인클루드는 제너레이터 위에만해야함. 추가를권장안함.

UCLASS()
class P38_API AP38Actor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties.
	AP38Actor();

protected:
	// Called when the game starts or when spawned.
	virtual void BeginPlay() override;

public:	
	// Called every frame.
	virtual void Tick(float DeltaTime) override;

	// 블루프린트에서 함수 호출하려면 아래 줄 입력.
	// 씨에서 블루프린트 함수 호출.

	UFUNCTION(BlueprintCallable, Category="Rotate")
	void RotatePropeller(UStaticMeshComponent * Mesh);


	// UPROPERTY(EditAnywhere, BlueprintReadWrite) //모든걸다바꿀수있음.
	// 엑터아닌 클래스는 무조건 U붙여야함.
	// UPROPERTY 에 Category 추가할수도있다.

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* Left;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* Right;
	
};
