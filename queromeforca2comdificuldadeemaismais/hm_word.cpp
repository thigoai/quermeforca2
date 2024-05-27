/*!
 * @file hm_word.cpp
 * Hangman Word class implementation.
 * 
 * \author Selan
 * \date April 20th, 2022
 */

#include <cwctype>
#include "hm_word.h"
#include <algorithm>

HangmanWord::HangmanWord(std::wstring secret, std::wstring show, wchar_t mask)
    : m_secret_word(std::move(secret)),
      m_masked_word(m_secret_word.length(), mask), // Initialize masked word with mask character
      m_open_letters(std::move(show)),
      m_n_correct_guesses(0),
      m_mask_char(mask) {

  // Unmask letters from open_letters
  for (wchar_t c : m_open_letters) {
    auto pos = m_secret_word.find(c);
    if (pos != std::wstring::npos) {
      m_masked_word[pos] = c;
    }
  }
}

void HangmanWord::initialize(const std::wstring &sw, const std::wstring &ol, wchar_t mch) {
  m_secret_word = sw;
  m_open_letters = ol;
  m_mask_char = mch;
  m_n_correct_guesses = 0;
  m_wrong_guesses.clear();
  m_masked_word = std::wstring(m_secret_word.length(), m_mask_char);
  for (wchar_t c : m_open_letters) {
    auto pos = m_secret_word.find(c);
    if (pos != std::wstring::npos) {
      m_masked_word[pos] = c;
    }
  }
}

/*!
 * Get the masked string representing the word.
 * 
 * @return The masked string with hidden characters.
 */
std::wstring HangmanWord::masked_str() const { return m_masked_word; }

/*!
 * Make a guess on the word.
 * 
 * @param g The guessed character.
 * @return The result of the guess (CORRECT, WRONG, or REPEATED).
 */
HangmanWord::guess_e HangmanWord::guess(wchar_t g) {
  if (std::find(m_wrong_guesses.begin(), m_wrong_guesses.end(), g) !=
      m_wrong_guesses.end()) {
    return guess_e::REPEATED;
  }

  auto pos = m_secret_word.find(g);
  if (pos != std::wstring::npos) {
    // Correct guess
    m_masked_word[pos] = g;
    ++m_n_correct_guesses;
    return guess_e::CORRECT;
  } else {
    // Wrong guess
    m_wrong_guesses.push_back(g);
    return guess_e::WRONG;
  }
}

/*!
 * Get the number of wrong guesses.
 * 
 * @return The number of wrong guesses.
 */
size_t HangmanWord::wrong_guesses() const { return m_wrong_guesses.size(); }

/*!
 * Get the number of correct guesses.
 * 
 * @return The number of correct guesses.
 */
size_t HangmanWord::correct_guesses() const { return m_n_correct_guesses; }

/*!
 * Get the list of wrong guesses.
 * 
 * @return The list of wrong guesses.
 */
std::vector<wchar_t> HangmanWord::wrong_guesses_list() const { return m_wrong_guesses; }

/*!
 * Get the number of masked characters in the word.
 * 
 * @return The number of masked characters.
 */
short HangmanWord::n_masked_ch() const {
  return static_cast<short>(std::count(m_masked_word.begin(), m_masked_word.end(), m_mask_char));
}

/*!
 * Get the mask character.
 * 
 * @return The mask character.
 */
wchar_t HangmanWord::mask_char() const { return m_mask_char; }

/*!
 * Get the secret word.
 * 
 * @return The secret word.
 */
std::wstring HangmanWord::operator()() const { return m_secret_word; }

/*!
 * Reset the object to its original state and mask the secret word.
 */
void HangmanWord::reset() {
  m_n_correct_guesses = 0;
  m_wrong_guesses.clear();
  m_masked_word = std::wstring(m_secret_word.length(), m_mask_char);
  for (wchar_t c : m_open_letters) {
    auto pos = m_secret_word.find(c);
    if (pos != std::wstring::npos) {
      m_masked_word[pos] = c;
    }
  }
}

