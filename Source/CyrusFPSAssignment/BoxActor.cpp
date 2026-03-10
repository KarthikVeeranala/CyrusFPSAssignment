#include "BoxActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "CyrusFPSAssignmentGameMode.h"

ABoxActor::ABoxActor()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube"));

    if (CubeMesh.Succeeded())
    {
        Mesh->SetStaticMesh(CubeMesh.Object);
    }
}

void ABoxActor::BeginPlay()
{
    Super::BeginPlay();

    if (Mesh)
    {
        DynamicMaterial = Mesh->CreateDynamicMaterialInstance(0);
    }
}

void ABoxActor::ApplyDamage()
{
    Health--;

    if (Health <= 0)
    {
        if (GetWorld())
        {
            ACyrusFPSAssignmentGameMode* GM =
                Cast<ACyrusFPSAssignmentGameMode>(GetWorld()->GetAuthGameMode());

            if (GM)
            {
                GM->AddScore(Score);
            }
        }

        Destroy();
    }
}

void ABoxActor::SetHealth(int NewHealth)
{
    Health = NewHealth;
}

void ABoxActor::SetScore(int NewScore)
{
    Score = NewScore;
}

void ABoxActor::SetColor(FLinearColor NewColor)
{
    if (DynamicMaterial)
    {
        DynamicMaterial->SetVectorParameterValue("Color", NewColor);
    }
}