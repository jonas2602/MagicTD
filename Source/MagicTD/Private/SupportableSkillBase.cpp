// Fill out your copyright notice in the Description page of Project Settings.

#include "SupportableSkillBase.h"

// Sets default values for this component's properties
USupportableSkillBase::USupportableSkillBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void USupportableSkillBase::TickSkill(float DeltaTime)
{
	if (CheckForAbortExec())
	{
		AbortSupport();
	}

	Super::TickSkill(DeltaTime);
}

void USupportableSkillBase::FinishSkill(bool Aborted)
{
	FinishSupport(Aborted);

	Super::FinishSkill(Aborted);
}

/***************************** End of Override **********************************/

bool USupportableSkillBase::ActivateSupport(AMyMotionController* SupportController)
{
	this->SupportController = SupportController;
	
	// Break when not supportable
	if (!SupportableExec(SupportController)) return false;

	if (AutoStartMontage)
	{
		StartMontage(SupportController, SupportMontage);
	}

	ActivateSupportExec();
	return true;
}

void USupportableSkillBase::ReleaseSupport()
{
	ReleaseSupportExec();

	if (FinishSupportOnRelease)
	{
		FinishSupport(false);
	}
}

void USupportableSkillBase::AbortSupport()
{
	AbortSupportExec();

	FinishSupport(true);
}

void USupportableSkillBase::FinishSupport(bool Aborted)
{
	if (AutoStopMontage)
	{
		StopMontage(SupportController, SupportMontage);
	}

	OnSupportFinished.Broadcast(this);
	PostFinishSupportExec(Aborted);

	this->SupportController = nullptr;
}