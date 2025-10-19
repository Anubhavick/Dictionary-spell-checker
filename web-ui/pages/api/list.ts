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
  
  const command = {
    command: 'list',
    method: dictMethod
  };

  const jsonCommand = JSON.stringify(command);

  exec(`cd ${projectRoot} && echo '${jsonCommand}' | ./dictionary_api`, { maxBuffer: 1024 * 1024 * 10 }, (error, stdout, stderr) => {
    if (error) {
      console.error('Error executing C API:', error);
      return res.status(500).json({ error: 'Internal server error' });
    }

    if (stderr) {
      console.error('C API stderr:', stderr);
    }

    try {
      const result = JSON.parse(stdout.trim());
      res.status(200).json(result);
    } catch (e) {
      console.error('Error parsing C API response:', e);
      res.status(500).json({ error: 'Invalid response from C API' });
    }
  });
}
