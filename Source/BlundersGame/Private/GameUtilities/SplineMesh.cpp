// Fill out your copyright notice in the Description page of Project Settings.


#include "BlundersGame/Public/GameUtilities/SplineMesh.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASplineMesh::ASplineMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));

	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);
	}

}

//Constructing Spline Mesh
void ASplineMesh::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//We don't do anything if we don't have a mesh
	if (!DefaultMesh)
	{
		return;
	}

	//Scale segment length by scale
	//For better handling scaling of spline mesh
	SegmentLength = GetActorScale().X * SegmentLength;

	//How many segments is needed to spawn
	//It's equals to SplineLength / SegmentLength
	//Basically it means that "How many Segment of X length we can put in spline of Y length"
	SegmentsCount = FMath::TruncToInt(SplineComponent->GetSplineLength() / SegmentLength);

	//Set each Spline Mesh start and end point
	for (auto SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
	}

	SplineMeshes.Empty();
	for (int SplineCount = 0; SplineCount < SegmentsCount; SplineCount++)
	{
		//Creating Spline Mesh Component
		USplineMeshComponent *SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass(), FName(GetName() + FString::FromInt(SplineCount)));
		
		//SetUp Spline Mesh Component
		SplineMeshComponent->RegisterComponent();
		//SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->SetStaticMesh(DefaultMesh);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);

		//Getting Start and End Tangents and Locations and Rolls
		const float CurrentDistanceAlongSpline = SplineCount * SegmentLength;

		//Get Start point parameters
		const FVector StartPoint = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::Local);
		const FVector StartTangent = SplineComponent->GetTangentAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::Local);
		const float StartRoll = SplineComponent->GetRollAtDistanceAlongSpline(CurrentDistanceAlongSpline, ESplineCoordinateSpace::Local);

		//Get End point parameters
		const FVector EndPoint = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistanceAlongSpline + SegmentLength, ESplineCoordinateSpace::Local);
		const FVector EndTangent = SplineComponent->GetTangentAtDistanceAlongSpline(CurrentDistanceAlongSpline + SegmentLength, ESplineCoordinateSpace::Local);
		const float EndRoll = SplineComponent->GetRollAtDistanceAlongSpline(CurrentDistanceAlongSpline + SegmentLength, ESplineCoordinateSpace::Local);

		//Set Spline Mesh parameters
		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent.GetClampedToMaxSize(SegmentLength),
			EndPoint, EndTangent.GetClampedToMaxSize(SegmentLength), true);

		//It's a little bit strange, but we are getting roll in degrees
		//And setting them in radians :0
		SplineMeshComponent->SetStartRoll(StartRoll * PI / 180);
		SplineMeshComponent->SetEndRoll(EndRoll * PI / 180);

		//Set Mesh Orientation
		SplineMeshComponent->SetForwardAxis(MeshAxis);

		//Set Collision >:|
		SplineMeshComponent->SetCollisionEnabled(MeshCollision);
		SplineMeshComponent->SetCollisionProfileName(CollisionProfile);
		SplineMeshComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

		SplineMeshes.Add(SplineMeshComponent);
	}
	
}

void ASplineMesh::Destroyed()
{
	Super::Destroyed();

	for (auto SplineMesh : SplineMeshes)
	{
		SplineMesh->DestroyComponent();
	}

	SplineMeshes.Empty();
}

