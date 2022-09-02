// Fill out your copyright notice in the Description page of Project Settings.


#include "Animal.h"

#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AAnimal::AAnimal()
{
	// Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// ShapeComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ShapeComponent"));

	GridMesh = CreateDefaultSubobject<UGridStaticMeshComponent>(TEXT("GridMesh"));
	// GridMesh->AttachToComponent(Root, FAttachmentTransformRules::SnapToTargetIncludingScale);

	// ShapeComponent->SetupAttachment(RootComponent);
	GridMesh->SetupAttachment(RootComponent);

	// ShapeComponent->SetSphereRadius(20);
}

void AAnimal::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GridMesh->SetStaticMesh(AnimalMesh);
}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
