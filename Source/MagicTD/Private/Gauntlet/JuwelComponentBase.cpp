// Fill out your copyright notice in the Description page of Project Settings.

#include "JuwelComponentBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/CollisionProfile.h"
#include "JuwelItem.h"
#include "Engine/Texture2D.h"

UJuwelComponentBase::UJuwelComponentBase()
	: JuwelColorMIP("Color")
	, JuwelIntensityMIP("Intensity")
	, JuwelIconMIP("Icon")
	, IconScaleMIP("Scale")
	, IconRotationMIP("Rotation")
	, IconAspectRatioMIP("AspectRatio")
	, GlowIntensity(10.0f)
{
	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
}

void UJuwelComponentBase::Init(UJuwelItem* Template)
{
	if (!Template) return;
	this->JuwelItem = Template;

	// Set Static Mesh
	SetStaticMesh(Template->JuwelMesh);

	// Init Material
	MaterialInstance = CreateDynamicMaterialInstance(0, JuwelMaterial);
	MaterialInstance->SetVectorParameterValue(JuwelColorMIP, Template->JuwelColor);
	MaterialInstance->SetScalarParameterValue(JuwelIntensityMIP, 0);
	MaterialInstance->SetTextureParameterValue(JuwelIconMIP, Template->ItemIcon);
	MaterialInstance->SetScalarParameterValue(IconScaleMIP, Template->IconScale);
	MaterialInstance->SetScalarParameterValue(IconRotationMIP, Template->IconRotationAngle);
	MaterialInstance->SetVectorParameterValue(IconAspectRatioMIP, FLinearColor(Template->IconAspectRatio.X, Template->IconAspectRatio.Y, 0));

}

void UJuwelComponentBase::ToggleGlow(bool Glow)
{
	if (!MaterialInstance) return;

	MaterialInstance->SetScalarParameterValue(JuwelIntensityMIP, Glow * GlowIntensity);
}
