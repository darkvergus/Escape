// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "WeaponBase.h"
#include "WeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UWeaponComponent : public USceneComponent
{
	GENERATED_BODY()

public:
    UWeaponComponent();


    /** Weapon to spawn and attach at BeginPlay */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    TSubclassOf<AWeaponBase> DefaultWeaponClass;

    /** Reference to the currently equipped weapon actor */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    AWeaponBase* CurrentWeapon;
    //
    //    /** The socket name to attach the weapon to */

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat|Weapon", meta = (ExposeOnSpawn = true))
    FName AttachToSocketName = "GripPoint";

    //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Preview")
    TSubclassOf<AWeaponBase> PreviewWeaponClass;

#if WITH_EDITORONLY_DATA
    // Keep a persistent pointer to the preview weapon
    UPROPERTY(Transient)
    AWeaponBase* PreviewWeaponActor = nullptr;
#endif



protected:
    virtual void BeginPlay() override;
    
    virtual void OnUnregister() override;

    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private :
    void UpdatePreviewWeapon();


public:
    /** Spawns and attaches a weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void EquipWeapon(TSubclassOf<AWeaponBase> NewWeaponClass);

    /** Removes the current weapon */
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    void UnequipWeapon();


    UFUNCTION(BlueprintCallable, Category = "Weapon")
    AWeaponBase* GetCurrentWeapon() const { return CurrentWeapon; }



    /** Optional delegate: broadcast when a weapon is equipped */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponEquippedSignature, AWeaponBase*, NewWeapon);

    UPROPERTY(BlueprintAssignable, Category = "Weapon|Events")
    FOnWeaponEquippedSignature OnWeaponEquipped;


};
