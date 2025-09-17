// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Interactable.h"
#include "GameFramework/Actor.h"
#include "InteractableActorParent.generated.h"

UCLASS()
class BLUNDERSGAME_API AInteractableActorParent : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableActorParent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent *Mesh;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interactable")
    void Interact(AActor *InteractedActor);
	virtual void Interact_Implementation(AActor *InteractedActor) override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category="Interactable")
	void OnInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
