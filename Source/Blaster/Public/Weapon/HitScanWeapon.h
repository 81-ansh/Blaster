// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Weapon.h"
#include "HitScanWeapon.generated.h"

class UNiagaraSystem;

/**
 * 
 */
UCLASS()
class BLASTER_API AHitScanWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:
	
	virtual void Fire(const FVector& HitTarget) override;
	
private:
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	UParticleSystem* BeamParticles;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FireSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HitSound;

};
