// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "WeaponSocketTypes_Base.h"
#include "DamageComponents/Damagable.h"

#include "WeaponSocket.generated.h"

class UWeaponSocketSettings_Base;
class APlayerCharacter;
struct FWeaponSocketData;
class UWeaponBase;
class UCameraComponent;
class AWeaponProjectile;
class UWeaponSocketUserWidget;

UENUM(BlueprintType)
enum class EWeaponSocketStates : uint8
{
	Fire UMETA(DisplayName = "Fire"),
    NotFire UMETA(DisplayName = "NotFire"),
	DifferentWeapon UMETA(DisplayName = "DifferentWeapon")
};

UCLASS()
class BLUNDERSGAME_API UWeaponSocket : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UWeaponSocket();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent *SocketMeshComponent;

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
	void SetWeaponState(EWeaponSocketStates NewState);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon Behaviour")
	void OnWeaponStateChanged();

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
    virtual void FireProjectile() {};

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
	void PlayAnimationMontages();

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
    void SetCanShoot(bool bCanShoot);

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
	bool GetCanShoot() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
	bool ShootCheck() const;

	UFUNCTION(BlueprintCallable, Category = "Weapon Behaviour")
	void ReduceAmmo();
	
	void SetInfoVisibility(bool bVisible);

	void Init(
		const FWeaponSocketData& InSocketData,
		UPrimitiveComponent* InAttachToComponent,
		UWeaponBase* InWeaponBase,
		APlayerCharacter* InPlayerCharacter);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual const FDamageEffects& GetDamageSettings();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void SetupWeaponSettings(UWeaponSocketSettings_Base* InWeaponSettings);

	UFUNCTION()
	void PlayerRecoil();

	UFUNCTION()
	void PlayCameraShake();

	UFUNCTION()
	void PlayWeaponSound();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Weapon Behaviour")
	float GetShotDelay() const;

	UFUNCTION()
	void ActivateShotAttributes();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Animation")
	UAnimMontage *SocketFireMontage;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon Behaviour")
    EWeaponSocketStates WeaponState;

	UPROPERTY()
    UWeaponBase *WeaponBase;

	UPROPERTY(BlueprintReadWrite, Category="Weapon Settings|Weapon Info")
	UWeaponSocketUserWidget *SocketWidget;
	
	UPROPERTY()
	APlayerCharacter* Player;

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsBase SocketSettingsBase {};

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsAmmo SocketSettingsAmmo {};

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsAimHelper SocketSettingsAimHelper {};

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsDamage SocketSettingsDamage {};

	UPROPERTY(BlueprintReadOnly)
	FWeaponSocketSettingsFeeling SocketSettingsFeeling {};

private:
	UFUNCTION()
	void RecoilHandle(float value);

	void CreateVisualComponents(const FWeaponSocketData& InSocketData, UPrimitiveComponent* InAttachToComponent);
	
	FTimeline RecoilTimeline;
	bool CanShoot = true;
};
