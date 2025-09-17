// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementState.h"
#include "PlayerJumpStates.generated.h"

/**
 * 
 */
UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateSlider : public UPlayerMovementState
{
    GENERATED_BODY()

public:
    UPlayerJumpStateSlider();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateGround : public UPlayerMovementState
{
    GENERATED_BODY()

public:
    UPlayerJumpStateGround();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateEnemy : public UPlayerMovementState
{
    GENERATED_BODY()

    public:
    UPlayerJumpStateEnemy();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateMidAir : public UPlayerMovementState
{
    GENERATED_BODY()

public:
    UPlayerJumpStateMidAir();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateWall : public UPlayerMovementState
{
    GENERATED_BODY()

public:
    UPlayerJumpStateWall();
    virtual void OnExecute() override;

private:
    UFUNCTION()
    void LaterJump();

    float GravityScaleSave;

    UPROPERTY()
    FTimerHandle TH_LaterJump;

    UPROPERTY()
    APlayerController *Controller;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateLong : public UPlayerMovementState
{
    GENERATED_BODY()
public:
    UPlayerJumpStateLong();
    virtual void OnExecute() override;
};

UCLASS()
class BLUNDERSGAME_API UPlayerJumpStateBackward : public UPlayerMovementState
{
    GENERATED_BODY()
public:
    UPlayerJumpStateBackward();
    virtual void OnExecute() override;
    
private:
    void RotateCharacter();
    FRotator StartRotation;
    FTimerHandle RotationTimerHandle;

    float RotationRate;

    UPROPERTY()
    APlayerController *Controller;
};

UCLASS()
class UPlayerJumpStateJumper : public UPlayerMovementState
{
    GENERATED_BODY()
public:
    UPlayerJumpStateJumper();
    virtual void OnExecute() override;

    virtual void SetVectorParameter(const FVector& Value) override {JumpVelocity = Value;};
    
protected:
    UPROPERTY()
    FVector JumpVelocity;
};

UCLASS()
class UPlayerJumpStateHigh : public UPlayerMovementState
{
    GENERATED_BODY()
public:
    UPlayerJumpStateHigh();
    virtual void OnExecute() override;
    
};
