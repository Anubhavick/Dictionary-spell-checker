import { DictionaryBST } from './bst';
import fs from 'fs';
import path from 'path';

// Singleton dictionary instance
let dictionaryInstance: DictionaryBST | null = null;

export function getDictionary(): DictionaryBST {
  if (!dictionaryInstance) {
    dictionaryInstance = new DictionaryBST();
    loadDictionaryFromFile();
  }
  return dictionaryInstance;
}

function loadDictionaryFromFile(): void {
  try {
    // Load from parent directory's dictionary.txt
    const dictPath = path.join(process.cwd(), '..', 'dictionary.txt');
    
    if (fs.existsSync(dictPath)) {
      const content = fs.readFileSync(dictPath, 'utf-8');
      const words = content
        .split('\n')
        .map(line => line.trim())
        .filter(line => line.length > 0);
      
      dictionaryInstance?.loadWords(words);
      console.log(`Loaded ${words.length} words from dictionary.txt`);
    } else {
      console.warn('dictionary.txt not found, starting with empty dictionary');
    }
  } catch (error) {
    console.error('Error loading dictionary:', error);
  }
}

export function saveDictionaryToFile(word: string): void {
  try {
    const dictPath = path.join(process.cwd(), '..', 'dictionary.txt');
    fs.appendFileSync(dictPath, word.toLowerCase() + '\n', 'utf-8');
  } catch (error) {
    console.error('Error saving word to dictionary:', error);
    throw error;
  }
}
