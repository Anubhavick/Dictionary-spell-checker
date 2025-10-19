import type { NextApiRequest, NextApiResponse } from 'next';
import { getDictionary } from '@/lib/dictionary';

type CheckResponse = {
  word: string;
  found: boolean;
  suggestions?: string[];
};

export default function handler(
  req: NextApiRequest,
  res: NextApiResponse<CheckResponse | { error: string }>
) {
  if (req.method !== 'POST') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const { word } = req.body;

  if (!word || typeof word !== 'string') {
    return res.status(400).json({ error: 'Word is required' });
  }

  const dictionary = getDictionary();
  const found = dictionary.search(word);

  const response: CheckResponse = {
    word: word.toLowerCase().trim(),
    found,
  };

  if (!found) {
    response.suggestions = dictionary.getSuggestions(word, 10);
  }

  res.status(200).json(response);
}
