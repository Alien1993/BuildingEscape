// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Rotator.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Finds PhysicsHandleComponent attached to Owner
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandleComponent for %s not found"), *GetOwner()->GetName());
	}
}

/// Finds and setup attached InputComponent to Owner
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InputComponent for %s not found"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult HitResult;
	FCollisionQueryParams TraceParams{ FName{}, false, GetOwner() };
	bool bHasHit = GetWorld()->LineTraceSingleByObjectType(
		HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams{ ECollisionChannel::ECC_PhysicsBody },
		TraceParams
	);
	return HitResult;
}

void UGrabber::Grab()
{
	if (PhysicsHandle == nullptr) { return; }
	FHitResult HitRes = GetFirstPhysicsBodyInReach();

	if (HitRes.GetActor()) 
	{
		// Attach PhysicsHandle to HitComponent
		UPrimitiveComponent* ComponentToGrab = HitRes.GetComponent();
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle == nullptr) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle == nullptr) { return; }

	if (PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

FVector UGrabber::GetReachLineStart() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	return PlayerLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	return PlayerLocation + (PlayerRotation.Vector() * Reach);
}

