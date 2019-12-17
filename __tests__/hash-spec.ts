import { Hash } from "../src/hash";
import { HASH_LENGTH } from "../src/types";

test("Should have Greeter available", () => {
  expect(
    Hash.from(Buffer.from("Hello world!")).length === HASH_LENGTH
  ).toBeTruthy();
});
