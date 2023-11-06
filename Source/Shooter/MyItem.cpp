// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Custom.h"
#include <iostream>
#include <string>
#include <any>

AMyItem::AMyItem() { PrimaryActorTick.bCanEverTick = true; }

void AMyItem::BeginPlay() { 

    Super::BeginPlay();
    PrintLogWarning("Log Clear");
    ClearLog();
}

void AMyItem::Tick(float DeltaTime) { Super::Tick(DeltaTime); }
