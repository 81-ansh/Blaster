// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ABlasterCharacter::ABlasterCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABlasterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlasterCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	}

}

void ABlasterCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (InputMapping)
			{
				Subsystem->AddMappingContext(InputMapping, 0);
			}
		}
	}
}

void ABlasterCharacter::Move(const FInputActionInstance& Instance)
{
	FVector2D MovementDirection = Instance.GetValue().Get<FVector2D>();
	const FRotator Rotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector ForwardDirection(FRotationMatrix(Rotation).GetUnitAxis(EAxis::X));
	const FVector RightDirection(FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y));
	AddMovementInput(RightDirection, MovementDirection.X);
	AddMovementInput(ForwardDirection, MovementDirection.Y);
	
}

void ABlasterCharacter::Look(const FInputActionInstance& Instance)
{
	FVector2D LookDirection = Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}