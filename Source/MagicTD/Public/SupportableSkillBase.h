// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "SupportableSkillBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGICTD_API USupportableSkillBase : public USkillBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	USupportableSkillBase();

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Skill")
	FSkillFinishedDelegate OnSupportFinished;

public:
	/*********** Start Override Base **************/
	virtual void TickSkill(float DeltaTime) override;
	virtual void FinishSkill(bool Aborted) override;
	/************ Stop Override Base **************/
	
	UFUNCTION(BlueprintCallable, Category = "Support")
	bool ActivateSupport(AMyMotionController* SupportController);

	UFUNCTION(BlueprintCallable, Category = "Support")
	void ReleaseSupport();

	UFUNCTION(BlueprintCallable, Category = "Support")
	virtual void AbortSupport();

	UFUNCTION(BlueprintCallable, Category = "Support")
	void FinishSupport(bool Aborted);

protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Support")
	UAnimMontage* SupportMontage;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Support")
	AMyMotionController* SupportController;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Support")
	bool FinishSupportOnRelease = true;
	
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Support")
	void ActivateSupportExec();
	virtual void ActivateSupportExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Support")
	void ReleaseSupportExec();
	virtual void ReleaseSupportExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Support")
	void AbortSupportExec();
	virtual void AbortSupportExec_Implementation() { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Support")
	void PostFinishSupportExec(bool Aborted);
	virtual void PostFinishSupportExec_Implementation(bool Aborted) { }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Support")
	bool SupportableExec(AMyMotionController* OtherController);
	virtual bool SupportableExec_Implementation(AMyMotionController* OtherController) { return false; }

};
