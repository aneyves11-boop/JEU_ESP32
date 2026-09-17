#ifndef MP_SERVER_H
#define MP_SERVER_H

#include <WiFi.h>
#include <stdint.h>
#include <string.h>

// ==================================================================================
//   ESP32 ROBUST WEBSOCKET RELAY, LOBBY & 1V1 MATCHMAKING (RFC 6455)
//   100% Natif C++ - ZÉRO BIBLIOTHÈQUE EXTERNE REQUISE
// ==================================================================================

static inline uint32_t rol_sha1(uint32_t value, size_t bits) {
  return (value << bits) | (value >> (32 - bits));
}

static void sha1_calc(const uint8_t *data, size_t len, uint8_t hash[20]) {
  uint32_t h0 = 0x67452301, h1 = 0xEFCDAB89, h2 = 0x98BADCFE, h3 = 0x10325476, h4 = 0xC3D2E1F0;
  size_t total_len = ((len + 8) / 64 + 1) * 64;
  uint8_t *buf = (uint8_t*)malloc(total_len);
  if (!buf) return;
  memset(buf, 0, total_len);
  memcpy(buf, data, len);
  buf[len] = 0x80;
  uint64_t bits = (uint64_t)len * 8;
  for (int i = 0; i < 8; i++) {
    buf[total_len - 1 - i] = (uint8_t)(bits >> (i * 8));
  }
  for (size_t chunk = 0; chunk < total_len; chunk += 64) {
    uint32_t w[80];
    for (int i = 0; i < 16; i++) {
      w[i] = ((uint32_t)buf[chunk + i*4] << 24) | ((uint32_t)buf[chunk + i*4 + 1] << 16) |
             ((uint32_t)buf[chunk + i*4 + 2] << 8)  | ((uint32_t)buf[chunk + i*4 + 3]);
    }
    for (int i = 16; i < 80; i++) {
      w[i] = rol_sha1(w[i-3] ^ w[i-8] ^ w[i-14] ^ w[i-16], 1);
    }
    uint32_t a = h0, b = h1, c = h2, d = h3, e = h4;
    for (int i = 0; i < 80; i++) {
      uint32_t f, k;
      if (i < 20) { f = (b & c) | ((~b) & d); k = 0x5A827999; }
      else if (i < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
      else if (i < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
      else { f = b ^ c ^ d; k = 0xCA62C1D6; }
      uint32_t temp = rol_sha1(a, 5) + f + e + k + w[i];
      e = d; d = c; c = rol_sha1(b, 30); b = a; a = temp;
    }
    h0 += a; h1 += b; h2 += c; h3 += d; h4 += e;
  }
  free(buf);
  for (int i = 0; i < 4; i++) {
    hash[i]      = (uint8_t)(h0 >> (24 - i*8));
    hash[i + 4]  = (uint8_t)(h1 >> (24 - i*8));
    hash[i + 8]  = (uint8_t)(h2 >> (24 - i*8));
    hash[i + 12] = (uint8_t)(h3 >> (24 - i*8));
    hash[i + 16] = (uint8_t)(h4 >> (24 - i*8));
  }
}

static const char b64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static String base64_encode_bytes(const uint8_t *data, size_t len) {
  String out = "";
  out.reserve(((len + 2) / 3) * 4);
  int val = 0, valb = -6;
  for (size_t i = 0; i < len; i++) {
    val = (val << 8) + data[i];
    valb += 8;
    while (valb >= 0) {
      out += b64_chars[(val >> valb) & 0x3F];
      valb -= 6;
    }
  }
  if (valb > -6) out += b64_chars[((val << 8) >> (valb + 8)) & 0x3F];
  while (out.length() % 4) out += '=';
  return out;
}

// ----------------------------------------------------------------------------------
//   WEBSOCKET SENDER
// ----------------------------------------------------------------------------------
static void wsSendText(WiFiClient &client, const String &payload) {
  if (!client.connected()) return;
  size_t len = payload.length();
  uint8_t header[10];
  size_t headerLen = 0;

  header[0] = 0x81; // FIN + Text frame
  if (len <= 125) {
    header[1] = (uint8_t)len;
    headerLen = 2;
  } else if (len <= 65535) {
    header[1] = 126;
    header[2] = (uint8_t)(len >> 8);
    header[3] = (uint8_t)(len & 0xFF);
    headerLen = 4;
  }
  client.write(header, headerLen);
  client.write((const uint8_t*)payload.c_str(), len);
}

// ----------------------------------------------------------------------------------
//   LOBBY & MATCHMAKING STRUCTURES
// ----------------------------------------------------------------------------------
#define MAX_WS_CLIENTS 8

struct MpPeer {
  WiFiClient client;
  bool active;
  bool handshaken;
  String handshakeBuf;
  String pseudo;
  String state;      // "lobby", "waiting_game", "in_game"
  int opponentId;    // index de l'adversaire jumelé (-1 si aucun)
  int role;          // 1 (J1) ou 2 (J2)
  String game;       // nom du jeu en cours
  unsigned long lastSeen;
};

static WiFiServer mpServer(81);
static MpPeer mpPeers[MAX_WS_CLIENTS];

// Diffusion de la liste des joueurs dans le salon
static void broadcastPlayerList() {
  String json = "{\"t\":\"players\",\"list\":[";
  bool first = true;
  for (int i = 0; i < MAX_WS_CLIENTS; i++) {
    if (mpPeers[i].active && mpPeers[i].handshaken && mpPeers[i].pseudo.length() > 0) {
      if (!first) json += ",";
      json += "{\"id\":" + String(i) + ",\"name\":\"" + mpPeers[i].pseudo + "\",\"status\":\"" + mpPeers[i].state + "\"}";
      first = false;
    }
  }
  json += "]}";

  for (int i = 0; i < MAX_WS_CLIENTS; i++) {
    if (mpPeers[i].active && mpPeers[i].handshaken && mpPeers[i].state == "lobby") {
      wsSendText(mpPeers[i].client, json);
    }
  }
}

static void closePeer(int idx) {
  if (idx < 0 || idx >= MAX_WS_CLIENTS) return;
  if (mpPeers[idx].active) {
    int opp = mpPeers[idx].opponentId;
    mpPeers[idx].client.stop();
    mpPeers[idx].active = false;
    mpPeers[idx].handshaken = false;
    mpPeers[idx].handshakeBuf = "";
    mpPeers[idx].pseudo = "";
    mpPeers[idx].state = "";
    mpPeers[idx].opponentId = -1;
    mpPeers[idx].role = 0;
    mpPeers[idx].game = "";

    if (opp >= 0 && opp < MAX_WS_CLIENTS && mpPeers[opp].active) {
      wsSendText(mpPeers[opp].client, "{\"t\":\"opp_left\"}");
      mpPeers[opp].state = "lobby";
      mpPeers[opp].opponentId = -1;
      mpPeers[opp].role = 0;
      mpPeers[opp].game = "";
    }

    broadcastPlayerList();
  }
}

static bool tryCompleteHandshake(int idx) {
  String &req = mpPeers[idx].handshakeBuf;
  int endHeader = req.indexOf("\r\n\r\n");
  if (endHeader < 0) return false;

  int keyIndex = req.indexOf("Sec-WebSocket-Key: ");
  if (keyIndex < 0) {
    closePeer(idx);
    return false;
  }
  keyIndex += 19;
  int keyEnd = req.indexOf("\r\n", keyIndex);
  if (keyEnd < 0) keyEnd = endHeader;
  String key = req.substring(keyIndex, keyEnd);
  key.trim();

  String magic = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
  uint8_t hash[20];
  sha1_calc((const uint8_t*)magic.c_str(), magic.length(), hash);
  String acceptKey = base64_encode_bytes(hash, 20);

  String response = "HTTP/1.1 101 Switching Protocols\r\n";
  response += "Upgrade: websocket\r\n";
  response += "Connection: Upgrade\r\n";
  response += "Sec-WebSocket-Accept: " + acceptKey + "\r\n\r\n";

  mpPeers[idx].client.print(response);
  mpPeers[idx].handshaken = true;
  mpPeers[idx].handshakeBuf = "";
  mpPeers[idx].state = "lobby";
  mpPeers[idx].lastSeen = millis();
  Serial.printf("[MP] Socket %d handshaken avec succès !\n", idx);
  return true;
}

static void processWsMessage(int idx, const String &msg) {
  mpPeers[idx].lastSeen = millis();

  // 1. Message "hello" depuis le Salon : enregistrement du pseudo
  if (msg.indexOf("\"hello\"") >= 0) {
    int nIdx = msg.indexOf("\"name\":");
    if (nIdx >= 0) {
      int sQ = msg.indexOf("\"", nIdx + 7);
      int eQ = msg.indexOf("\"", sQ + 1);
      if (sQ >= 0 && eQ > sQ) {
        mpPeers[idx].pseudo = msg.substring(sQ + 1, eQ);
        mpPeers[idx].state = "lobby";
        wsSendText(mpPeers[idx].client, "{\"t\":\"welcome\",\"my_id\":" + String(idx) + "}");
        broadcastPlayerList();
      }
    }
    return;
  }

  // 2. Message "join" depuis une page de jeu : jumelage direct de partie
  if (msg.indexOf("\"join\"") >= 0) {
    String gameName = "pong";
    int gPos = msg.indexOf("\"g\":");
    if (gPos >= 0) {
      int sQ = msg.indexOf("\"", gPos + 4);
      int eQ = msg.indexOf("\"", sQ + 1);
      if (sQ >= 0 && eQ > sQ) gameName = msg.substring(sQ + 1, eQ);
    }
    int nPos = msg.indexOf("\"name\":");
    if (nPos >= 0) {
      int sQ = msg.indexOf("\"", nPos + 7);
      int eQ = msg.indexOf("\"", sQ + 1);
      if (sQ >= 0 && eQ > sQ) mpPeers[idx].pseudo = msg.substring(sQ + 1, eQ);
    }
    if (mpPeers[idx].pseudo.length() == 0) mpPeers[idx].pseudo = "Joueur_" + String(idx + 1);

    mpPeers[idx].game = gameName;

    // Chercher si un autre joueur attend dans le même jeu
    int existingWaiting = -1;
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
      if (i != idx && mpPeers[i].active && mpPeers[i].handshaken && mpPeers[i].game == gameName) {
        if (mpPeers[i].state == "waiting_game" && mpPeers[i].opponentId < 0) {
          existingWaiting = i;
          break;
        }
      }
    }

    if (existingWaiting >= 0) {
      // Jumeler immédiatement les deux joueurs !
      mpPeers[existingWaiting].opponentId = idx;
      mpPeers[existingWaiting].role = 1;
      mpPeers[existingWaiting].state = "in_game";

      mpPeers[idx].opponentId = existingWaiting;
      mpPeers[idx].role = 2;
      mpPeers[idx].state = "in_game";

      // Notifier le Joueur 1
      wsSendText(mpPeers[existingWaiting].client, "{\"t\":\"start\",\"role\":1,\"p\":1,\"opp\":\"" + mpPeers[idx].pseudo + "\"}");
      // Notifier le Joueur 2
      wsSendText(mpPeers[idx].client, "{\"t\":\"start\",\"role\":2,\"p\":2,\"opp\":\"" + mpPeers[existingWaiting].pseudo + "\"}");

      Serial.printf("[MP] Match %s démarré : %s (J1) vs %s (J2) !\n", gameName.c_str(), mpPeers[existingWaiting].pseudo.c_str(), mpPeers[idx].pseudo.c_str());
    } else {
      // Premier arrivé dans la room du jeu : en attente du 2ème
      mpPeers[idx].role = 1;
      mpPeers[idx].opponentId = -1;
      mpPeers[idx].state = "waiting_game";
      wsSendText(mpPeers[idx].client, "{\"t\":\"joined\",\"p\":1,\"role\":1}");
      Serial.printf("[MP] Joueur %s en attente d'un adversaire pour %s\n", mpPeers[idx].pseudo.c_str(), gameName.c_str());
    }
    broadcastPlayerList();
    return;
  }

  // 3. Défi envoyé dans le salon : {"t":"challenge","to":1,"game":"pong"}
  if (msg.indexOf("\"challenge\"") >= 0 && msg.indexOf("\"accept_challenge\"") < 0) {
    int toIdx = -1;
    int tPos = msg.indexOf("\"to\":");
    if (tPos >= 0) toIdx = msg.substring(tPos + 5).toInt();
    String game = "pong";
    int gPos = msg.indexOf("\"game\":");
    if (gPos >= 0) {
      int sQ = msg.indexOf("\"", gPos + 7);
      int eQ = msg.indexOf("\"", sQ + 1);
      if (sQ >= 0 && eQ > sQ) game = msg.substring(sQ + 1, eQ);
    }

    if (toIdx >= 0 && toIdx < MAX_WS_CLIENTS && mpPeers[toIdx].active && mpPeers[toIdx].handshaken) {
      String inv = "{\"t\":\"challenged\",\"from\":" + String(idx) + ",\"from_name\":\"" + mpPeers[idx].pseudo + "\",\"game\":\"" + game + "\"}";
      wsSendText(mpPeers[toIdx].client, inv);
    }
    return;
  }

  // 4. Défi accepté : {"t":"accept_challenge","from":0,"game":"pong"}
  if (msg.indexOf("\"accept_challenge\"") >= 0) {
    int fromIdx = -1;
    int fPos = msg.indexOf("\"from\":");
    if (fPos >= 0) fromIdx = msg.substring(fPos + 7).toInt();
    String game = "pong";
    int gPos = msg.indexOf("\"game\":");
    if (gPos >= 0) {
      int sQ = msg.indexOf("\"", gPos + 7);
      int eQ = msg.indexOf("\"", sQ + 1);
      if (sQ >= 0 && eQ > sQ) game = msg.substring(sQ + 1, eQ);
    }

    if (fromIdx >= 0 && fromIdx < MAX_WS_CLIENTS && mpPeers[fromIdx].active) {
      wsSendText(mpPeers[fromIdx].client, "{\"t\":\"launch_match\",\"game\":\"" + game + "\",\"role\":1,\"opp\":\"" + mpPeers[idx].pseudo + "\"}");
      wsSendText(mpPeers[idx].client, "{\"t\":\"launch_match\",\"game\":\"" + game + "\",\"role\":2,\"opp\":\"" + mpPeers[fromIdx].pseudo + "\"}");
    }
    return;
  }

  // 5. Défi refusé : {"t":"decline_challenge","from":0}
  if (msg.indexOf("\"decline_challenge\"") >= 0) {
    int fromIdx = -1;
    int fPos = msg.indexOf("\"from\":");
    if (fPos >= 0) fromIdx = msg.substring(fPos + 7).toInt();
    if (fromIdx >= 0 && fromIdx < MAX_WS_CLIENTS && mpPeers[fromIdx].active) {
      wsSendText(mpPeers[fromIdx].client, "{\"t\":\"declined\",\"by\":\"" + mpPeers[idx].pseudo + "\"}");
    }
    return;
  }

  // 6. En cours de jeu : relayer instantanément à l'adversaire jumelé
  int opp = mpPeers[idx].opponentId;
  if (opp >= 0 && opp < MAX_WS_CLIENTS && mpPeers[opp].active && mpPeers[opp].handshaken) {
    wsSendText(mpPeers[opp].client, msg);
  }
}

static void initMultiplayerServer() {
  mpServer.begin();
  for (int i = 0; i < MAX_WS_CLIENTS; i++) {
    mpPeers[i].active = false;
    mpPeers[i].handshaken = false;
    mpPeers[i].handshakeBuf = "";
    mpPeers[i].pseudo = "";
    mpPeers[i].state = "";
    mpPeers[i].opponentId = -1;
    mpPeers[i].role = 0;
    mpPeers[i].game = "";
    mpPeers[i].lastSeen = 0;
  }
  Serial.println("[MP] Serveur Multijoueur & Matchmaking opérationnel (port 81)");
}

static void handleMultiplayer() {
  // 1. Accepter de nouvelles connexions
  if (mpServer.hasClient()) {
    WiFiClient newClient = mpServer.available();
    int slot = -1;
    for (int i = 0; i < MAX_WS_CLIENTS; i++) {
      if (!mpPeers[i].active || !mpPeers[i].client.connected()) {
        slot = i;
        break;
      }
    }
    if (slot >= 0) {
      if (mpPeers[slot].active) closePeer(slot);
      mpPeers[slot].client = newClient;
      mpPeers[slot].active = true;
      mpPeers[slot].handshaken = false;
      mpPeers[slot].handshakeBuf = "";
      mpPeers[slot].pseudo = "";
      mpPeers[slot].state = "connecting";
      mpPeers[slot].opponentId = -1;
      mpPeers[slot].role = 0;
      mpPeers[slot].game = "";
      mpPeers[slot].lastSeen = millis();
    } else {
      newClient.stop();
    }
  }

  // 2. Traitement des flux de chaque client
  for (int i = 0; i < MAX_WS_CLIENTS; i++) {
    if (!mpPeers[i].active) continue;

    if (!mpPeers[i].client.connected()) {
      closePeer(i);
      continue;
    }

    // A. Phase d'accumulation du Handshake HTTP
    if (!mpPeers[i].handshaken) {
      while (mpPeers[i].client.available()) {
        char c = (char)mpPeers[i].client.read();
        mpPeers[i].handshakeBuf += c;
        if (mpPeers[i].handshakeBuf.endsWith("\r\n\r\n")) {
          tryCompleteHandshake(i);
          break;
        }
        if (mpPeers[i].handshakeBuf.length() > 2048) {
          closePeer(i);
          break;
        }
      }
      continue;
    }

    // B. Lecture des trames WebSocket
    while (mpPeers[i].client.available() >= 2) {
      int b0 = mpPeers[i].client.read();
      int b1 = mpPeers[i].client.read();
      if (b0 < 0 || b1 < 0) break;

      int opcode = b0 & 0x0F;
      if (opcode == 0x08) { // Frame Close
        closePeer(i);
        break;
      }

      bool masked = (b1 & 0x80) != 0;
      uint64_t pLen = b1 & 0x7F;
      if (pLen == 126) {
        while (mpPeers[i].client.available() < 2) delay(1);
        int h = mpPeers[i].client.read();
        int l = mpPeers[i].client.read();
        pLen = (h << 8) | l;
      }

      uint8_t mask[4] = {0,0,0,0};
      if (masked) {
        while (mpPeers[i].client.available() < 4) delay(1);
        for (int m = 0; m < 4; m++) mask[m] = mpPeers[i].client.read();
      }

      if (pLen > 4096) {
        closePeer(i);
        break;
      }

      char *buf = (char*)malloc(pLen + 1);
      if (buf) {
        size_t readCount = 0;
        unsigned long startWait = millis();
        while (readCount < pLen && millis() - startWait < 500) {
          if (mpPeers[i].client.available()) {
            int c = mpPeers[i].client.read();
            if (masked) c ^= mask[readCount % 4];
            buf[readCount++] = (char)c;
          }
        }
        buf[readCount] = '\0';
        String msg = String(buf);
        free(buf);

        if (readCount == pLen) {
          processWsMessage(i, msg);
        }
      }
    }
  }
}

#endif
