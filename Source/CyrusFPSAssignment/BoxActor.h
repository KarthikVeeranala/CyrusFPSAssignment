#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxActor.generated.h"

UCLASS()
class CYRUSFPSASSIGNMENT_API ABoxActor : public AActor
{
    GENERATED_BODY()

public:
    ABoxActor();

protected:
    virtual void BeginPlay() override;

public:
    void ApplyDamage();

private:

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UMaterialInstanceDynamic* DynamicMaterial;

    int Health = 3;
    int Score = 0;

public:

    void SetHealth(int NewHealth);
    void SetScore(int NewScore);
    void SetColor(FLinearColor NewColor);
};