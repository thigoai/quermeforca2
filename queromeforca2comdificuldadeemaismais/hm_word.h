#ifndef HM_WORD_H
#define HM_WORD_H
/*!
 * Hangman Word class
 * @file hm_word.h
 *
 * This class represents a Hangman Word, being responsible
 * for receiving a word, a (optional) list of letter tha
 * must be revealed, and creating the corresponding masked
 * string for display.
 *
 * This class also receives guesses, and keeps track of how
 * many of theses guesses were correct or wrong, so that at
 * the end we may retrieve this information for score
 * (performance) calculation.
 *
 * \author Selan
 * \date April 20th, 2022
 */

#include <string>
#include <vector>

class HangmanWord {
  //=== Data members or attributes.
private:
  std::wstring m_secret_word;  //!< The secret word to guess.
  std::wstring m_masked_word;  //!< The secret word with closed letters masked.
  std::wstring m_open_letters; //!< List of letter to show at the start.
  long m_n_correct_guesses;    //!< # of correct guesses made by the player.
  std::vector<wchar_t> m_wrong_guesses; //!< List of wrong guesses made.
  wchar_t m_mask_char; //!< Char used as mask to hide letters in the secret word.

  //=== Public types
public:
  /// Return types of the guess(char) method.
  enum class guess_e : short {
    CORRECT = 0, //!< Correct guess.
    WRONG,       //!< Wrong guess.
    REPEATED     //!< This guess (wrong/correct) has been used before.
  };

  //=== Public interface
  /// Default Ctor. Assumes input strings are all lower case.
  /*!
   * @param secret The secret word.
   * @param show List of letters in the secret word to show to player.
   * @param mask Char to use as mask.
   */
  HangmanWord(std::wstring secret = L"", std::wstring show = L"", wchar_t mask = L'_');
  /// Default Dtor.
  ~HangmanWord() = default;
  /// Initialize the object providing a (new) word, show letters and mask char.
  void initialize(const std::wstring &sw, const std::wstring &ol = L"", wchar_t mch = L'_');
  /// Return the secret word with the unguessed letters masked.
  [[nodiscard]] std::wstring masked_str() const;
  /// Given a guess `g` this method returns three possible outcomes: CORRECT, WRONG or REPEATED.
  guess_e guess(wchar_t g);
  /// Return the number of **wrong** guesses so far in the match.
  [[nodiscard]] size_t wrong_guesses() const;
  /// Return the number of **correct** guesses so far in the match.
  [[nodiscard]] size_t correct_guesses() const;
  /// Return the list of wrong guesses.
  [[nodiscard]] std::vector<wchar_t> wrong_guesses_list() const;
  /// Return the number of masked letters in the secret word.
  [[nodiscard]] short n_masked_ch() const;
  /// Return the mask char.
  [[nodiscard]] wchar_t mask_char() const;
  /// Returns the secret word.
  std::wstring operator()() const;
  /// Reset the object to its original state and mask the secret word.
  void reset();
};

#endif // HM_WORD_H

