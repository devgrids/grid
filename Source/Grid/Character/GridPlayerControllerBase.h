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

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void AddControllerYawInput(float Value);
	void AddControllerPitchInput(float Value);

	void JumpBoost();
	void StopJumping();

	void FireWeapon();

	void InitializeDefaultPawnInputBindings();
	void AddInputBindings();
	UFUNCTION(BlueprintCallable)
	void RemoveInputBindings();

protected:
	ACharacter* Character{nullptr};

	bool CompareInputActionBindings(FInputActionBinding lhs, FInputActionBinding rhs);

	TArray<FInputAxisBinding> Axis;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;
};
