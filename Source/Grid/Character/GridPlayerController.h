// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridCharacter.h"
#include "GridPlayerControllerBase.h"
#include "GridPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRID_API AGridPlayerController final : public AGridPlayerControllerBase
{
	GENERATED_BODY()
public:
	AGridPlayerController();
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

	void InitInput();
	void StopInput();



	FInputActionBinding MyActionBinding;
	FInputActionBinding MyActionBinding2;


	bool bandera = true;

protected:
	AGridCharacter* GridCharacter{nullptr};
};
