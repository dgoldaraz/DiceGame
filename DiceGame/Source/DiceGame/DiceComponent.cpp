// Fill out your copyright notice in the Description page of Project Settings.

#include "DiceGame.h"
#include "DiceComponent.h"
#include "DrawDebugHelpers.h"
#include "RenderCore.h"
#include "StaticMeshResources.h"
#include "EngineGlobals.h"
#include "Engine.h"



FColor ChooseColor(int i)
{
	return FColor(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255));
}

void UDiceComponent::DrawNormals()
{
	if (MeshComponent->StaticMesh->HasValidRenderData() && MeshComponent->StaticMesh->RenderData->LODResources.Num() > 0)
	{
		float NumVertex = MeshComponent->StaticMesh->RenderData->LODResources[0].GetNumVertices();
		FColor Color = FColor::Blue;
		FVector CurrentVector = FVector::ZeroVector;
		int j = 0;
		for (int i = 0; i < NumVertex; i++)
		{
			FVector ZVector = MeshComponent->StaticMesh->RenderData->LODResources[0].VertexBuffer.VertexTangentZ(i).GetSafeNormal();
			if (!ZVector.Equals(CurrentVector))
			{
				DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + ZVector * 100, ChooseColor(j), true);
				CurrentVector = ZVector;
				j++;
			}
		}
	}
}

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
	//Get Owner and MeshComponent
	Owner = GetOwner();
	if (Owner)
	{
		MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
		if (MeshComponent)
		{
			//Desactivate Physics until the dice it's rolled
			MeshComponent->SetSimulatePhysics(false);
			TArray<USceneComponent*> MeshChildren;
			MeshComponent->GetChildrenComponents(true, MeshChildren);

			for (auto Child : MeshChildren)
			{
				UPositionDice* CurrentPositionDice = dynamic_cast<UPositionDice*>(Child);
				if (CurrentPositionDice != nullptr)
				{
					DicePositionsArray.Add(CurrentPositionDice);
				}
			}
		}
		
	}

	InitialPosition = Owner->GetActorLocation();
	InitialRotator = Owner->GetActorRotation();
}


// Called every frame
void UDiceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsStopped())
	{
		if (!bDrawNormals)
		{
			DrawNormals();
			CurrentTopValue = GetTopValue();
			if (GEngine)
			{
				FString TextValue = TEXT("The top Face value is ");
				TextValue.Append(FString::FromInt(CurrentTopValue));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TextValue);
			}

			bDrawNormals = true;
		}
	}
	else
	{
		bDrawNormals = false;
	}
}

bool UDiceComponent::IsStopped()
{
	if (Owner && MeshComponent)
	{
		FVector AngularVelocity = MeshComponent->GetPhysicsAngularVelocity();
		return MeshComponent->GetPhysicsAngularVelocity().IsZero() && MeshComponent->GetPhysicsLinearVelocity().IsZero();
	}
	return false;
}

int UDiceComponent::GetCurrentValue()
{
	return CurrentTopValue;
}

int UDiceComponent::GetTopValue()
{
	float HigherPosition = -1000;
	int Value = -1;
	for (auto Position : DicePositionsArray)
	{
		if (Position != nullptr)
		{
			if (Position->GetComponentLocation().Z > HigherPosition)
			{
				HigherPosition = Position->GetComponentLocation().Z;
				Value = Position->ID;
			}
		}
	}
	return Value;
}

void UDiceComponent::RollDice(FVector UserDirection)
{
	//Calculate a new Thrown direction (using min max)
	//Add Random rotations to the dice
	if (Owner != nullptr)
	{
		FVector Direction = UserDirection;
		if (Direction.IsZero())
		{
			FVector NewOffset = FVector(FMath::RandRange(MinDirection.X, MaxDirection.X), FMath::RandRange(MinDirection.Y, MaxDirection.Y), FMath::RandRange(MinDirection.Z, MaxDirection.Z));
			Direction = Owner->GetActorForwardVector() + NewOffset;
		}
		//Add the new offset to the forward direction
		float Impulse = FMath::RandRange(5.0f, MaxImpulse);
		//Add impulse
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->AddImpulse(Direction * Impulse);
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



