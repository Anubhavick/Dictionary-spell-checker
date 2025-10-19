import type { NextApiRequest, NextApiResponse } from 'next';
import { getDictionary } from '@/lib/dictionary';

type ListResponse = {
  words: string[];
  count: number;
};

export default function handler(
  req: NextApiRequest,
  res: NextApiResponse<ListResponse | { error: string }>
) {
  if (req.method !== 'GET') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const dictionary = getDictionary();
  const words = dictionary.getAllWords();

  res.status(200).json({
    words,
    count: words.length,
  });
}
