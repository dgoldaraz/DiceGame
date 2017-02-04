// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "DiceComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DICEGAME_API UDiceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Dice")
	void RollDice();

	//Reset Position to Initial
	UFUNCTION(BlueprintCallable, Category = "Dice")
	void ResetPosition();

	//Set the min and max direction between the dice throw direction will be calculated
	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MinDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MaxDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	float MaxImpulse = 500.0f;


	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MinRotation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Dice")
	FVector MaxRotation = FVector::ZeroVector;


	// Sets default values for this component's properties
	UDiceComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	AActor* Owner = nullptr;
	UStaticMeshComponent* MeshComponent = nullptr;
	FVector InitialPosition = FVector::ZeroVector;
	FRotator InitialRotator = FRotator::ZeroRotator;
};
