// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPlayerController.h"
#include "GameFramework/PlayerInput.h"

AGridPlayerController::AGridPlayerController()
{
	GridCharacter = Cast<AGridCharacter>(GetCharacter());
}

void AGridPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// GridCharacter = Cast<AGridCharacter>(Character);

	if (IsValid(GridCharacter))
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid character"));
	}
}

void AGridPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AGridPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();


	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Jump/Boost", EKeys::SpaceBar));

	MyActionBinding = FInputActionBinding("Jump/Boost", IE_Pressed);
	MyActionBinding.ActionDelegate.BindDelegate(this, &AGridPlayerController::InitInput);

	MyActionBinding2 = FInputActionBinding("Jump/Boost", IE_Released);
	MyActionBinding2.ActionDelegate.BindDelegate(this, &AGridPlayerController::StopInput);


	InputComponent->AddActionBinding(MyActionBinding);
	InputComponent->AddActionBinding(MyActionBinding2);
}

void AGridPlayerController::InitInput()
{
	int intVar = 5;
	float floatVar = 3.7f;
	FString fstringVar = "an fstring variable";
	UE_LOG(LogTemp, Warning, TEXT("Text, %d %f %s"), intVar, floatVar, *fstringVar);
}

void AGridPlayerController::StopInput()
{
	UE_LOG(LogTemp, Warning, TEXT("FRES"));

	// for (int i = 0; i < InputComponent->GetNumActionBindings(); i++)
	// {
	// 	FInputActionBinding binding = InputComponent->GetActionBinding(i);
	// 	if (CompareInputActionBindings(binding, MyActionBinding2))
	// 	{
	// 		InputComponent->RemoveActionBinding(i);
	// 		i--;
	// 		continue;
	// 	}
	// }


	if (bandera)
		RemoveInputBindings();
	else
		AddInputBindings();
	bandera = !bandera;
}
