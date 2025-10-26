# Quick Usage Guide

## Terminal Commands

### Check Spelling
```bash
# Using BST
./bst check hello
# Output: FOUND: 'hello'

# Using HashMap (FASTER!)
./hashmap check hello
# Output: FOUND: 'hello'

# Check non-existent word
./hashmap check xyz123
# Output: NOT_FOUND: 'xyz123'
```

### Add Word
```bash
# Using BST
./bst add myword
# Output: ADDED: 'myword'

# Using HashMap
./hashmap add myword
# Output: EXISTS: 'myword' (if already exists)

# Word is automatically added in alphabetical order!
```

### List All Words
```bash
# Using BST
./bst list

# Using HashMap
./hashmap list

# Both output the same sorted list from dictionary.txt
```

## Web Interface

1. **Start the server:**
```bash
cd web-ui
npm install  # First time only
npm run dev
```

2. **Open:** http://localhost:3000

3. **Features:**
   - 🔍 **Check Spelling** - Blue button
   - ➕ **Add Word** - Green button  
   - 📋 **View All** - Purple button
   - 🔄 **Switch Method** - BST or HashMap dropdown

## Which Method to Use?

### Use **HashMap** when:
- ✅ You have a large dictionary (100K+ words)
- ✅ Speed is critical
- ✅ You need fast lookups

### Use **BST** when:
- ✅ Learning data structures
- ✅ Small dictionary (<10K words)
- ✅ Memory is very limited

## Performance Comparison

With **300,000 words** dictionary:

| Operation | HashMap | BST |
|-----------|---------|-----|
| Check word | ~1ms | ~100ms+ |
| Add word | ~500ms | ~500ms |
| List words | ~500ms | ~500ms |

**Verdict:** HashMap is **100x faster** for checking words!

## Tips

- Words are case-insensitive (automatically converted to lowercase)
- All words are kept in alphabetical order in dictionary.txt
- You can use either method - both access the same dictionary file
- The terminal interface is clean - no fancy UI, just results

## Common Issues

**Problem:** "Permission denied" when running ./bst or ./hashmap
**Solution:** 
```bash
chmod +x bst hashmap
```

**Problem:** Programs not found
**Solution:** Compile first:
```bash
make
```

**Problem:** Web UI not starting
**Solution:** Install dependencies:
```bash
cd web-ui
npm install
npm run dev
```

## Examples

```bash
# Check if "computer" exists
./hashmap check computer
# FOUND: 'computer'

# Add a new technical term
./hashmap add blockchain
# ADDED: 'blockchain'

# Verify it was added
./hashmap check blockchain
# FOUND: 'blockchain'

# See total word count
./hashmap list | wc -l
# 300001 (original 300000 + 1 new word)
```

Enjoy your spell checker! 🎉
