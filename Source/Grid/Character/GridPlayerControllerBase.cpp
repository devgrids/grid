// Fill out your copyright notice in the Description page of Project Settings.

#include "GridPlayerControllerBase.h"

#include "GridCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

AGridPlayerControllerBase::AGridPlayerControllerBase()
{
	InitializeDefaultPawnInputBindings();
}

void AGridPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();
	Character = GetCharacter();
	BaseTurnRate = BaseLookUpRate = 50.f;
}

void AGridPlayerControllerBase::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AGridPlayerControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
	AddInputBindings();
}

void AGridPlayerControllerBase::MoveForward(const float Value)
{
	if ((Character != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation{Character->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X)};
		Character->AddMovementInput(Direction, Value);
	}
}

void AGridPlayerControllerBase::MoveRight(const float Value)
{
	if ((Character != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation{Character->GetControlRotation()};
		const FRotator YawRotation{0, Rotation.Yaw, 0};

		const FVector Direction{FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y)};
		Character->AddMovementInput(Direction, Value);
	}
}

void AGridPlayerControllerBase::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	Character->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame
}

void AGridPlayerControllerBase::LookUpAtRate(const float Rate)
{
	Character->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); // deg/sec * sec/frame
}

void AGridPlayerControllerBase::AddControllerYawInput(const float Value)
{
	Character->AddControllerYawInput(Value);
}

void AGridPlayerControllerBase::AddControllerPitchInput(const float Value)
{
	Character->AddControllerPitchInput(Value);
}

void AGridPlayerControllerBase::JumpBoost()
{
	if (!Character->GetCharacterMovement()->IsMovingOnGround())
	{
	}
	else
	{
		Character->Jump();
	}
}

void AGridPlayerControllerBase::StopJumping()
{
	Character->StopJumping();
}

void AGridPlayerControllerBase::FireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("FireButton"));
}

void AGridPlayerControllerBase::InitializeDefaultPawnInputBindings()
{
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveForward", EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::D, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("MoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Left, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("TurnRate", EKeys::Right, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("Turn", EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUp", EKeys::MouseY, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("LookUpRate", EKeys::Gamepad_RightThumbstick, 1.f));

	Axis.Add(FInputAxisBinding("MoveForward"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::MoveForward);
	Axis.Add(FInputAxisBinding("MoveRight"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::MoveRight);
	Axis.Add(FInputAxisBinding("TurnRate"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::TurnAtRate);
	Axis.Add(FInputAxisBinding("Turn"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::AddControllerYawInput);
	Axis.Add(FInputAxisBinding("LookUp"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::AddControllerPitchInput);
	Axis.Add(FInputAxisBinding("LookUpRate"));
	Axis.Last().AxisDelegate.BindDelegate(this, &AGridPlayerControllerBase::LookUpAtRate);
}

void AGridPlayerControllerBase::AddInputBindings()
{
	for (int i = 0; i < Axis.Num(); i++)
	{
		InputComponent->AxisBindings.Add(Axis[i]);
	}
}

void AGridPlayerControllerBase::RemoveInputBindings()
{
	// InputComponent->ClearBindingValues();
	InputComponent->AxisBindings.Empty();
	// InputComponent->ClearActionBindings();
	// InputComponent->AxisBindings.RemoveAt(4);
	// for (int i = 0; i < Axis.Num(); i++)
	// {
	// 	InputComponent->AxisBindings.RemoveAt(i);
	// }
}

bool AGridPlayerControllerBase::CompareInputActionBindings(FInputActionBinding lhs, FInputActionBinding rhs)
{
	return lhs.ActionDelegate.GetDelegateForManualSet().GetHandle() == rhs.ActionDelegate.GetDelegateForManualSet().
	                                                                       GetHandle() &&
		lhs.GetActionName() == rhs.GetActionName();
}
