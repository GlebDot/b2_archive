// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "GameFramework/Actor.h"
#include "SplineMesh.generated.h"

UCLASS()
class BLUNDERSGAME_API ASplineMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASplineMesh();

	UFUNCTION()
	int32 GetSegmentsCount() const
	{
		return SegmentsCount;
	}

	virtual void OnConstruction(const FTransform& Transform) override;

	virtual void Destroyed() override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Spline")
	USplineComponent *SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	UStaticMesh *DefaultMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ECollisionEnabled::Type> MeshCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	FName CollisionProfile = "Trigger";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TEnumAsByte<ESplineMeshAxis::Type> MeshAxis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
    float SegmentLength = 100.0;

	UPROPERTY()
	TArray<USplineMeshComponent*> SplineMeshes;

private:
	UPROPERTY()
	int32 SegmentsCount;
};
