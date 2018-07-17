// Fill out your copyright notice in the Description page of Project Settings.

#include "P38Rocket.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"


// Sets default values
AP38Rocket::AP38Rocket()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	Body->AddLocalRotation(FRotator(-90.0f, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body
	(TEXT("StaticMesh'/Game/Plane/P38Data/Meshes/SM_Rocket.SM_Rocket'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->MaxSpeed = 5000.0f;
	Movement->InitialSpeed = 3000.0f;
	Movement->ProjectileGravityScale = 0.0f;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_Explosion(
		TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	if (P_Explosion.Succeeded())
	{
		Explosion = P_Explosion.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> S_ExplosionSound(
		TEXT("SoundWave'/Game/StarterContent/Audio/Explosion01.Explosion01'"));
	if (S_ExplosionSound.Succeeded())
	{
		ExplosionSound = S_ExplosionSound.Object;
	}
}

// Called when the game starts or when spawned
void AP38Rocket::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3.0f);
	Box->OnComponentBeginOverlap.AddDynamic(this, &AP38Rocket::OnBeginOverLap);
}

// Called every frame
void AP38Rocket::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AP38Rocket::OnBeginOverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (Cast<APawn>(OtherActor) == nullptr)
	{

		UE_LOG(LogClass, Warning, TEXT("충돌"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorTransform(), true);
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorTransform().GetLocation());

		Destroy();
	}
}
