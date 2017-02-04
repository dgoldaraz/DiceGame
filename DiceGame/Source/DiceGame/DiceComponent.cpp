// Fill out your copyright notice in the Description page of Project Settings.

#include "DiceGame.h"
#include "DiceComponent.h"
#include "DrawDebugHelpers.h"
#include "RenderCore.h"

/*
This class encapsulates the behavior of the any Dice
It will allow the user to throw the dice, know when it's stoped and return the value of the face up on the dice
*/
// Sets default values for this component's properties
UDiceComponent::UDiceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDiceComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	//Get Owner and MeshComponent

	if (Owner)
	{
		MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComponent)
		{
			//Desactivate Physics until the dice it's rolled
			MeshComponent->SetSimulatePhysics(false);
		}
	}
	InitialPosition = Owner->GetActorLocation();
	InitialRotator = Owner->GetActorRotation();
}


// Called every frame
void UDiceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Owner->GetActorForwardVector() * 100.0f, FColor::Green, false, 1.0f, 0, 5);
}

void UDiceComponent::RollDice()
{
	//Calculate a new Thrown direction (using min max)
	//Add Random rotations to the dice
	if (Owner != nullptr)
	{
		FVector NewOffset = FVector(FMath::RandRange(MinDirection.X, MaxDirection.X), FMath::RandRange(MinDirection.Y, MaxDirection.Y), FMath::RandRange(MinDirection.Z, MaxDirection.Z));
		//Add the new offset to the forward direction
		FVector NewDirection = Owner->GetActorForwardVector() + NewOffset;
		float Impulse = FMath::RandRange(200.0f, MaxImpulse);
		DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + NewDirection * Impulse, FColor::Red, true, 10.0f, 0, 5);
		//Add impulse
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->AddImpulse(NewDirection * Impulse);
		FVector TorqueImpulse = FVector(FMath::RandRange(MinRotation.X, MaxRotation.X), FMath::RandRange(MinRotation.Y, MaxRotation.Y), FMath::RandRange(MinRotation.Z, MaxRotation.Z));
		MeshComponent->AddAngularImpulse(TorqueImpulse * Impulse);
	}
}

void UDiceComponent::ResetPosition()
{
	Owner->SetActorLocation(InitialPosition);
	MeshComponent->SetSimulatePhysics(false);
	Owner->SetActorRotation(InitialRotator);
}

