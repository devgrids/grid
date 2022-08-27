// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GridCharacterBase.generated.h"

UCLASS()
class GRID_API AGridCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	AGridCharacterBase();
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	//*****************************************************************************
	// Interface
	//*****************************************************************************

	//*****************************************************************************
	// AI
	//*****************************************************************************

	//*****************************************************************************

	//*****************************************************************************
	// Components
	//*****************************************************************************

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class USpringArmComponent* CameraBoom{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* FollowCamera{nullptr};

	//*****************************************************************************


	//*****************************************************************************

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FText CharacterName;

private:
	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return FollowCamera; }
};
