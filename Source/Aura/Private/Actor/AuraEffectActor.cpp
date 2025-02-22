


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass) const
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if(TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = TargetASC->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle GameplayEffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, GameplayEffectContextHandle);
	TargetASC->ApplyGameplayEffectSpecToSelf(*GameplayEffectSpecHandle.Data.Get());
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	if(InstanceApplyEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	if(DurationApplyEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if(InstanceApplyEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		ApplyEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	if(DurationApplyEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
}

