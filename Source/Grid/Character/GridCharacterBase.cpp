// Fill out your copyright notice in the Description page of Project Settings.


#include "GridCharacterBase.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

AGridCharacterBase::AGridCharacterBase()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	CameraBoom->SetupAttachment(RootComponent);
	//CameraComponent->SetupAttachment(SpringArmComponent); 
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->TargetArmLength = 180.f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SocketOffset = FVector(0.f, 50.f, 70.f);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character

	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	GetMesh()->bEnableUpdateRateOptimizations = true;
	GetMesh()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	//GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -100.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	// Don't rotate when the controller rotates. Let the controller only affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f); // ... at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AGridCharacterBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (GetMesh())
	{
		GetMesh()->bRenderCustomDepth = true;
		GetMesh()->CustomDepthStencilValue = 2;
	}
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

	if (static const IConsoleVariable* ExtraForce = IConsoleManager::Get().FindConsoleVariable(
			TEXT("OurGame.ExtraMagnetism")); ExtraForce != nullptr &&
		ExtraForce->GetInt() != 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("CMD"));
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("NOT CMD") );
	}
}
