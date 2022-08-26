// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCharacterBase.h"

// Sets default values
AGridCharacterBase::AGridCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGridCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

