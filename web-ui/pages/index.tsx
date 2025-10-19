import { useState } from 'react';
import Head from 'next/head';

type CheckResult = {
  word: string;
  found: boolean;
  suggestions?: string[];
  timeMs?: number;
  method?: string;
};

type Method = 'bst' | 'hashmap';

export default function Home() {
  const [word, setWord] = useState('');
  const [result, setResult] = useState<CheckResult | null>(null);
  const [allWords, setAllWords] = useState<string[]>([]);
  const [showAllWords, setShowAllWords] = useState(false);
  const [loading, setLoading] = useState(false);
  const [message, setMessage] = useState('');
  const [method, setMethod] = useState<Method>('hashmap');
  const [timing, setTiming] = useState<string>('');

  const checkSpelling = async () => {
    if (!word.trim()) return;
    
    setLoading(true);
    setMessage('');
    setTiming('');
    try {
      const res = await fetch('/api/check', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ word, method }),
      });
      const data = await res.json();
      setResult(data);
      if (data.timeMs !== undefined) {
        setTiming(`⏱️ Search completed in ${data.timeMs.toFixed(4)}ms using ${data.method?.toUpperCase() || method.toUpperCase()}`);
      }
    } catch (error) {
      setMessage('Error checking word');
    }
    setLoading(false);
  };

  const addWord = async () => {
    if (!word.trim()) return;
    
    setLoading(true);
    setMessage('');
    setTiming('');
    try {
      const res = await fetch('/api/add', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ word, method }),
      });
      const data = await res.json();
      setMessage(data.message);
      if (data.timeMs !== undefined) {
        setTiming(`⏱️ Operation completed in ${data.timeMs.toFixed(4)}ms using ${data.method?.toUpperCase() || method.toUpperCase()}`);
      }
      if (data.success) {
        setWord('');
        setResult(null);
      }
    } catch (error) {
      setMessage('Error adding word');
    }
    setLoading(false);
  };

  const loadAllWords = async () => {
    setLoading(true);
    setTiming('');
    try {
      const res = await fetch(`/api/list?method=${method}`);
      const data = await res.json();
      setAllWords(data.words);
      setShowAllWords(true);
      if (data.timeMs !== undefined) {
        setTiming(`⏱️ Loaded ${data.count} words in ${data.timeMs.toFixed(4)}ms using ${method.toUpperCase()}`);
      }
    } catch (error) {
      setMessage('Error loading dictionary');
    }
    setLoading(false);
  };

  return (
    <>
      <Head>
        <title>Dictionary Spell Checker</title>
        <meta name="description" content="Check spelling using Binary Search Tree" />
        <meta name="viewport" content="width=device-width, initial-scale=1" />
      </Head>

      <main className="min-h-screen bg-gradient-to-br from-blue-50 to-indigo-100 py-12 px-4">
        <div className="max-w-4xl mx-auto">
          {/* Header */}
          <div className="text-center mb-12">
            <h1 className="text-5xl font-bold text-gray-800 mb-4">
              📚 Dictionary Spell Checker
            </h1>
            <p className="text-gray-600 text-lg">
              Powered by C implementations
            </p>
          </div>

          {/* Method Selector */}
          <div className="bg-white rounded-2xl shadow-2xl p-6 mb-8">
            <div className="flex items-center justify-between">
              <div>
                <h2 className="text-xl font-bold text-gray-800 mb-2">
                  ⚙️ Data Structure Method
                </h2>
                <p className="text-gray-600 text-sm">
                  Choose between Binary Search Tree or Hash Map for performance comparison
                </p>
              </div>
              <div className="flex gap-3">
                <button
                  onClick={() => setMethod('bst')}
                  className={`px-6 py-3 rounded-lg font-semibold transition-all ${
                    method === 'bst'
                      ? 'bg-blue-500 text-white shadow-lg'
                      : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                  }`}
                >
                  🌲 BST
                </button>
                <button
                  onClick={() => setMethod('hashmap')}
                  className={`px-6 py-3 rounded-lg font-semibold transition-all ${
                    method === 'hashmap'
                      ? 'bg-blue-500 text-white shadow-lg'
                      : 'bg-gray-200 text-gray-700 hover:bg-gray-300'
                  }`}
                >
                  # Hash Map
                </button>
              </div>
            </div>
          </div>

          {/* Main Card */}
          <div className="bg-white rounded-2xl shadow-2xl p-8 mb-8">
            {/* Input Section */}
            <div className="mb-6">
              <label htmlFor="word" className="block text-gray-700 font-semibold mb-2 text-lg">
                Enter a word to check:
              </label>
              <input
                id="word"
                type="text"
                value={word}
                onChange={(e) => setWord(e.target.value)}
                onKeyPress={(e) => e.key === 'Enter' && checkSpelling()}
                className="w-full px-4 py-3 border-2 border-gray-300 rounded-lg focus:outline-none focus:border-blue-500 text-lg"
                placeholder="Type a word..."
                disabled={loading}
              />
            </div>

            {/* Action Buttons */}
            <div className="flex gap-4 mb-6">
              <button
                onClick={checkSpelling}
                disabled={loading || !word.trim()}
                className="flex-1 bg-blue-500 hover:bg-blue-600 text-white font-semibold py-3 px-6 rounded-lg disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
              >
                🔍 Check Spelling
              </button>
              <button
                onClick={addWord}
                disabled={loading || !word.trim()}
                className="flex-1 bg-green-500 hover:bg-green-600 text-white font-semibold py-3 px-6 rounded-lg disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
              >
                ➕ Add Word
              </button>
              <button
                onClick={loadAllWords}
                disabled={loading}
                className="flex-1 bg-purple-500 hover:bg-purple-600 text-white font-semibold py-3 px-6 rounded-lg disabled:opacity-50 disabled:cursor-not-allowed transition-colors"
              >
                📖 View All
              </button>
            </div>

            {/* Timing Info */}
            {timing && (
              <div className="mb-6 p-4 bg-blue-50 border-l-4 border-blue-400 text-blue-800 rounded font-mono text-sm">
                {timing}
              </div>
            )}

            {/* Message */}
            {message && (
              <div className="mb-6 p-4 bg-yellow-50 border-l-4 border-yellow-400 text-yellow-800 rounded">
                {message}
              </div>
            )}

            {/* Result Section */}
            {result && (
              <div className="mt-6 p-6 rounded-lg border-2">
                {result.found ? (
                  <div className="bg-green-50 border-green-200">
                    <p className="text-green-800 text-xl font-semibold flex items-center gap-2">
                      ✅ <span className="font-bold">&apos;{result.word}&apos;</span> is spelled correctly!
                    </p>
                  </div>
                ) : (
                  <div className="bg-red-50 border-red-200">
                    <p className="text-red-800 text-xl font-semibold mb-4 flex items-center gap-2">
                      ❌ <span className="font-bold">&apos;{result.word}&apos;</span> not found in dictionary
                    </p>
                    {result.suggestions && result.suggestions.length > 0 && (
                      <div>
                        <p className="text-gray-700 font-semibold mb-2">💡 Suggestions:</p>
                        <div className="flex flex-wrap gap-2">
                          {result.suggestions.map((suggestion, idx) => (
                            <button
                              key={idx}
                              onClick={() => setWord(suggestion)}
                              className="px-3 py-1 bg-blue-100 hover:bg-blue-200 text-blue-800 rounded-full text-sm transition-colors"
                            >
                              {suggestion}
                            </button>
                          ))}
                        </div>
                      </div>
                    )}
                  </div>
                )}
              </div>
            )}
          </div>

          {/* All Words Display */}
          {showAllWords && (
            <div className="bg-white rounded-2xl shadow-2xl p-8">
              <div className="flex justify-between items-center mb-6">
                <h2 className="text-2xl font-bold text-gray-800">
                  📖 Dictionary ({allWords.length} words)
                </h2>
                <button
                  onClick={() => setShowAllWords(false)}
                  className="text-gray-500 hover:text-gray-700 font-semibold"
                >
                  ✕ Close
                </button>
              </div>
              <div className="max-h-96 overflow-y-auto">
                <div className="grid grid-cols-2 md:grid-cols-3 lg:grid-cols-4 gap-2">
                  {allWords.map((w, idx) => (
                    <div
                      key={idx}
                      className="px-3 py-2 bg-gray-50 rounded text-gray-700 text-sm"
                    >
                      {w}
                    </div>
                  ))}
                </div>
              </div>
            </div>
          )}

          {/* Footer Info */}
          <div className="mt-8 text-center text-gray-600 text-sm">
            <p>Built with Next.js, TypeScript, and Tailwind CSS</p>
            <p className="mt-1">Backend powered by C with {method === 'bst' ? 'Binary Search Tree' : 'Hash Map (DJB2)'}</p>
            <p className="mt-1 text-xs">💡 Tip: Try switching methods to compare performance!</p>
          </div>
        </div>
      </main>
    </>
  );
}
