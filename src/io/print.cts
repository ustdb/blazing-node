import native from "../loader.cjs";

export function hello(): string {
    return native.hello();
}
