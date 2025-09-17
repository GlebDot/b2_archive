// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/PlayerMovementStates/SpecialAttacksStates.h"

#include "DrawDebugHelpers.h"
#include "BlundersGame/Public/PlayerBase/PlayerCharacter.h"
#include "DamageComponents/Explosions/ExplosionBasic.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

//TODO: questionable states
void UPlayerAttackStateGround::OnExecute()
{
    Super::OnExecute();
    //GetReference to Player Controller
    PlayerController = Cast<APlayerController>(Player->GetController());
    Player->DisableInput(PlayerController);

    //Setup Collision Settings for First Trace
    FCollisionShape CollisionShape;
    CollisionShape.SetSphere(Player->GetSpecialAttackSpreadRadius());
    
    FVector StartLocation = Player->GetActorLocation();
    FVector EndLocation = StartLocation + Player->GetActorForwardVector() * Player->GetSpecialAttackDistance();
    
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn); //enemies

    FCollisionQueryParams CollisionQueryParams = FCollisionQueryParams::DefaultQueryParam;
    CollisionQueryParams.AddIgnoredActor(Player);

    //First Trace
    TArray<FHitResult> Hits;
    Player->GetWorld()->SweepMultiByObjectType(Hits,
                                               StartLocation,
                                               EndLocation,
                                               FQuat::Identity,
                                               ObjectQueryParams,
                                               CollisionShape,
                                               CollisionQueryParams);

    //Setup Collision Settings for Check trace 
    FCollisionObjectQueryParams CheckTraceParams;
    CheckTraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
    CheckTraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
    // CheckTraceParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
    const FVector CheckTraceEnd = Player->GetActorLocation();

    //Check traces
    for (auto &Hit : Hits)
    {
        FHitResult HitResult;

        FCollisionQueryParams CheckTraceQueryParams = FCollisionQueryParams::DefaultQueryParam;
        CheckTraceQueryParams.AddIgnoredActor(Hit.GetActor());
        
        const FVector CheckTraceStart = Hit.GetActor()->GetActorLocation();
        
        bool IsHit = Player->GetWorld()->LineTraceSingleByObjectType(HitResult,
                                                                     CheckTraceStart,
                                                                     CheckTraceEnd,
                                                                     CheckTraceParams,
                                                                     CheckTraceQueryParams);

        //Applying damage if we passed a check trace
        if (IsHit)
        {
            if (HitResult.GetActor() == Player)
            {
                GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Damaging From fists");
                UGameplayStatics::ApplyDamage(Hit.GetActor(),
                                              Player->GetSpecialAttackDamage(),
                                              Player->GetController(),
                                              Player,
                                              Player->GetSpecialAttackDamageType());
            }
        }
    }

    
    //Setup Cooldown
    FTimerHandle SpecialAttackEndTimerHandle;
    Player->GetWorld()->GetTimerManager().SetTimer(SpecialAttackEndTimerHandle,
                                                   this,
                                                   &UPlayerAttackStateGround::AttackEnd,
                                                   Player->GetSpecialAttackDuration(),
                                                   false);
}

void UPlayerAttackStateGround::AttackEnd()
{
    Player->EnableInput(PlayerController);
    Player->ResetCurrentState();
}

void UPlayerAttackStateAir::OnLanding()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, Player->GetSpecialAttackDamageType()->GetName());
    FVector SpawnLocation = Player->GetActorLocation();
    SpawnLocation.Z += 10.0f;
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, SpawnLocation.ToString());
    const FTransform SpawnTransform = FTransform(FRotator::ZeroRotator, SpawnLocation, FVector(1.0f));
    AExplosionBasic *Explosion = Player->GetWorld()->SpawnActor<AExplosionBasic>(AExplosionBasic::StaticClass(), SpawnTransform);
    if (Explosion)
    {
        Explosion->CollisionQueryParams.AddIgnoredActor(Player);
        Explosion->SetInstigator(Player);
        Explosion->SetDamage(Player->GetSpecialAttackDamage());
        Explosion->SetDamageRadius(Player->GetSpecialAttackDistance() * 2.0f);
        Explosion->SetDamageType(Player->GetSpecialAttackDamageType());
    }
    
    // const TArray<AActor*> IgnoreActors;
    // UGameplayStatics::ApplyRadialDamage(Player->GetWorld(),
    //                                     Player->GetSpecialAttackDamage(), 
    //                                     Player->GetActorLocation(),
    //                                     Player->GetSpecialAttackDistance(),
    //                                     Player->GetSpecialAttackDamageType(),
    //                                     IgnoreActors,
    //                                     Player,
    //                                     PlayerController,
    //                                     true);
    //
    // DrawDebugSphere(Player->GetWorld(), Player->GetActorLocation(), Player->GetSpecialAttackDistance(), 12, FColor::Green, false, 0.25);

    FTimerHandle SpecialAttackEndTimerHandle;
    Player->GetWorld()->GetTimerManager().SetTimer(SpecialAttackEndTimerHandle, this, &UPlayerAttackStateAir::AttackEnd, Player->GetSpecialAttackDuration(), false);
}

void UPlayerAttackStateAir::AttackEnd()
{
    Super::OnLanding();

    Player->ResetCurrentState();
    // PlayerMovementComponent->GravityScale = GravityScaleSave;
    // Player->EnableInput(PlayerController);
}
