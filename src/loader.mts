import { createRequire } from "node:module";
import { dirname, join } from "node:path";
import { fileURLToPath } from "node:url";

export interface NativeAPI {
    hello(): string;
}

const nativeRequire = createRequire(import.meta.url);
const __dirname = dirname(fileURLToPath(import.meta.url));

function getNativeBinary(): string {
    if (
        process.platform === "linux" &&
        process.arch === "arm64"
    ) {
        return join(
            __dirname,
            "..",
            "native-bin",
            "linux-arm64",
            "blazing_node.node",
        );
    }

    throw new Error(
        `Unsupported platform: ${process.platform}-${process.arch}`,
    );
}

export default nativeRequire(
    getNativeBinary(),
) as NativeAPI;
