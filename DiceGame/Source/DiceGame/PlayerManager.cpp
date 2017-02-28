// Fill out your copyright notice in the Description page of Project Settings.

#include "DiceGame.h"
#include "PlayerManager.h"


// Sets default values for this component's properties
UPlayerManager::UPlayerManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerManager::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UPlayerManager::AddDice(UDiceComponent * NewDice)
{
	DiceComponents.Add(NewDice);
}

UDiceComponent* UPlayerManager::GetDice(int Index)
{
	if (Index < DiceComponents.Num())
	{
		return DiceComponents[Index];
	}
	return nullptr;
}

void UPlayerManager::RollDices(FVector Direction)
{
	for (auto Dice : DiceComponents)
	{
		Dice->RollDice(Direction);
	}
}

bool UPlayerManager::AreDicesStop()
{

	for (auto Dice : DiceComponents)
	{
		if (!Dice->IsStopped())
		{
			return false;
		}
	}
	return true;
}
