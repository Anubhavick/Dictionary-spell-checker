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
  
  const command = {
    command: 'check',
    word: word.toLowerCase().trim(),
    method: dictMethod
  };

  const jsonCommand = JSON.stringify(command);

  exec(`cd ${projectRoot} && echo '${jsonCommand}' | ./dictionary_api`, 
    { timeout: 120000, maxBuffer: 1024 * 1024 }, // 2 minute timeout, 1MB buffer
    (error, stdout, stderr) => {
      if (error) {
        console.error('Error executing C API:', error);
        return res.status(500).json({ error: 'Internal server error' });
      }

      if (stderr) {
        console.error('C API stderr:', stderr);
      }

      try {
        const result = JSON.parse(stdout.trim());
        return res.status(200).json(result);
      } catch (e) {
        console.error('Error parsing C API response:', e);
        return res.status(500).json({ error: 'Invalid response from C API' });
      }
    });
}

