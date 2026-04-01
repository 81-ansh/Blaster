// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/Weapon.h"
#include "Components/SphereComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	WeaponMesh->SetupAttachment(RootComponent);
	SetRootComponent(WeaponMesh);
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Block);							// block all channels by default
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);		// override: ignore pawns so players can walk over it
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// disable collision for now (re-enabled when dropped)
	
	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECR_Ignore);							// ignore all channels by default
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);			// collision off by default, enabled only on server

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);	// enable collision on server
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);	// detect when pawns overlap the pickup sphere
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

