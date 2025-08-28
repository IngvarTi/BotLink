// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RpcClient.generated.h"

/**
 * 
 */
UCLASS()
class BOTLINK_API URpcClient : public UObject
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable, Category = "RPC")
    void SendMoveCommandFromXY(float X, float Y);

    UFUNCTION(BlueprintCallable, Category = "RPC")
    void SendMoveCommand(float Angle);

    UFUNCTION(BlueprintCallable, Category = "RPC")
    void Stop();

    void SendClawIKCommand(float X, float Y, float Z);

private:
    void SendRPCRequest(const FString& Method, const FString& ParamsJSON);
};

