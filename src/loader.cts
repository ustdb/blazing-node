import { join } from "node:path";

export interface NativeAPI {
    hello(): string;
}

function getNativeBinary(): string {
    if (
        process.platform === "linux" &&
        process.arch === "arm64"
    ) {
        return join(
            __dirname,
            "..",
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

export default require(
    getNativeBinary(),
) as NativeAPI;
