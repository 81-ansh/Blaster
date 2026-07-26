// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraComponent.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class BLASTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AProjectile();

	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;
	
	/*
	 * Used with server side rewind
	 */
	
	bool bUseServerSideRewind = false;
	FVector_NetQuantize TraceStart;
	FVector_NetQuantize100 InitialVelocity;
	
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000;
	
	UPROPERTY(EditAnywhere)
	float HeadShotDamage = 40.f;
	
	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
	
protected:
	
	virtual void BeginPlay() override;
	
	void StartDestroyTimer();
	void DestroyTimerFinished();
	void SpawnTrailSystem();
	void ExplodeDamage();
	
	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;
	
	UPROPERTY(EditAnywhere)
	class UBoxComponent* CollisionBox;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere)
	class USoundBase* ImpactSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> TrailSystem;
	
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> TrailSystemComponent;
	
	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;
	
	UPROPERTY(EditAnywhere)
	float DamageInnerRadius = 200.f;
	
	UPROPERTY(EditAnywhere)
	float DamageOuterRadius = 500.f;
	
private:
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* Tracer;
	
	UPROPERTY()
	class UNiagaraComponent* TracerComponent;
	
	FTimerHandle DestroyTimer;
	
	UPROPERTY(EditAnywhere)
	float DestroyTime = 3.f;
	
};
