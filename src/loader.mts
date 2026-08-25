import { createRequire } from "node:module";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

export interface NativeAPI {
    hello(): string;
    printf(format: string, ...args: any[]): string;
}

const nativeRequire = createRequire(import.meta.url);
const __dirname = dirname(fileURLToPath(import.meta.url));

function getNativeBinary(): string {
    // Only Linux is supported – adjust if you need other OSes
    if (process.platform !== "linux") {
        throw new Error(`Unsupported platform: ${process.platform}-${process.arch}`);
    }

    let archFolder: string;
    switch (process.arch) {
        case "arm64":
            archFolder = "aarch64-linux-gnu";
            break;
        case "x64":
            archFolder = "x86_64-linux-gnu";    // Change to your actual folder name if different
            break;
        default:
            throw new Error(`Unsupported architecture: ${process.arch}`);
    }

    return join(
        __dirname,
        "..",
        "..",
        "native-bin",
        archFolder,
        "blazing_node.node",
    );
}

export default nativeRequire(getNativeBinary()) as NativeAPI;
