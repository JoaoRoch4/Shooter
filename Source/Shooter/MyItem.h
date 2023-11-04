// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "MyItem.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AMyItem : public AItem {
    GENERATED_BODY()

public:
    AMyItem();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;
};
