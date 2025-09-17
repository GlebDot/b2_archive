// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyHolderComponent.generated.h"

class UKeysScreenWidget;

UENUM(BlueprintType)
enum class EKeyTypes : uint8
{
	Red UMETA(DisplayName="Red"),
	Green UMETA(DisplayName="Green"),
	Blue UMETA(DisplayName="Blue"),
	White UMETA(DisplayName="White")
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLUNDERSGAME_API UKeyHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKeyHolderComponent();

	virtual void OnComponentCreated() override;

	UFUNCTION(BlueprintCallable)
	void ReceiveKey(const EKeyTypes& KeyType, const uint8& KeyAmount);

	UFUNCTION(BlueprintCallable)
	bool RemoveKey(const EKeyTypes& KeyType);

	UFUNCTION(BlueprintCallable)
	bool CombineKeysToWhite();

	UFUNCTION(BlueprintCallable)
	const TMap<EKeyTypes, uint8>& GetAvailableKeys() const { return KeysMap; }

	UFUNCTION(BlueprintCallable)
	void ToggleKeysScreenVisibility();
	
	UFUNCTION()
	void ShowKeysScreen();

	UFUNCTION()
    void HideKeysScreen();

protected:
	UPROPERTY()
	TMap<EKeyTypes, uint8> KeysMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UKeysScreenWidget> KeysScreenWidgetType;

	UPROPERTY()
	UKeysScreenWidget *KeysScreenWidget;

	UFUNCTION()
	void LoadKeysFromGameInstance();

	UFUNCTION()
	void UploadKeysToGameInstance() const;

private:
	UPROPERTY()
	bool IsKeyScreenVisible = false;
		
};
