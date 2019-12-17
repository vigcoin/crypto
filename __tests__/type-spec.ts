import {
  CNCheckHash,
  CNFashHash,
  CNSlowHash,
  CryptoHash,
  CryptonoteDifficulty,
  CryptoSignature,
  HASH_LENGTH,
  IsPublicKey,
  NULL_HASH,
  SIGNATURE_LENGTH,
} from "../src/types";

test("Should have variables", () => {
  expect(HASH_LENGTH).toBe(32);
  expect(SIGNATURE_LENGTH).toBe(64);
  expect(NULL_HASH.length).toBe(32);
  expect(CNFashHash).toBeTruthy();
  expect(CNSlowHash).toBeTruthy();
  expect(CNCheckHash).toBeTruthy();
  expect(IsPublicKey).toBeTruthy();
  expect(CryptoHash).toBeTruthy();
  expect(CryptoSignature).toBeTruthy();
  expect(CryptonoteDifficulty).toBeTruthy();
});
