#include "BullCowCartridge.h"
#include "HiddenWordList.h"
//#include "Math/UnrealMathUtility.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    // Welcome message and game initialization
    PrintLine(TEXT("Welcome to the Bull/Cow Game"));
    PrintLine(TEXT("Press enter to start..."));
    InitializeGame();
}

void UBullCowCartridge::InitializeGame()
{
    // Variable initializations
    Isograms = GetIsograms(Words);
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    NumberOfGuesses = 0;
    FirstEntry = true;
    Lives = HiddenWord.Len() * 2;

    // ---Debug helper line---
    //PrintLine(TEXT("The word is: %s."), *HiddenWord);
}

// Gets only the isograms from the list of hidden words
TArray<FString> UBullCowCartridge::GetIsograms(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
    for (FString WordItem : WordList)
    {
        if (IsIsogram(WordItem))
        {
            ValidWords.Emplace(WordItem);
        }
    }
    return ValidWords;
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    ClearScreen();
    if (FirstEntry)
    {
        // Informs the player on the rules of the game the first time they press enter
        PrintLine(TEXT("You are guessing a word with no repeating letters"));
        PrintLine(TEXT("You are given the number of letters in the word"));
        PrintLine(TEXT("Once you have entered a word press enter to guess"));
        PrintLine(TEXT("You will then be told how many letters you have put in the right position, called Bulls, and how many letters are correct but not in the right spot, called Cows"));
        PrintLine(TEXT("Please make your first guess, the word has %i letters in it"), HiddenWord.Len());
        FirstEntry = false;
    }

    // Checks the win condition
    else if (PlayerInput == HiddenWord && Lives > 0)
    {
        // Increments the number of guesses for each entry
        NumberOfGuesses++;

        // Win message
        PrintLine(TEXT("You guessed correctly! The word was \"") + HiddenWord + TEXT("\" and it took you ") + FString::FromInt(NumberOfGuesses) + TEXT(" guesses to get it right. Congratulations!"));
        PrintLine(TEXT("Press ENTER to play again or press ESC to exit"));
        InitializeGame();
    }

    // Game code for guessing incorrectly
    else if (Lives > 0)
    {
        // Skip condition to not have to count Bulls and Cows when the word is the wrong length or not an isogram
        if (PlayerInput.Len() == HiddenWord.Len() && IsIsogram(PlayerInput))
        {
            // Increments the number of guesses for each entry
            NumberOfGuesses++;

            FBullCowCount CountBC = GetBullsAndCows(PlayerInput);

            // Displays Bulls and Cows and prompts for a new guess
            Lives--;
            PrintLine(TEXT("Your guess was wrong. You had ") + FString::FromInt(CountBC.Bulls) + TEXT(" Bulls and ") + FString::FromInt(CountBC.Cows) + TEXT(" Cows."));
            PrintLine(TEXT("You have %i lives"), Lives);
            PrintLine(TEXT("Please enter a new word to guess again"));
        }
        // Information for incorrect guess formatting
        else
        {
            PrintLine(TEXT("Your guess was the wrong length or had repeating letters, try again please"));
        }
    }

    // Game code for running out of lives
    else
    {
        PrintLine(TEXT("You ran out of lives, sorry. The word was \"%s\"."), *HiddenWord);
        PrintLine(TEXT("Press ENTER to play again or press ESC to exit"));
        InitializeGame();
    }
}

// Checks for repeating letters in the entered word
bool UBullCowCartridge::IsIsogram(const FString& Entry) const
{
    int32 Counter;
    for (int32 i = 0; i < Entry.Len(); i++)
    {
        Counter = 0;
        for (int32 x = i + 1; x < Entry.Len(); x++)
        {
            if (Entry[i] == Entry[x])
                Counter++;
        }
        if (Counter > 0)
            return false;
    }
    return true;
}

FBullCowCount UBullCowCartridge::GetBullsAndCows(const FString& Guess) const
{
    FBullCowCount Count;

    // Counts number of Bulls and Cows, i.e. correct letters
    for (int32 i = 0; i < HiddenWord.Len(); i++)
    {
        for (int32 x = 0; x < Guess.Len(); x++)
        {
            if (HiddenWord[i] == Guess[x] && i == x)
            {
                Count.Bulls++;
                break;
            }
            else if (HiddenWord[i] == Guess[x])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}