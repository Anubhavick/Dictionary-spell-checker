import type { NextApiRequest, NextApiResponse } from 'next';
import { exec } from 'child_process';
import path from 'path';

type ListResponse = {
  words: string[];
  count: number;
  timeMs?: number;
};

export default async function handler(
  req: NextApiRequest,
  res: NextApiResponse<ListResponse | { error: string }>
) {
  if (req.method !== 'GET') {
    return res.status(405).json({ error: 'Method not allowed' });
  }

  const { method = 'bst' } = req.query;
  const dictMethod = method === 'hashmap' ? 'hashmap' : 'bst';
  const projectRoot = path.join(process.cwd(), '..');

  const startTime = Date.now();

  exec(`cd ${projectRoot} && ./${dictMethod} list`, 
    { timeout: 60000, maxBuffer: 1024 * 1024 * 10 }, 
    (error, stdout, stderr) => {
      const timeMs = Date.now() - startTime;

      if (error) {
        console.error('Error executing C program:', error);
        return res.status(500).json({ error: 'Internal server error' });
      }

      if (stderr) {
        console.error('C program stderr:', stderr);
      }

      const words = stdout.trim().split('\n').filter(w => w.length > 0);

      return res.status(200).json({
        words: words,
        count: words.length,
        timeMs: timeMs
      });
    });
}
