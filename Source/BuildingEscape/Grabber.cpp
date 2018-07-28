// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerLocation;
	FRotator PlayerRotation;
	PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);

//	UE_LOG(LogTemp, Warning, TEXT("Player position and rotation is %s and %s"),
//		*Location.ToString(), *Rotation.ToString());

	FVector LineTraceEnd = PlayerLocation + (PlayerRotation.Vector() * Reach);

	DrawDebugLine(
		GetWorld(),
		PlayerLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		.0f,
		.0f,
		10.0f
	);
}

