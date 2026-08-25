import native from '../loader.mjs';

export function printf(format: string, ...args: any[]): string {
  return native.printf(format, ...args);
}
