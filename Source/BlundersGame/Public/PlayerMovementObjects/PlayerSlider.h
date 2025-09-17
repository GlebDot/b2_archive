// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BlundersGame/Public/GameUtilities/SplineMesh.h"
#include "PlayerSlider.generated.h"

class APlayerCharacter;
/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API APlayerSlider : public ASplineMesh
{
	GENERATED_BODY()
protected:
	bool IsUsed;
	
	UPROPERTY()
	APlayerCharacter *Player;

	float TrackLength;
	float TrackAlpha;
	float YawOffset;
	float PitchRollMultiplier;
	float TrackAlphaSumator;

	void BeginPlay() override;
public:
	void Tick(float DeltaTime) override;


	void SetIsUsed(bool bIsUsed)
	{
		IsUsed = bIsUsed;
		if (!bIsUsed)
		{
			OnActorBeginOverlap.AddDynamic(this, &APlayerSlider::OnActorOverlapBegin);
		}
	}

	APlayerSlider();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider Settings")
	float CharacterSpeed = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slider Settings")
	float ZOffset = 20.0;

	UFUNCTION()
    void OnActorOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

};
