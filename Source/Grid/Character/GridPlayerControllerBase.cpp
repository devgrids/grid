// Fill out your copyright notice in the Description page of Project Settings.

#include <imgui.h>
#include "GridPlayerControllerBase.h"

AGridPlayerControllerBase::AGridPlayerControllerBase()
{
}

void AGridPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
}

void AGridPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	InputComponent->BindAction("FireWeapon", IE_Pressed, this, &AGridPlayerControllerBase::FireWeapon);
	InputComponent->BindAction("StopFireWeapon", IE_Pressed, this, &AGridPlayerControllerBase::StopFireWeapon);
}

void AGridPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AGridPlayerControllerBase::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Some warning message") );

	ImGui::SetMouseCursor(true);
}

void AGridPlayerControllerBase::StopFireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Some warning message") );

	ImGui::SetMouseCursor(false);
}
