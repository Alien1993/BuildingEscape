// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate"), *Owner->GetName());
	}
}

void UOpenDoor::OpenDoor()
{
	if (Owner)
	{
		Owner->SetActorRotation(FRotator{ .0f, OpenAngle, .0f });
	}
}

void UOpenDoor::CloseDoor()
{
	if (Owner)
	{
		Owner->SetActorRotation(FRotator{ .0f, .0f, .0f });
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() const
{
	float TotalMass = .0f;

	TArray<AActor*> OverlappingActors;
	if (PressurePlate != nullptr)
	{
		PressurePlate->GetOverlappingActors(OverlappingActors);
	}

	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastOpenDoorTime = GetWorld()->GetTimeSeconds();
	}

	float Delta = GetWorld()->GetTimeSeconds() - LastOpenDoorTime;
	if (Delta > CloseDelay)
	{
		CloseDoor();
	}
}

