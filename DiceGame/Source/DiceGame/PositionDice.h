// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "PositionDice.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DICEGAME_API UPositionDice : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnyWhere, Category = "Dice")
	int ID = 0;

	// Sets default values for this component's properties
	UPositionDice();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
		
	
};
