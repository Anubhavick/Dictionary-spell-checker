# Project Structure

```
Dictionary-spell-checker/
├── .git/                       # Git repository
├── .gitignore                  # Git ignore file
├── Makefile                    # Build configuration
├── README.md                   # Project documentation
├── OPTIMIZATIONS.md            # Optimization details
│
├── main.c                      # Terminal application (BST-based)
├── dictionary_api.c            # Web API backend (BST + HashMap)
├── hashmap.c                   # Hash map implementation
├── hashmap.h                   # Hash map header
├── compare_methods.c           # Performance comparison tool
│
├── dictionary.txt              # Word dictionary (370K+ words)
├── download-dictionary.sh      # Script to download dictionary
├── benchmark.sh                # Performance testing script
│
└── web-ui/                     # Next.js web application
    ├── package.json
    ├── tsconfig.json
    ├── next.config.js
    ├── tailwind.config.js
    ├── postcss.config.js
    ├── pages/
    │   ├── index.tsx           # Main UI page
    │   ├── _app.tsx
    │   └── api/
    │       ├── check.ts        # Spell check endpoint
    │       ├── add.ts          # Add word endpoint
    │       └── list.ts         # List words endpoint
    ├── styles/
    │   └── globals.css
    └── lib/
```

## File Purposes

### Source Files (C)
- **main.c**: Standalone terminal spell checker with BST
- **dictionary_api.c**: JSON API wrapper for web integration (supports both BST and HashMap)
- **hashmap.c/h**: Optimized hash map with FNV-1a hashing
- **compare_methods.c**: Benchmark tool to compare BST vs HashMap performance

### Build Files
- **Makefile**: Compiles `spellchecker`, `dictionary_api`, and `compare_methods`

### Scripts
- **download-dictionary.sh**: Downloads 466K words from GitHub
- **benchmark.sh**: Performance testing script

### Documentation
- **README.md**: Complete project documentation
- **OPTIMIZATIONS.md**: Detailed optimization explanations

### Data
- **dictionary.txt**: 370,105 English words (4.2 MB)

### Generated Files (Not in Git)
- `spellchecker` - Terminal app binary
- `dictionary_api` - API backend binary
- `compare_methods` - Benchmark tool binary
- `.DS_Store` - macOS system file

## Build Commands

```bash
# Build all
make

# Build specific target
make spellchecker
make dictionary_api
make compare_methods

# Clean binaries
make clean

# Rebuild everything
make clean && make
```

## Clean Codebase Principles

1. ✅ No duplicate source files
2. ✅ No backup files committed to git
3. ✅ Binaries excluded from repository
4. ✅ Proper .gitignore configuration
5. ✅ Clear file naming conventions
6. ✅ Organized directory structure
7. ✅ Documentation for every component
