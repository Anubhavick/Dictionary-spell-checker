import type { NextApiRequest, NextApiResponse } from 'next';
import { exec } from 'child_process';
import path from 'path';

type CheckResponse = {
  word: string;
  found: boolean;
  suggestions?: string[];
  timeMs: number;
  method: string;
};

export default async function handler(
  req: NextApiRequest,
  res: NextApiResponse<CheckResponse | { error: string }>
) {
  if (req.method !== 'POST') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const { word, method = 'bst' } = req.body;

  if (!word || typeof word !== 'string') {
    return res.status(400).json({ error: 'Word is required' });
  }

  const dictMethod = method === 'hashmap' ? 'hashmap' : 'bst';
  const projectRoot = path.join(process.cwd(), '..');
  const cleanWord = word.toLowerCase().trim();

  const startTime = Date.now();

  exec(`cd ${projectRoot} && ./${dictMethod} check "${cleanWord}"`, 
    { timeout: 30000, maxBuffer: 1024 * 1024 },
    (error, stdout, stderr) => {
      const timeMs = Date.now() - startTime;

      if (error) {
        console.error('Error executing C program:', error);
        return res.status(500).json({ error: 'Internal server error' });
      }

      if (stderr) {
        console.error('C program stderr:', stderr);
      }

      const output = stdout.trim();
      const found = output.startsWith('FOUND');

      return res.status(200).json({
        word: cleanWord,
        found: found,
        timeMs: timeMs,
        method: dictMethod
      });
    });
}

