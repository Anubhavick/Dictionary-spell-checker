# Dictionary Spell Checker (BST)

A comprehensive spell-checking application with both **terminal (C)** and **web (Next.js)** interfaces, implementing a Binary Search Tree (BST) for efficient word lookup.

## 🎯 Two Ways to Use

### 1. 🖥️ Terminal App (C)
Beautiful command-line interface with colors and emojis

### 2. 🌐 Web App (Next.js)
Modern browser-based UI at `localhost:3000`

---

---

## ✨ Features

Both interfaces share these core features:
- ✅ Check if a word is spelled correctly (case-insensitive)
- ➕ Add new words to the dictionary
- 📖 Display all words in alphabetical order (in-order BST traversal)
- 💡 Smart suggestions for misspelled words (prefix matching)
- 💾 Persistent storage in `dictionary.txt`

### Terminal App Extras
- 🎨 Colorful ANSI output with emojis
- 📦 Lightweight and fast

### Web App Extras  
- 🎨 Modern, responsive UI with Tailwind CSS
- 🖱️ Click suggestions to quickly check them
- 📱 Works on mobile devices

---

## 🚀 Quick Start

### Terminal App

Build and run the C program:

```bash
make
./spellchecker
```

### Web App

Navigate to `web-ui` and start the Next.js dev server:

```bash
cd web-ui
npm install
npm run dev
```

Then open `http://localhost:3000` in your browser.

---

## 📚 ## 📚 Why a BST?

- **Ordered structure**: BST keeps words in sorted order automatically
- **In-order traversal**: Prints words alphabetically without additional sorting
- **Efficient operations**: Average O(log n) for search, insert, and delete (balanced tree)
- **Simple implementation**: Easy to understand and maintain

⚠️ **Note**: For very large dictionaries, consider balanced trees (AVL/Red-Black) or tries for guaranteed O(log n) performance.

---

## 🖥️ Terminal App Usage

### Build

```bash
make
```

This compiles `main.c` into the `spellchecker` executable.

### Run

```bash
./spellchecker
```

### Interactive Menu

The terminal app presents a beautiful menu with colors and emojis:

```
╔════════════════════════════════════════╗
║   📚 Dictionary Spell Checker          ║
╚════════════════════════════════════════╝

1) 🔍 Check spelling
2) ➕ Add new word to dictionary  
3) 📖 Display all dictionary words
4) 🚪 Exit
```

**Option 1: Check Spelling**
- Enter a word (e.g., `Cat`)
- Case-insensitive matching
- Shows ✅ if correct or ❌ with suggestions if not found

**Option 2: Add Word**
- Enter a new word
- Adds to in-memory BST and appends to `dictionary.txt`

**Option 3: Display All**
- Shows all words in alphabetical order
- Color-coded output

**Option 4: Exit**
- Safely exits and frees memory

### Example Session

```bash
$ ./spellchecker

Enter choice: 1
Enter word to check: hello
✓ 'hello' is spelled correctly! ✓

Enter choice: 1  
Enter word to check: helo
✗ 'helo' is NOT found in the dictionary.

💡 Suggestions:
  → hello
  → help
  → held

Enter choice: 2
Enter new word to add: helo
✓ 'helo' added to dictionary and saved to file! ✓

Enter choice: 4
👋 Goodbye! Exiting...
```

---

## 🌐 Web App Usage

### Setup

```bash
cd web-ui
npm install
```

### Run Development Server

```bash
npm run dev
```

Open `http://localhost:3000` in your browser.

### Production Build

```bash
npm run build
npm start
```

### Using the Web Interface

1. **Check Spelling**: Type a word in the input field and click "🔍 Check Spelling" (or press Enter)
2. **Add Word**: Click "➕ Add Word" to add the current word to the dictionary
3. **View All**: Click "📖 View All" to see the entire dictionary in a modal
4. **Click Suggestions**: When a word is misspelled, click any suggestion chip to check it

The web app has a modern, gradient design with:
- Real-time feedback
- Color-coded results (green for correct, red for not found)
- Clickable suggestion chips
- Responsive layout for mobile/desktop

---

## 📁 Project Structure

```
Dictionary-spell-checker/
├── main.c                    # C implementation with BST
├── Makefile                  # Build configuration
├── dictionary.txt            # Word list (shared by both apps)
├── download-dictionary.sh    # Script to get comprehensive word list
├── README.md                 # This file
└── web-ui/                   # Next.js web application
    ├── lib/
    │   ├── bst.ts           # TypeScript BST implementation
    │   └── dictionary.ts    # Dictionary loader/saver
    ├── pages/
    │   ├── api/
    │   │   ├── check.ts     # Spell check endpoint
    │   │   ├── add.ts       # Add word endpoint
    │   │   └── list.ts      # List words endpoint
    │   ├── _app.tsx         # App wrapper
    │   └── index.tsx        # Main UI page
    ├── styles/
    │   └── globals.css      # Tailwind CSS
    ├── package.json
    └── README.md            # Web app specific docs
```

---

## 📚 Expanding the Dictionary

The default `dictionary.txt` has only ~28 words for testing. To use a comprehensive English dictionary:

### Download Full Word List

```bash
./download-dictionary.sh
```

This downloads ~**466,000 English words** from [dwyl/english-words](https://github.com/dwyl/english-words).

### Integration Options

**Option 1: Replace**
```bash
mv dictionary-full.txt dictionary.txt
```

**Option 2: Merge**
```bash
cat dictionary.txt dictionary-full.txt | sort -u > dictionary-merged.txt
mv dictionary-merged.txt dictionary.txt
```

**Option 3: Keep both** (recommended)

---

## 🎨 Implementation Details

### Case Handling
All words are lowercased automatically. `Cat` and `cat` are treated the same.

### Dictionary File
- Both apps read from `dictionary.txt`
- New words are appended when added
- Changes persist across sessions

### Suggestions Algorithm
1. Find words with matching prefix (up to 10)
2. If no prefix matches, show first 10 words alphabetically
3. Simple and fast for most use cases

### Performance Notes
- **Average case**: O(log n) for balanced BST
- **Worst case**: O(n) for unbalanced BST (sorted input)
- **Large dictionaries**: Consider AVL/Red-Black tree for guaranteed O(log n)

---

## 🚀 Future Enhancements

**Suggestions:**
- Levenshtein distance for smarter corrections
- Phonetic matching (Soundex, Metaphone)

**Performance:**
- Balanced BST (AVL/Red-Black tree)
- Trie data structure for prefix queries

**Features:**
- Word definitions via external API
- Multiple language support
- User authentication and personal dictionaries
- Import/export functionality

---

## 🛠️ Development

### Clean Build

```bash
make clean
make
```

### Testing

Terminal app:
```bash
echo -e "1\ntest\n4" | ./spellchecker
```

Web app:
```bash
cd web-ui
npm run build
npm test  # if tests are added
```

---

## 📝 License

This project is open source and available for educational purposes.

---

## 🤝 Contributing

Feel free to fork, modify, and improve! Suggested improvements:
- Add unit tests
- Implement Levenshtein distance
- Create mobile app version
- Add word frequency analysis

---

**Happy spell checking! 📚✨**

