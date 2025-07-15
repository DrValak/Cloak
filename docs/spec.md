# Cloak Specification

## 1. Overview  
**Cloak** is a point-to-point secure messenger, written in C, designed to operate on local networks (intranet). It uses circular buffers to enqueue bytes, TCP sockets for transport, and symmetric encryption (AES-GCM or ChaCha20-Poly1305) to protect confidentiality and integrity.

---

## 2. Objectives  
1. **Confidentiality**: No one intercepting local network traffic can read the messages.  
2. **Integrity**: Any accidental or malicious alteration of messages is detected.  
3. **Efficiency**: Use ring buffers to avoid unnecessary copies and keep latency low.  
4. **Modularity**: Each component (buffer, crypto, network, app) is a separate module.  
5. **Portability**: Runs on Linux, macOS, and Windows using POSIX sockets and C99.

---

## 3. Use Cases  
- **Peer-to-peer chat** between two nodes on the same local network.  
- **Relay**: a simple server that receives from A and forwards to B.  
- **Multi-user** (future extension): N clients connected to a server, group chat.

---

## 4. Functional Requirements  
1. **FR1**: Client can send text messages (up to 256 bytes) to another client.  
2. **FR2**: Encrypted messages are packaged with header and authentication tag.  
3. **FR3**: Ring buffer handles byte-by-byte reception via socket without data loss.  
4. **FR4**: Receiver validates the authentication tag and decrypts before delivering.  
5. **FR5**: Error logs or integrity‐failure alerts are displayed for invalid packets.

---

## 5. Non-Functional Requirements  
- **NFR1**: End-to-end latency < 100 ms on a typical local network.  
- **NFR2**: Memory footprint < 1 MB per client/server instance.  
- **NFR3**: Source code documented with Doxygen comments.  
- **NFR4**: Unit tests cover ≥ 80% of code.  
- **NFR5**: Reproducible build via CMake or Makefile.

---

## 6. Module Architecture  

┌──────────────────┐ ┌────────────────────┐ ┌───────────────────┐
│ Application │──────▶│ Crypto Module │──────▶│ Ring Buffer │
│ (src/main.c) │ │ (encrypt/decrypt) │ │ (put/get bytes) │
│ │◀──────│ │◀──────│ │
└──────────────────┘ └────────────────────┘ └───────────────────┘
│ ▲
│ │
▼ │
┌──────────────────┐ │
│ Network Module │──────────────────────────┘
│ (TCP sockets) │
└──────────────────┘


- **Application**: orchestrates send/receive and provides a CLI.  
- **Crypto Module**: wraps the crypto library (libsodium/mbedTLS), offers AES-GCM or ChaCha20-Poly1305 + HMAC.  
- **Ring Buffer**: fixed-size circular buffer (e.g. 1024 B).  
- **Network Module**: encapsulates POSIX socket calls (`connect`, `bind`, `listen`, `accept`, `send`, `recv`).

---

## 7. Communication Protocol  

1. **Message format**  
[0xAA][Version=1][Type=MSG][PayloadLen (2 bytes)][Nonce (12 bytes)][Ciphertext][Tag (16 bytes)]


2. **Fields**  
- **0xAA**: sync byte (fixed header).  
- **Version**: for future compatibility.  
- **Type**: 0x01=MSG, 0x02=ACK, 0x03=ERROR.  
- **PayloadLen**: length of plaintext in bytes (big-endian).  
- **Nonce**: unique per-message IV (GCM/Poly1305).  
- **Ciphertext**: encrypted payload.  
- **Tag**: authentication tag (16 bytes).

3. **Receiver parsing flow**  
1. Read bytes until 0xAA is found.  
2. Read Version, Type, PayloadLen.  
3. Read Nonce, then Ciphertext, then Tag.  
4. Call `decrypt_and_verify()`.  
5. On success, enqueue plaintext in the output buffer.

---

## 8. Send Flow (Client)  

1. Application calls `pack_message(type, plaintext, &out_buf)`.  
2. Crypto Module generates Nonce, encrypts with the shared key.  
3. Construct the packet as per protocol.  
4. Call `ringbuffer_put_bytes(&tx_buffer, out_buf.data, out_buf.len)`.  
5. Network Module drains `tx_buffer` via `send()` on the socket.

---

## 9. Receive Flow (Client/Server)  

1. Network Module reads from socket (`recv()`), calls `ringbuffer_put_bytes(&rx_buffer, data, len)`.  
2. Internal loop:
```c
if (ringbuffer_peek_header(&rx_buffer)) {
  packet = ringbuffer_extract_packet(&rx_buffer);
  if (decrypt_and_verify(packet)) {
    deliver_to_app(packet.payload);
  } else {
    log_error("Invalid authentication tag!");
  }
}
```

## 10. Security Measures

1. Unique Nonce per message to prevent replay attacks.
2. AES-GCM or ChaCha20-Poly1305 for combined confidentiality and integrity.
3. 16-byte Tag verified automatically during decryption.
4. No dynamic allocation in the performance-critical path—use statically allocated buffers.
5. DoS protection: enforce max payload size (e.g. 256 B) and drop overly large packets.

## 11. Risks

1. Buffer overflow in the ring buffer -> Validate indices; use size_t and modulo ops.
2. Nonce reuse -> Use a secure counter or cryptographic RNG.
3. Malloc failure -> Pre-allocate fixed-size static buffers.
4. Threading bugs (future extension) -> Employ mutexes or lock free safe patterns.