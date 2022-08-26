// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GridPlayerControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class GRID_API AGridPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()

public:

	AGridPlayerControllerBase();
	
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	
	void FireWeapon();
	void StopFireWeapon();
	
};
