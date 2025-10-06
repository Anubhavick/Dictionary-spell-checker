# Dictionary Spell Checker (BST)

This is a small C program implementing a dictionary-based spell checker using a Binary Search Tree (BST).

Features
- Load words from `dictionary.txt` into a BST
- Check if a word is present (case-insensitive)
- Add new words (updates the BST and appends to `dictionary.txt`)
- Display all words in alphabetical order (in-order traversal)
- Simple suggestions: prefix matches or nearby words

Why a BST?
- BST stores words in a structure that keeps elements ordered.
- In-order traversal prints words alphabetically without extra sorting.
- Average search, insert, and delete are O(log n) for balanced trees. For an unbalanced tree (e.g., sorted input) performance can degrade to O(n).
- For production-scale spell-checkers, balanced trees (AVL/Red-Black) or tries may be preferable. This project keeps the BST simple and readable.

Build & Run
1. Build:

   make

2. Run:

   ./spellchecker

Notes
- The program expects a `dictionary.txt` file in the same directory. A sample is provided.
- The suggestion algorithm is simple: it lists words sharing the same prefix or a few nearby words. It is intentionally minimal to keep the code short and readable.

Detailed usage and examples
---------------------------

Files in this folder
- `main.c`         — the C source implementing the BST spell checker
- `Makefile`       — build rules (runs `gcc` to produce `spellchecker`)
- `dictionary.txt` — plaintext dictionary; one word per line (lowercase recommended)
- `README.md`      — this file

Build the program (macOS/Linux):

```sh
make
```

This produces the executable `spellchecker`.

Run interactively:

```sh
./spellchecker
```

Interactive menu (what to type at the program prompt):

1) Check spelling
   - After selecting `1`, type the word and press Enter.
   - Example: `Cat` → program treats it case-insensitively and checks `cat`.

2) Add new word to dictionary
   - After selecting `2`, type the new word and press Enter.
   - If the word does not already exist, the program inserts it into the in-memory BST and appends it to `dictionary.txt`.

3) Display all dictionary words (alphabetical)
   - Prints an in-order traversal of the BST; output is alphabetically sorted.

4) Exit
   - Exit the program safely. In-memory data is freed on exit.

Example session (user input lines are prefixed with `>`):

```
$ ./spellchecker
Loading dictionary from 'dictionary.txt'...
Dictionary loaded.

Dictionary Spell Checker
1) Check spelling
2) Add new word to dictionary
3) Display all dictionary words (alphabetical)
4) Exit
Enter choice: > 1
Enter word to check: > Cat
'cat' is spelled correctly.

Enter choice: > 1
Enter word to check: > Caat
'caat' is NOT found in the dictionary.
No prefix matches. Showing nearby words alphabetically:
  apple
  banana
  cat
  dog
  ...

Enter choice: > 2
Enter new word to add: > Caat
'caat' added to dictionary and saved to file.

Enter choice: > 3
Dictionary words (alphabetical):
apple
banana
caat
cat
... 

Enter choice: > 4
Exiting.
```

Important implementation notes
------------------------------
- Case handling: All words are lowercased on load and before insertion/search. The program treats `Cat` and `cat` the same.
- Dictionary file: The program appends newly added words to `dictionary.txt`. If you prefer not to modify the file, remove write permission or edit `main.c` to disable appending.
- Suggestions: Currently, suggestions are produced by finding words that start with the same prefix (up to 10 matches). If none match, the program prints up to 10 words from the dictionary as a fallback. This keeps the code simple and fast.
- Performance: For large dictionaries, an unbalanced BST can degrade to O(n) operations. If you plan to use very large dictionaries, consider replacing the BST with a balanced tree (AVL/Red-Black) or a trie for faster prefix queries.

