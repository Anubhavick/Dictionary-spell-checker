#!/bin/bash
# Download comprehensive English word list

echo " Downloading comprehensive English dictionary..."

# Option 1: dwyl/english-words (very comprehensive, ~466k words)
DICT_URL="https://raw.githubusercontent.com/dwyl/english-words/master/words_alpha.txt"

# Download to current directory
curl -L "$DICT_URL" -o dictionary-full.txt

if [ $? -eq 0 ]; then
    echo "✓ Downloaded $(wc -l < dictionary-full.txt) words to dictionary-full.txt"
    echo ""
    echo "Options:"
    echo "1. Replace current dictionary:"
    echo "   mv dictionary-full.txt dictionary.txt"
    echo ""
    echo "2. Merge with existing dictionary (remove duplicates):"
    echo "   cat dictionary.txt dictionary-full.txt | sort -u > dictionary-merged.txt"
    echo "   mv dictionary-merged.txt dictionary.txt"
    echo ""
    echo "3. Keep both files (recommended for testing)"
    echo "   # Use dictionary-full.txt for comprehensive checking"
    echo "   # Keep dictionary.txt as your personal list"
else
    echo "✗ Failed to download dictionary"
    exit 1
fi
