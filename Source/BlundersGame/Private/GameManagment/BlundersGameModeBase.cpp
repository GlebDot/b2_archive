// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/GameManagment/BlundersGameModeBase.h"
#include "Engine/LevelScriptActor.h"
#include "EngineUtils.h"
#include "BlundersGame/Public/GameManagment/CheckPointActor.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase/PlayerCharacter.h"

ABlundersGameModeBase::ABlundersGameModeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
}

void ABlundersGameModeBase::ResetLevel()
{
    APawn *Pawn = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn();
    if (Pawn == nullptr || Pawn->IsPendingKillPending())
    {
        Super::ResetLevel();
    } else
    {
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->UnPossess();

        Pawn->SpawnDefaultController();
			
        Super::ResetLevel();
    
        Pawn->GetController()->Destroy();
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(Pawn);
        RestartPlayerAfterFalloff(Pawn);
    }
}

void ABlundersGameModeBase::RestartPlayerAfterDeath()
{
    ResetLevel();

    if (CurrentCheckPoint)
    {
        const FVector SpawnLocation = CurrentCheckPoint->GetActorLocation();
        APawn *Player = GetWorld()->SpawnActor<APawn>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator);
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(Player);
        OnPlayerDead();
    }
}

void ABlundersGameModeBase::SetCurrentCheckPoint(ACheckPointActor* CheckPoint)
{
    CurrentCheckPoint = CheckPoint;
}

void ABlundersGameModeBase::RestartPlayerAfterFalloff(APawn *Player)
{
    if (CurrentCheckPoint)
    {
        OnPlayerFellOut();
        Player->SetActorLocation(CurrentCheckPoint->GetActorLocation());
    }
}
