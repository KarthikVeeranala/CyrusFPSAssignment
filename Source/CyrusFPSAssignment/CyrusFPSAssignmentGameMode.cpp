#include "CyrusFPSAssignmentGameMode.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Interfaces/IHttpRequest.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "BoxActor.h"

ACyrusFPSAssignmentGameMode::ACyrusFPSAssignmentGameMode()
{
}

void ACyrusFPSAssignmentGameMode::BeginPlay()
{
    Super::BeginPlay();

    FetchJson();
}

void ACyrusFPSAssignmentGameMode::FetchJson()
{
    FString Url = "https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json";

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request =
        FHttpModule::Get().CreateRequest();

    Request->OnProcessRequestComplete().BindUObject(
        this,
        &ACyrusFPSAssignmentGameMode::OnJsonResponse
    );

    Request->SetURL(Url);
    Request->SetVerb("GET");
    Request->ProcessRequest();
}

void ACyrusFPSAssignmentGameMode::OnJsonResponse(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful)
{
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
        return;
    }

    FString JsonString = Response->GetContentAsString();

    TSharedPtr<FJsonObject> JsonObject;

    TSharedRef<TJsonReader<>> Reader =
        TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("JSON Parsed Successfully"));

        const TArray<TSharedPtr<FJsonValue>>* TypesArray;

        if (JsonObject->TryGetArrayField("types", TypesArray))
        {
            for (auto& TypeValue : *TypesArray)
            {
                TSharedPtr<FJsonObject> TypeObject = TypeValue->AsObject();

                FString Name = TypeObject->GetStringField("name");
                int Health = TypeObject->GetIntegerField("health");
                int Score = TypeObject->GetIntegerField("score");

                const TArray<TSharedPtr<FJsonValue>>* ColorArray;
                TypeObject->TryGetArrayField("color", ColorArray);

                FLinearColor Color(
                    (*ColorArray)[0]->AsNumber(),
                    (*ColorArray)[1]->AsNumber(),
                    (*ColorArray)[2]->AsNumber(),
                    1.0f
                );

                FBoxTypeData Data;
                Data.Health = Health;
                Data.Score = Score;
                Data.Color = Color;

                BoxTypes.Add(Name, Data);

                UE_LOG(LogTemp, Warning, TEXT("Stored Type: %s"), *Name);
            }
        }
    }

    const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;

    if (JsonObject->TryGetArrayField("objects", ObjectsArray))
    {
        for (auto& ObjValue : *ObjectsArray)
        {
            TSharedPtr<FJsonObject> Obj = ObjValue->AsObject();

            FString TypeName = Obj->GetStringField("type");

            TSharedPtr<FJsonObject> TransformObj = Obj->GetObjectField("transform");

            const TArray<TSharedPtr<FJsonValue>>* LocationArray;
            TransformObj->TryGetArrayField("location", LocationArray);

            FVector Location(
                (*LocationArray)[0]->AsNumber(),
                (*LocationArray)[1]->AsNumber(),
                (*LocationArray)[2]->AsNumber()
            );

            const TArray<TSharedPtr<FJsonValue>>* RotationArray;
            TransformObj->TryGetArrayField("rotation", RotationArray);

            FRotator Rotation(
                (*RotationArray)[1]->AsNumber(),
                (*RotationArray)[2]->AsNumber(),
                (*RotationArray)[0]->AsNumber()
            );

            const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
            TransformObj->TryGetArrayField("scale", ScaleArray);

            FVector Scale(
                (*ScaleArray)[0]->AsNumber(),
                (*ScaleArray)[1]->AsNumber(),
                (*ScaleArray)[2]->AsNumber()
            );

            FTransform SpawnTransform(Rotation, Location, Scale);

            if (BoxActorClass)
            {
                ABoxActor* SpawnedBox =
                    GetWorld()->SpawnActor<ABoxActor>(
                        BoxActorClass.Get(),
                        SpawnTransform
                    );

                if (SpawnedBox && BoxTypes.Contains(TypeName))
                {
                    FBoxTypeData Data = BoxTypes[TypeName];

                    SpawnedBox->SetHealth(Data.Health);
                    SpawnedBox->SetScore(Data.Score);
                    SpawnedBox->SetColor(Data.Color);
                }
            }
        }
    }
}

void ACyrusFPSAssignmentGameMode::AddScore(int ScoreToAdd)
{
    PlayerScore += ScoreToAdd;

    UE_LOG(LogTemp, Warning, TEXT("Current Score: %d"), PlayerScore);
}