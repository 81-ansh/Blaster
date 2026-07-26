// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BlasterHUD.generated.h"

class UElimAnnouncement;
class UAnnouncement;
class UTexture2D;
class UCharacterOverlay;
class APlayerController;

USTRUCT(BlueprintType)
struct FHUDPackage
{
	GENERATED_BODY()
	
public:
	
	TObjectPtr<UTexture2D> CrosshairsCenter;
	TObjectPtr<UTexture2D> CrosshairsLeft;
	TObjectPtr<UTexture2D> CrosshairsRight;
	TObjectPtr<UTexture2D> CrosshairsTop;
	TObjectPtr<UTexture2D> CrosshairsBottom;
	float CrosshairSpread;
	FLinearColor CrosshairColor;
};

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	/*
	 * Character Overlay
	 */
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	TSubclassOf<UUserWidget> CharacterOverlayClass;
	
	UPROPERTY()
	TObjectPtr<UCharacterOverlay> CharacterOverlay;
	
	void AddCharacterOverlay();
	
	/*
	 * Announcement
	 */
	
	UPROPERTY(EditAnywhere, Category = "Announcements")
	TSubclassOf<UUserWidget> AnnouncementClass;
	
	UPROPERTY()
	TObjectPtr<UAnnouncement> Announcement;
	
	void AddAnnouncement();
	void AddElimAnnouncement(FString Attacker, FString Victim);
	
	virtual void DrawHUD() override;
	FORCEINLINE void SetHUDPackage(const FHUDPackage& Package) { HUDPackage = Package; }
	
protected:
	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY()
	APlayerController* OwningPlayer;
	
	FHUDPackage HUDPackage;
	
	void DrawCrosshair(UTexture2D* Texture, FVector2D ViewportCenter, FVector2D Spread, FLinearColor CrosshairColor);
	
	UPROPERTY(EditAnywhere)
	float CrosshairSpreadMax = 16.f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UElimAnnouncement> ElimAnnouncementClass;
};
