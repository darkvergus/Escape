#include "GASCharacter.h"
#include "GASCharacterAttributeSet.h" 

AGASCharacter::AGASCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ASC = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	ASC->SetIsReplicated(true);

	AttributeSet = CreateDefaultSubobject<UGASCharacterAttributeSet>("AttributeSet");
	TraitManager = CreateDefaultSubobject<UTraitManagerComponent>("TraitManager");
	MentalResolver = CreateDefaultSubobject<UMentalStateResolverComponent>("MentalResolver");

	DefaultAttributeEffects = TArray<TSubclassOf<UGameplayEffect>>();
	AttackAbilityList = TArray<TSubclassOf<UGameplayAbility>>();
}

float AGASCharacter::GetHealth() const
{
	return AttributeSet ? AttributeSet->GetHealth() : 0.0f;
}

float AGASCharacter::GetMaxHealth() const
{
	return AttributeSet ? AttributeSet->GetMaxHealth() : 0.0f;
}

float AGASCharacter::GetStamina() const 
{

	return AttributeSet ? AttributeSet->GetStamina() : 0.0f;
}

float AGASCharacter::GetMaxStamina() const 
{
	return AttributeSet ? AttributeSet->GetMaxStamina() : 0.0f;
}

bool AGASCharacter::ActivateAbilitiesWithTag(FGameplayTagContainer AbilityTags, bool AllowRemoteActivation)
{
	return ASC ? ASC->TryActivateAbilitiesByTag(AbilityTags, AllowRemoteActivation) : false; 
}

void AGASCharacter::OnHealthChanged(float DeltaValue, AActor* Origin)
{

}

void AGASCharacter::OnStaminaChanged(float DeltaValue, AActor* Origin)
{

}

void AGASCharacter::OnDead()
{

}

void AGASCharacter::DebugDamage()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!ASC)
	{
		return;
	}

	ASC->ApplyModToAttributeUnsafe(UGASCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, -15.f);
}

void AGASCharacter::DebugHeal()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!ASC)
	{
		return;
	}

	ASC->ApplyModToAttributeUnsafe(UGASCharacterAttributeSet::GetHealthAttribute(), EGameplayModOp::Additive, +15.f);
}

void AGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	ASC->InitAbilityActorInfo(this, this);

	if (MentalResolver)
	{
		MentalResolver->Initialise(ASC);
		MentalResolver->Recompute();
	}

	if (HasAuthority() && TraitManager)
	{
		TraitManager->OnArchetypeApplied.AddDynamic(this, &AGASCharacter::HandleArchetypeApplied);

		if (Asset.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("[Traits] Applying direct archetype %s"), *Asset.GetAssetName());
			TraitManager->ApplyArchetype(Asset.Get());
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("[Traits] Rolling archetype from list"));
			TraitManager->RollArchetypes(1);
		}
	}
}

void AGASCharacter::SetTestAbilities() 
{
	if (!ASC) 
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& TestAbility : TestAbilities) 
		{
			ASC->GiveAbility(FGameplayAbilitySpec(TestAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void AGASCharacter::SetAttackAbilities()
{
	if (!ASC) 
	{
		return;
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		for (TSubclassOf<UGameplayAbility>& AttackAbility : AttackAbilityList) 
		{
			ASC->GiveAbility(FGameplayAbilitySpec(AttackAbility, GetCharacterLevel(), INDEX_NONE, this));
		}
	}
}

void AGASCharacter::HandleArchetypeApplied(UArchetypeDefinition* Arch)
{
	if (!Arch)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[Traits] Archetype applied: %s"), *Arch->GetName());

	if (MentalResolver)
	{
		MentalResolver->Recompute();
	}
}

void AGASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(FInputChord(EKeys::G), IE_Pressed, this, &AGASCharacter::DebugDamage);
	PlayerInputComponent->BindKey(FInputChord(EKeys::H), IE_Pressed, this, &AGASCharacter::DebugHeal);
}

void AGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!ASC) 
	{
		return;
	}
	if (EnableTestAbilities) 
	{
		SetTestAbilities();
	}

	ApplyDefaultAttributeEffects();
	SetAttackAbilities();

	const float Health = ASC->GetNumericAttribute(UGASCharacterAttributeSet::GetHealthAttribute());
	const float MaxHealth = ASC->GetNumericAttribute(UGASCharacterAttributeSet::GetMaxHealthAttribute());

	UE_LOG(LogTemp, Log, TEXT("Initial Health = %.1f"), Health);
	UE_LOG(LogTemp, Log, TEXT("Max Health = %.1f"), MaxHealth);
}

UAbilitySystemComponent* AGASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AGASCharacter::HandleHealthChange(float DeltaValue, AActor* Origin)
{
	OnHealthChanged(DeltaValue, Origin);
	if (GetHealth() <= 0.f) 
	{
		OnDead();
	}
}

void AGASCharacter::HandleStaminaChange(float DeltaValue, AActor* Origin)
{
	OnStaminaChanged(DeltaValue, Origin);
	//if (GetHealth() <= 0.f) {
	//	OnDead();
	//}
}

void AGASCharacter::ApplyDefaultAttributeEffects()
{
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySystemComponent is null!"));
		return;
	}

	FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (TSubclassOf <class UGameplayEffect>& DefaultEffect : DefaultAttributeEffects) 
	{
		//FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffects, CharacterLevel, EffectContext);

		if (!IsValid(DefaultEffect))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Gameplay Effect in DefaultAttributeEffects!"));
			continue;
		}
	}
}

void AGASCharacter::RemoveDefaultAttributeEffects() 
{
	FGameplayEffectQuery Query;
	Query.EffectSource = this;
	ASC->RemoveActiveEffects(Query);
}