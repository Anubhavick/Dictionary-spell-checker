# Dictionary Spell Checker - Web UI

A modern web interface for the Dictionary Spell Checker, built with Next.js, TypeScript, and Tailwind CSS.

## Features

- 🔍 **Check spelling** - Instantly check if a word exists in the dictionary
- ➕ **Add words** - Add new words to the dictionary (persisted to file)
- 📖 **Browse dictionary** - View all words in alphabetical order
- 💡 **Smart suggestions** - Get prefix-based suggestions for misspelled words
- ⚡ **Fast BST implementation** - Binary Search Tree for O(log n) operations
- 🎨 **Beautiful UI** - Modern, responsive design with Tailwind CSS

## Setup

### Prerequisites

- Node.js 18+ 
- npm or yarn

### Installation

1. Navigate to the web-ui directory:

```bash
cd web-ui
```

2. Install dependencies:

```bash
npm install
```

### Running the App

Development mode (with hot reload):

```bash
npm run dev
```

The app will be available at `http://localhost:3000`

Production build:

```bash
npm run build
npm start
```

## How It Works

The web app uses the same Binary Search Tree (BST) algorithm as the C implementation, but written in TypeScript for the Node.js backend.

### Architecture

- **Frontend**: React components with Tailwind CSS
- **Backend**: Next.js API routes
- **Data Structure**: BST implementation in TypeScript (`lib/bst.ts`)
- **Persistence**: Reads/writes to `../dictionary.txt` (parent directory)

### API Endpoints

- `POST /api/check` - Check if a word exists, get suggestions if not
- `POST /api/add` - Add a new word to the dictionary
- `GET /api/list` - Get all words in alphabetical order

## Usage

1. **Check Spelling**: Type a word and click "Check Spelling" or press Enter
2. **Add Word**: Type a new word and click "Add Word"
3. **View All**: Click "View All" to see the entire dictionary
4. **Click Suggestions**: When a word is misspelled, click on any suggestion to check it

## Development

### Project Structure

```
web-ui/
├── lib/
│   ├── bst.ts          # BST implementation
│   └── dictionary.ts   # Dictionary loader/saver
├── pages/
│   ├── api/
│   │   ├── check.ts    # Spell check endpoint
│   │   ├── add.ts      # Add word endpoint
│   │   └── list.ts     # List all words endpoint
│   ├── _app.tsx        # App wrapper
│   └── index.tsx       # Main page
├── styles/
│   └── globals.css     # Global styles (Tailwind)
└── package.json
```

### Expanding the Dictionary

To use a larger dictionary, see the `download-dictionary.sh` script in the parent directory:

```bash
cd ..
./download-dictionary.sh
```

This downloads ~466k English words. You can then:
- Replace `dictionary.txt` with the full list, or
- Merge them together, or
- Update `lib/dictionary.ts` to load from a different file

## Notes

- The web app shares the same `dictionary.txt` file with the C terminal app
- All words are automatically lowercased
- Dictionary is loaded into memory on first API call
- New words are appended to `dictionary.txt` immediately
- The BST ensures words are always in alphabetical order

## Troubleshooting

**"Cannot find dictionary.txt"**: Make sure `dictionary.txt` exists in the parent directory (one level up from `web-ui/`)

**Port 3000 already in use**: Change the port with `npm run dev -- -p 3001`

## Future Enhancements

- Levenshtein distance for better suggestions
- Word definitions via external API
- User accounts and personal dictionaries
- Export/import dictionary
- Balanced BST (AVL/Red-Black) for guaranteed performance
