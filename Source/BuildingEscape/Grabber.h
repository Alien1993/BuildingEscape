// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InputComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"	
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	float Reach = 100.0f;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	void Grab();
	void Release();
	
	/// Finds PhysicsHandleComponent attached to Owner
	void FindPhysicsHandleComponent();

	/// Finds and setup attached InputComponent to Owner
	void SetupInputComponent();

	/// Returns FHitResult with first PhysicsBody in reach
	FHitResult GetFirstPhysicsBodyInReach() const;

	/// Returns current start of reach line
	FVector GetReachLineStart() const;

	/// Returns current end of reach line
	FVector GetReachLineEnd() const;
};
