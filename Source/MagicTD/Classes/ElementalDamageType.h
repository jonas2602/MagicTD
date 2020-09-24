// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "ElementalDamageType.generated.h"

/**
 *
 */

UENUM(BlueprintType)
enum class EElementType : uint8
{
	Fire,
	Ice,
	Energy
};

UCLASS() // TODO: Seperate Events for different Elements? // Call via Interface?
class MAGICTD_API UElementalDamageType : public UDamageType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	EElementType Element;

};

//USTRUCT()
//struct MAGICTD_API FElementalDamageEvent : public FDamageEvent
//{
//	GENERATED_USTRUCT_BODY()
//
//		// skipping ImpulseMag for now
//		UPROPERTY()
//		float Damage;
//
//	/** Direction the shot came from. Should be normalized. */
//	UPROPERTY()
//		FVector_NetQuantizeNormal ShotDirection;
//
//	UPROPERTY()
//		struct FHitResult HitInfo;
//
//	FElementalDamageEvent() : HitInfo() {}
//	FElementalDamageEvent(float InDamage, struct FHitResult const& InHitInfo, FVector const& InShotDirection, TSubclassOf<class UDamageType> InDamageTypeClass)
//		: FDamageEvent(InDamageTypeClass), Damage(InDamage), ShotDirection(InShotDirection), HitInfo(InHitInfo)
//	{}
//
//	/** ID for this class. NOTE this must be unique for all damage events. */
//	static const int32 ClassID = 1;
//
//	virtual int32 GetTypeID() const override { return FElementalDamageEvent::ClassID; };
//	virtual bool IsOfType(int32 InID) const override { return (FElementalDamageEvent::ClassID == InID) || FDamageEvent::IsOfType(InID); };
//
//	/** Simple API for common cases where we are happy to assume a single hit is expected, even though damage event may have multiple hits. */
//	virtual void GetBestHitInfo(AActor const* HitActor, AActor const* HitInstigator, struct FHitResult& OutHitInfo, FVector& OutImpulseDir) const override
//	{
//		// assume the actor got hit in the center of his root component
//		OutHitInfo = HitInfo;
//		OutImpulseDir = ShotDirection;
//	}
//};