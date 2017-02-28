// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DiceComponent.h"
#include "PlayerManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DICEGAME_API UPlayerManager : public UActorComponent
{
	GENERATED_BODY()

public:	

	// Get true if is the player turn
	UFUNCTION(BlueprintCallable, Category = "Dice")
	bool GetPlayerTurn() { return bPlayerTurn; }

	//Sets player turn
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void SetActiveTurn(bool Active) { bPlayerTurn = Active; }

	//Add a new Dice to the player
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void AddDice(UDiceComponent* NewDice);

	//Get Dice in Index
	UFUNCTION(BlueprintCallable, Category = "Dice")
	UDiceComponent* GetDice(int Index);

	//Roll all the player Dices
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void RollDices(FVector Direction = FVector::ZeroVector);

	//Return true if all the player Dices are stopped
	UFUNCTION(BlueprintCallable, Category = "Dice")
	bool AreDicesStop();

	// Sets default values for this component's properties
	UPlayerManager();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:

	bool bPlayerTurn = false;
	TArray<UDiceComponent*> DiceComponents;

};
