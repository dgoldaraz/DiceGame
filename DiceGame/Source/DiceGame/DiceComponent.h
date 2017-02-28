// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "PositionDice.h"
#include "DiceComponent.generated.h"

/*
	This component Stores the functionality relevant for simulating a Dice. You can throw it and know the value when it's stopped
*/

static FAutoConsoleVariable DiceDebug = FAutoConsoleVariable(TEXT("d.Dice"), 0,TEXT("Enables Dice Debug View"), ECVF_Cheat);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DICEGAME_API UDiceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//This function will start to roll the dice in a semi random direction
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void RollDice(FVector Direction);

	//Reset Position to Initial
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void ResetPosition();

	//This function return true if the actor it's not moving by physic objects
	UFUNCTION(BlueprintCallable, Category = "Dice")
	bool IsStopped();

	//Get Top Value
	UFUNCTION(BlueprintCallable, Category = "Dice")
	int GetCurrentValue();

	//Get Initial Position, before roll the dices
	UFUNCTION(BlueprintCallable, Category = "Dice")
		const FVector GetInitialPosition();

	//Set the min and max direction between the dice throw direction will be calculated
	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MinDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MaxDirection = FVector::ZeroVector;

	//Max impulse that can be applied to the Dice
	UPROPERTY(EditAnywhere, Category = "Dice")
	float MaxImpulse = 500.0f;

	//Rotation Limits
	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MinRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MaxRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	TArray<UPositionDice*> DicePositionsArray;


	// Sets default values for this component's properties
	UDiceComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	//This function will go through all the DicePositions and choose the one with the higer location, and return it's value.
	int GetTopValue();

	//Draw the normals of the element in different colours
	void DrawNormals();

	//Center the cursor
	void CenterCursor();

	AActor* Owner = nullptr;
	UStaticMeshComponent* MeshComponent = nullptr;
	FRotator InitialRotator = FRotator::ZeroRotator;
	FVector InitialPosition = FVector::ZeroVector;
	int CurrentTopValue = 0;
	bool Stopped = true;
	bool AskForMoving = false;
};
