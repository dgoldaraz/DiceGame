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
				DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + ZVector * 100, ChooseColor(j), false, 1.0f, 0, 5.0f);
				CurrentVector = ZVector;
				j++;
			}
		}
	}
}

void UDiceComponent::CenterCursor()
{
	//Show Mouse
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		const ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (LocalPlayer && LocalPlayer->ViewportClient)
		{
			FViewport* Viewport = LocalPlayer->ViewportClient->Viewport;
			if (Viewport)
			{
				FVector2D ViewportSize;
				LocalPlayer->ViewportClient->GetViewportSize(ViewportSize);
				const int32 X = static_cast<int32>(ViewportSize.X * 0.5f);
				const int32 Y = static_cast<int32>(ViewportSize.Y * 0.5f);

				Viewport->SetMouse(X, Y);
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
		CenterCursor();
		//Stop Movement
		GetWorld()->GetFirstPlayerController()->SetIgnoreMoveInput(true);
		GetWorld()->GetFirstPlayerController()->SetIgnoreLookInput(true);
	}
	InitialPosition = Owner->GetActorLocation();
	InitialRotator = Owner->GetActorRotation();
}


// Called every frame
void UDiceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	if (DiceDebug->GetInt() != 0)
	{
		DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + Owner->GetActorForwardVector() * 1000.0f, FColor::Green, false, 1.0f, 0, 5.0f);

		if (DiceDebug->GetInt() == 2)
		{
			DrawNormals();
		}
		if (DiceDebug->GetInt() == 3)
		{
			if (GEngine)
			{
				FString TextValue = TEXT("The top Face value is ");
				TextValue.Append(FString::FromInt(CurrentTopValue));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TextValue);
			}
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsStopped())
	{
		CurrentTopValue = GetTopValue();
		Stopped = true;
		if (!AskForMoving)
		{
			MeshComponent->SetSimulatePhysics(false);
		}
	}
}

bool UDiceComponent::IsStopped()
{

	if (Owner && MeshComponent && (!Stopped || AskForMoving))
	{
		FVector AngularVelocity = MeshComponent->GetPhysicsAngularVelocity();
		bool IsNotMoving = MeshComponent->GetPhysicsAngularVelocity().IsZero() && MeshComponent->GetPhysicsLinearVelocity().IsZero();
		if (!IsNotMoving && AskForMoving)
		{
			AskForMoving = false;
			Stopped = false;
		}
		return IsNotMoving;
	}
	return Stopped;
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
	//A Dice can only be thrown if it's stopped
	if (Owner != nullptr && Stopped)
	{
		FVector Direction = UserDirection;
		float Length = Direction.Size();
		if (Direction.IsZero())
		{
			FVector NewOffset = FVector(FMath::RandRange(MinDirection.X, MaxDirection.X), FMath::RandRange(MinDirection.Y, MaxDirection.Y), FMath::RandRange(MinDirection.Z, MaxDirection.Z));
			Direction = Owner->GetActorForwardVector() + NewOffset;
		}
		else
		{
			Direction.Z = FMath::RandRange(MinDirection.Z, MaxDirection.Z);
		}
		//Add the new offset to the forward direction
		float Impulse = FMath::RandRange(MaxImpulse*0.5f, MaxImpulse);
		Impulse *= Length;
		//Add impulse
		MeshComponent->SetSimulatePhysics(true);
		MeshComponent->AddImpulse(Direction * Impulse);

		if (DiceDebug->GetInt() == 1)
		{
			DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Direction*Impulse), FColor::Red, true);
		}

		FVector TorqueImpulse = FVector(FMath::RandRange(MinRotation.X, MaxRotation.X), FMath::RandRange(MinRotation.Y, MaxRotation.Y), FMath::RandRange(MinRotation.Z, MaxRotation.Z));
		MeshComponent->AddAngularImpulse(TorqueImpulse * Impulse);
		AskForMoving = true;
	}
}

void UDiceComponent::ResetPosition()
{
	Owner->SetActorLocation(InitialPosition);
	MeshComponent->SetPhysicsAngularVelocity(FVector::ZeroVector);
	MeshComponent->SetPhysicsLinearVelocity(FVector::ZeroVector);
	MeshComponent->SetSimulatePhysics(false);
	Owner->SetActorRotation(InitialRotator);
}



