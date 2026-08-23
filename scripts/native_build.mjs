import { execFileSync } from 'node:child_process';
import { existsSync, mkdirSync, renameSync } from 'node:fs';
import { dirname, join } from 'node:path';
import { arch, platform } from 'node:process';

const target = `${platform}-${arch}`;

const source = join(
  'build',
  'Release',
  'blazing_node.node'
);

const destination = join(
  'native-bin',
  target,
  'blazing_node.node'
);

console.log(`Building native addon for ${target}`);

execFileSync(
  'cmake-js',
  ['rebuild', '--config', 'Release'],
  {
    stdio: 'inherit'
  }
);

if (!existsSync(source)) {
  throw new Error(
    `CMake.js completed, but native addon was not found: ${source}`
  );
}

mkdirSync(dirname(destination), {
  recursive: true
});

renameSync(source, destination);

console.log(`Native addon copied to: ${destination}`);
