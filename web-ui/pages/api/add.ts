import type { NextApiRequest, NextApiResponse } from 'next';
import { getDictionary, saveDictionaryToFile } from '@/lib/dictionary';

type AddResponse = {
  word: string;
  success: boolean;
  message: string;
};

export default function handler(
  req: NextApiRequest,
  res: NextApiResponse<AddResponse | { error: string }>
) {
  if (req.method !== 'POST') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const { word } = req.body;

  if (!word || typeof word !== 'string') {
    return res.status(400).json({ error: 'Word is required' });
  }

  const dictionary = getDictionary();
  const cleanWord = word.toLowerCase().trim();

  // Check if already exists
  if (dictionary.search(cleanWord)) {
    return res.status(200).json({
      word: cleanWord,
      success: false,
      message: 'Word already exists in dictionary',
    });
  }

  // Add to BST
  dictionary.insert(cleanWord);

  // Save to file
  try {
    saveDictionaryToFile(cleanWord);
    res.status(200).json({
      word: cleanWord,
      success: true,
      message: 'Word added successfully',
    });
  } catch (error) {
    res.status(500).json({
      word: cleanWord,
      success: false,
      message: 'Word added to memory but failed to save to file',
    });
  }
}
