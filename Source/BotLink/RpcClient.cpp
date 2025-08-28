// Fill out your copyright notice in the Description page of Project Settings.

#include "RpcClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Math/UnrealMathUtility.h"

void URpcClient::SendMoveCommandFromXY(float X, float Y)
{
    // Stop if the stick is in the dead zone
    if (FMath::Abs(X) < 0.3f && FMath::Abs(Y) < 0.3f)
    {
        UE_LOG(LogTemp, Warning, TEXT("[RPC] Stopping robot (input too low)."));
        Stop();
        return;
    }

    // Convert stick input to movement angle in degrees (0° = right, 90° = forward)
    float AngleRad = FMath::Atan2(Y, X);
    float AngleDeg = FMath::RadiansToDegrees(AngleRad);

    // Normalize angle to [0, 360)
    if (AngleDeg < 0.0f)
    {
        AngleDeg += 360.0f;
    }

    UE_LOG(LogTemp, Log, TEXT("[RPC] MoveCommand: X=%.2f, Y=%.2f ? Angle=%.1f"), X, Y, AngleDeg);
    SendMoveCommand(AngleDeg);
}

void URpcClient::SendMoveCommand(float Angle)
{
    FString Params = FString::Printf(TEXT("[%f]"), Angle);
    SendRPCRequest("SetMovementAngle", Params);
}

void URpcClient::Stop()
{
    UE_LOG(LogTemp, Warning, TEXT("[RPC] STOP command sent."));
    SendRPCRequest("SetMovementAngle", TEXT("[-1]"));
}

void URpcClient::SendClawIKCommand(float X, float Y, float Z)
{
    FString Params = FString::Printf(TEXT("[%f,%f,%f,0,-90,90,1000]"), X, Y, Z);
    SendRPCRequest("ArmMoveIk", Params);
}


void URpcClient::SendRPCRequest(const FString& Method, const FString& ParamsJSON)
{
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();

    // video http://192.168.149.1:8080/
    //Request->SetURL("http://192.168.149.1:9030");
    Request->SetURL("http://192.168.0.42:9030");
    Request->SetVerb("POST");
    Request->SetHeader("Content-Type", "application/json");

    FString Body = FString::Printf(TEXT("{\"jsonrpc\":\"2.0\",\"method\":\"%s\",\"params\":%s,\"id\":1}"), *Method, *ParamsJSON);
    Request->SetContentAsString(Body);

    Request->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Req, FHttpResponsePtr Resp, bool bSuccess)
        {
            if (bSuccess && Resp.IsValid())
            {
                UE_LOG(LogTemp, Log, TEXT("RPC Response: %s"), *Resp->GetContentAsString());
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("RPC Request failed"));
            }
        });

    Request->ProcessRequest();
}
