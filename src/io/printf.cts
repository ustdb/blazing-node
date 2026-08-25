const native = require("../loader.cjs").default;

export function printf(format: string, ...args: any[]): string {
   return native.printf(format, ...args);
}  
