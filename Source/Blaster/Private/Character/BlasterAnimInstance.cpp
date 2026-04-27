// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BlasterAnimInstance.h"

#include "Character/BlasterCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBlasterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
}

void UBlasterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (BlasterCharacter == nullptr)
	{
		BlasterCharacter = Cast<ABlasterCharacter>(TryGetPawnOwner());
	}
	if (BlasterCharacter == nullptr) return;
	
	FVector Velocity = BlasterCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	
	bIsInAir = BlasterCharacter->GetCharacterMovement()->IsFalling();
	bIsAccelerating = BlasterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
	bWeaponEquipped = BlasterCharacter->IsWeaponEquipped();
	bIsCrouched = BlasterCharacter->IsCrouched();
	bAiming = BlasterCharacter->IsAiming();
	
	// Offset Yaw for Strafing
	FRotator AimRotation = BlasterCharacter->GetBaseAimRotation();																// get the direction the character is looking
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(BlasterCharacter->GetVelocity());								// convert velocity direction to a rotation
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;							// get the angle difference between movement and look direction (-180 to 180)
	
	CharacterRotationLastFrame = CharacterRotation;																				// store last frame's rotation
	CharacterRotation = BlasterCharacter->GetActorRotation();																	// get current frame's rotation
	const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame);		// get the difference between current and last frame rotation
	const float Target = Delta.Yaw / DeltaSeconds;																				// convert rotation difference to a per-second rate
	const float Interp = FMath::FInterpTo(Lean, Target, DeltaSeconds, 6.f);							// smoothly interpolate towards the target lean value
	Lean = FMath::Clamp(Interp, -90.f, 90.f);																		// clamp lean between -90 and 90 degrees
}
