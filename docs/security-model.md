# Security model

## Intended protection

The planned system aims to make ordinary device navigation unavailable after an authorized emergency transition, while leaving physical DFU recovery available. This repository does not implement that device behavior and must not be relied on to secure a device or files.

## Implemented checks and limits

- Records are versioned and have a CRC-32 checksum. This detects accidental damage and torn/partial data, but CRC is not a keyed authenticator and does not stop deliberate modification.
- Two-slot selection can recover a valid prior record when the other slot is absent, truncated, or corrupt. A storage writer and actual persistent medium are not implemented.
- A corrupt or unavailable state has no implicit `unlocked` default in the record-selection API; the caller must handle failure explicitly.
- The test cleanup predicate requires explicit confirmation and an exact dedicated path. It does not itself access the filesystem or prove that a future adapter is safe.
- No screen lock, restricted menu, file-access control, encryption, key generation, key storage, or cryptographic erasure is implemented.

## Planned data protection

Any confidential-file feature must use a maintained, vetted AEAD implementation, authenticate metadata, use unique nonces, and define crash-safe replacement behavior. Keys should be generated from a cryptographically secure random source and stored separately from ciphertext; the selected hardware and backup model must be documented before claiming protection. No keys or ciphertext are currently produced.

Deleting an encryption key can make correctly encrypted data inaccessible only if no usable key copies or plaintext remain. NAND flash wear leveling and remapped pages make ordinary FAT32 formatting an unreliable secure-erasure method. DFU must not be described as wiping microSD or all internal storage.

## Attacker limits

An attacker with physical access, debug/programming access, a modified firmware image, or direct access to removable media may bypass software-only controls. A lock UI is not equivalent to secure boot or encrypted storage. The hardware bootloader remains outside the project's control.
