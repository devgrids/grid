// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Animal.generated.h"

UCLASS()
class GRID_API AAnimal : public AActor
{
	GENERATED_BODY()

public:
	AAnimal();
	virtual void OnConstruction(const FTransform& Transform) override;
	// USceneComponent* Root;
	UGridStaticMeshComponent* GridMesh;
	// USphereComponent* ShapeComponent;
	// UShapeComponent* ShapeComponent;

	UPROPERTY(EditAnywhere, Category=Animal)
	UStaticMesh* AnimalMesh;


protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
