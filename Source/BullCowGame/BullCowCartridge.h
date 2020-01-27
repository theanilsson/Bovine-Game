// Header file

#pragma once

#include "CoreMinimal.h"
#include "Console/Cartridge.h"
#include "BullCowCartridge.generated.h"

struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BULLCOWGAME_API UBullCowCartridge : public UCartridge
{
	GENERATED_BODY()

	// Function declaration
	public:
	virtual void BeginPlay() override;
	virtual void OnInput(const FString& Input) override;
	void InitializeGame();
	bool IsIsogram(const FString& Entry) const;
	TArray<FString> GetIsograms(const TArray<FString>& WordList) const;
	FBullCowCount GetBullsAndCows(const FString& Guess) const;

	// Variable declarations
	private:
	FString HiddenWord;
	int32 NumberOfGuesses;
	int32 Lives;
	bool FirstEntry;
	TArray<FString> Isograms;
};
