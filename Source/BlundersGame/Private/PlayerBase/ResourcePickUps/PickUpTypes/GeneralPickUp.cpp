// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerBase/ResourcePickUps/PickUpTypes/GeneralPickUp.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "BlundersGame/Public/PlayerBase/Components/PlayerSpeedEnergyComponent.h"

AGeneralPickUp::AGeneralPickUp()
{
    PickUpMesh->SetVectorParameterValueOnMaterials(FName("Color"), FVector(10.0f, 0.0f, 10.0f));
}

void AGeneralPickUp::TransferResources(APlayerCharacter* Player)
{
    Player->SpeedEnergyComponent->AddEnergy(23.0f);
}
