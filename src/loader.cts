import { join } from "node:path";

export interface NativeAPI {
    hello(): string;
    printf(format: string, ...args: any[]): string;
}

function getNativeBinary(): string {
    if (process.platform !== 'linux') {
        throw new Error(`Unsupported platform: ${process.platform}-${process.arch}`);
    }

    let archDir: string;
    switch (process.arch) {
        case 'arm64':
            archDir = 'aarch64-linux-gnu';
            break;
        case 'x64':
            archDir = 'x86_64-linux-gnu';
            break;
        default:
            throw new Error(`Unsupported architecture: ${process.arch}`);
    }

    return join(__dirname, '..', '..', 'native-bin', archDir, 'blazing_node.node');
}

export default require(
    getNativeBinary(),
) as NativeAPI;
