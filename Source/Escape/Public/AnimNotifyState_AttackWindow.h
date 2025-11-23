
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"

#include "AnimNotifyState_AttackWindow.generated.h"
/**
 * 
 */

UCLASS()
class ESCAPE_API UAnimNotifyState_AttackWindow : public UAnimNotifyState
{
    GENERATED_BODY()

public:

    virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};

