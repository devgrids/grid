// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimalSystem.h"

// Sets default values
AAnimalSystem::AAnimalSystem()
{
	Animals.Add(TEXT("Acrocanthosaurus"), {});
}

// Called when the game starts or when spawned
void AAnimalSystem::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAnimalSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
