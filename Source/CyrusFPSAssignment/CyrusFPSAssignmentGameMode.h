#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Http.h"
#include "Json.h"
#include "JsonUtilities.h"

class ABoxActor;

#include "CyrusFPSAssignmentGameMode.generated.h"

USTRUCT(BlueprintType)
struct FBoxTypeData
{
    GENERATED_BODY()

    int Health;
    int Score;
    FLinearColor Color;
};

UCLASS(abstract)
class CYRUSFPSASSIGNMENT_API ACyrusFPSAssignmentGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:

    ACyrusFPSAssignmentGameMode();
    virtual void BeginPlay() override;

    UPROPERTY(BlueprintReadOnly)
    int PlayerScore = 0;

    void AddScore(int ScoreToAdd);

private:

    void FetchJson();
    void OnJsonResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

    UPROPERTY(EditAnywhere)
    TSubclassOf<ABoxActor> BoxActorClass;

    TMap<FString, FBoxTypeData> BoxTypes;
};