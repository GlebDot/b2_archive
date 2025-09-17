// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/AnimInstances/EnemyBasicAnimInstance.h"
#include "EnemyMountAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UEnemyMountAnimInstance : public UEnemyBasicAnimInstance
{
	GENERATED_BODY()


public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
	void SetMountYaw(float NewYaw)
	{
		this->MountYaw = NewYaw;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
	void SetMountPitch(float NewPitch)
	{
		this->MountPitch = NewPitch;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
	void SetIsMount(bool bIsMount)
	{
		this->IsMount = bIsMount;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
    float GetMountYaw() const
	{
		return MountYaw;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
    float GetMountPitch() const
	{
		return MountPitch;
	}

	UFUNCTION(BlueprintCallable, Category = "Enemy Animation|Mount")
    bool GetIsMount() const
	{
		return IsMount;
	}

	UFUNCTION()
	void SetSocketRotation(const FRotator &Rotation)
	{
		EnemySocketRotationSave = Rotation;
	}

	UFUNCTION()
	FRotator GetSocketRotation() const
	{
		return EnemySocketRotationSave;
	}

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Enemy Mount")
	float MountPitch;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Mount")
	float MountYaw;

	UPROPERTY(BlueprintReadWrite, Category = "Enemy Mount")
	bool IsMount = false;

private:
	UPROPERTY()
	FRotator EnemySocketRotationSave;
};
