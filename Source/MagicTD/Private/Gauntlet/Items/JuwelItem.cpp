// Fill out your copyright notice in the Description page of Project Settings.

#include "JuwelItem.h"
#include "JuwelBase.h"

UJuwelItem::UJuwelItem()
{
	JuwelClass = AJuwelBase::StaticClass();
	JuwelColor = FLinearColor::White;
	IconScale = 1.0f;
	IconRotationAngle = 0.0f;
	IconAspectRatio = FVector2D(1.0f, 1.0f);
}


