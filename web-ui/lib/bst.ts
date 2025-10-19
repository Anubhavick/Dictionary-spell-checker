// Binary Search Tree Node
export class BSTNode {
  word: string;
  left: BSTNode | null = null;
  right: BSTNode | null = null;

  constructor(word: string) {
    this.word = word.toLowerCase();
  }
}

// BST Dictionary class
export class DictionaryBST {
  private root: BSTNode | null = null;

  // Insert a word into the BST
  insert(word: string): void {
    const lowercaseWord = word.toLowerCase().trim();
    if (!lowercaseWord) return;
    this.root = this.insertNode(this.root, lowercaseWord);
  }

  private insertNode(node: BSTNode | null, word: string): BSTNode {
    if (!node) return new BSTNode(word);
    
    const cmp = word.localeCompare(node.word);
    if (cmp === 0) return node; // already exists
    if (cmp < 0) node.left = this.insertNode(node.left, word);
    else node.right = this.insertNode(node.right, word);
    
    return node;
  }

  // Search for a word
  search(word: string): boolean {
    return this.searchNode(this.root, word.toLowerCase().trim());
  }

  private searchNode(node: BSTNode | null, word: string): boolean {
    if (!node) return false;
    
    const cmp = word.localeCompare(node.word);
    if (cmp === 0) return true;
    if (cmp < 0) return this.searchNode(node.left, word);
    return this.searchNode(node.right, word);
  }

  // Get all words in alphabetical order
  getAllWords(): string[] {
    const words: string[] = [];
    this.inorderTraversal(this.root, words);
    return words;
  }

  private inorderTraversal(node: BSTNode | null, words: string[]): void {
    if (!node) return;
    this.inorderTraversal(node.left, words);
    words.push(node.word);
    this.inorderTraversal(node.right, words);
  }

  // Get suggestions based on prefix matching
  getSuggestions(word: string, limit: number = 10): string[] {
    const suggestions: string[] = [];
    const prefix = word.toLowerCase().trim();
    this.collectPrefixMatches(this.root, prefix, suggestions, limit);
    
    // If no prefix matches, return first N words
    if (suggestions.length === 0) {
      const allWords = this.getAllWords();
      return allWords.slice(0, limit);
    }
    
    return suggestions;
  }

  private collectPrefixMatches(
    node: BSTNode | null,
    prefix: string,
    suggestions: string[],
    limit: number
  ): void {
    if (!node || suggestions.length >= limit) return;
    
    this.collectPrefixMatches(node.left, prefix, suggestions, limit);
    
    if (suggestions.length < limit && node.word.startsWith(prefix)) {
      suggestions.push(node.word);
    }
    
    this.collectPrefixMatches(node.right, prefix, suggestions, limit);
  }

  // Load words from array
  loadWords(words: string[]): void {
    words.forEach(word => this.insert(word));
  }
}
