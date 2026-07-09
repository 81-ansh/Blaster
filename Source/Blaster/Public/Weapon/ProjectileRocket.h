// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "ProjectileRocket.generated.h"

class URocketMovementComponent;

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileRocket : public AProjectile
{
	GENERATED_BODY()
	
public:
	
	AProjectileRocket();
	virtual void Destroyed() override;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
	void DestroyTimerFinished();
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> TrailSystem;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> TrailSystemComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ProjectileLoop;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> ProjectileLoopComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundAttenuation> LoopingSoundAttenuation;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<URocketMovementComponent> RocketMovementComponent;
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> RocketMesh;

	FTimerHandle DestroyTimer;
	
	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;
	
};