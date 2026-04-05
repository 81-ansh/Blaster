// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "GameFramework/Character.h"
#include "Weapon/Weapon.h"
#include "BlasterCharacter.generated.h"

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* EquipAction;
	
	void SetOverlappingWeapon(AWeapon* Weapon);

protected:
	
	virtual void BeginPlay() override;
	
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void EquipButtonPressed();

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	class UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
};
