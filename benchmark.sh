#!/bin/bash
#
# Performance Test Script
# Compares original vs optimized implementations
#

echo "═══════════════════════════════════════════════"
echo "  Dictionary Spell Checker - Performance Test"
echo "═══════════════════════════════════════════════"
echo ""

# Test words
TEST_WORDS=("apple" "banana" "computer" "programming" "optimization" "dictionary" "performance" "algorithm" "database" "network")

echo "Testing with 10 common words..."
echo ""

# Function to test API response time
test_api() {
    local word=$1
    local method=$2
    
    START=$(date +%s%N)
    RESULT=$(echo "{\"command\":\"check\",\"word\":\"$word\",\"method\":\"$method\"}" | ./dictionary_api 2>/dev/null)
    END=$(date +%s%N)
    
    ELAPSED=$(echo "scale=2; ($END - $START) / 1000000" | bc)
    
    if echo "$RESULT" | grep -q '"found":true'; then
        STATUS="✓ Found"
    else
        STATUS="✗ Not Found"
    fi
    
    # Extract timeMs from JSON
    TIME_MS=$(echo "$RESULT" | grep -o '"timeMs":[0-9.]*' | cut -d':' -f2)
    
    printf "  %-12s | %-10s | %8.2f ms (total) | %8.4f ms (search)\n" "$word" "$STATUS" "$ELAPSED" "$TIME_MS"
}

echo "┌────────────────────────────────────────────────────────────────┐"
echo "│  HASH MAP METHOD                                               │"
echo "├────────────────────────────────────────────────────────────────┤"
printf "│  %-12s | %-10s | %-14s | %-16s │\n" "Word" "Status" "Total Time" "Search Time"
echo "├────────────────────────────────────────────────────────────────┤"

for word in "${TEST_WORDS[@]}"; do
    echo -n "│  "
    test_api "$word" "hashmap"
    echo "  │"
done

echo "└────────────────────────────────────────────────────────────────┘"
echo ""

echo "┌────────────────────────────────────────────────────────────────┐"
echo "│  BST METHOD                                                    │"
echo "├────────────────────────────────────────────────────────────────┤"
printf "│  %-12s | %-10s | %-14s | %-16s │\n" "Word" "Status" "Total Time" "Search Time"
echo "├────────────────────────────────────────────────────────────────┤"

for word in "${TEST_WORDS[@]}"; do
    echo -n "│  "
    test_api "$word" "bst"
    echo "  │"
done

echo "└────────────────────────────────────────────────────────────────┘"
echo ""

echo "═══════════════════════════════════════════════════════════════"
echo "  Key Optimizations Applied:"
echo "───────────────────────────────────────────────────────────────"
echo "  ✓ FNV-1a hash function (faster than DJB2)"
echo "  ✓ Iterative BST search (no recursion overhead)"
echo "  ✓ Single dictionary load at startup"
echo "  ✓ Optimized JSON parsing"
echo "  ✓ Direct output (no buffering)"
echo "  ✓ Reduced memory allocations"
echo "═══════════════════════════════════════════════════════════════"
