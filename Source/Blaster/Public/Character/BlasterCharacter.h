// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NiagaraComponent.h"
#include "BlasterComponents/CombatComponent.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractWithCrosshairInterface.h"
#include "BlasterCharacter.generated.h"

class AWeapon;
class UAnimMontage;
class ABlasterPlayerState;
class ABlasterPlayerController;
class USpringArmComponent;
class UWidgetComponent;
class UBuffComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairInterface
{
	GENERATED_BODY()

public:
	
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void CancelReloadMontage();
	void PlayThrowGrenadeMontage();
	virtual void OnRep_ReplicatedMovement() override;
	void Elim();
	virtual void Destroyed() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();
	
	void SpawnDefaultWeapon();
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GrenadeThrowAction;
	
	UPROPERTY()
	ABlasterPlayerState* BlasterPlayerState;
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }

protected:
	
	virtual void BeginPlay() override;
	
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void PlayHitReactMontage();
	void PlayElimMontage();
	
	// Poll for any relevant class and initialize HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);
	
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* Combat;
	
	UPROPERTY(VisibleAnywhere)
	UBuffComponent* Buff;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
	/*
	 * Animation Montage
	 */
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ElimMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ThrowGrenadeMontage;

	void HideCharacterIfCameraClose();
	
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();
	
	/*
	 * Player Health
	 */
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Health, Category = "Player Stats")
	float Health = 100.f;
	
	UFUNCTION()
	void OnRep_Health(float LastHealth);
	
	/*
	 * Player Shield
	 */
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Shield, Category = "Player Stats")
	float Shield = 0.f;
	
	UFUNCTION()
	void OnRep_Shield(float LastShield);
	
	UPROPERTY()
	ABlasterPlayerController* BlasterPlayerController;
	
	bool bElimmed = false;
	
	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	
	void ElimTimerFinished();
	
	/*
	 * Dissolve Effect
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	
	FOnTimelineFloat DissolveTrack;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DissolveCurve;
	
	// Dynamic Instance that can be changed at runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	
	// Material Instance set on the Blueprint
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	
	void StartDissolve();
	
	/*
	 * Elim Bot
	 */
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ElimBotEffect;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ElimBotComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ElimBotSound;
	
	/*
	 * Grenade
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AttachedGrenade;
	
	/*
	 * Default Weapon
	 */
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
};
