// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ArmourPlate.h"
#include "CoreMinimal.h"
#include "../EnemyParent.h"
#include "Components/ActorComponent.h"
#include "ArmourContainerComponent.generated.h"


//
USTRUCT(BlueprintType)
struct FArmourPlateSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachSocketName = "none";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsMirrored = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh *PlateMesh = nullptr;
};


UCLASS( ClassGroup=(Enemy), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UArmourContainerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UArmourContainerComponent();

	virtual void OnComponentCreated() override;
	
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UPROPERTY(BlueprintReadOnly, Category="ArmourSettings|Base")
	float ArmourHealth = 10.0f;

	UPROPERTY(BlueprintReadWrite, Category="ArmourSettings|Base")
	TArray<FArmourPlateSettings> ArmourSettingsList;

	UFUNCTION(BlueprintCallable)
	void BreakAllArmour();

	UFUNCTION(BlueprintCallable)
	float DealDamageToArmourPlates(float Damage, AController* EventInstigator, AActor* DamageCauser, const UDamageType *DamageTypeClass);

protected:
	UPROPERTY(BlueprintReadOnly, Category="ArmourSettings")
	TArray<class UChildActorComponent*> ArmourComponents;

	UPROPERTY()
	bool IsArmourPlatesOn = true;
};
