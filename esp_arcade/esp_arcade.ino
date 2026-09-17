/*
 * ==================================================================================
 *   ESP32 WEB ARCADE - 50 VRAIS JEUX D'ARCADE EMBARQUÉS (100% PROGMEM OFFLINE)
 * ==================================================================================
 * 
 * 50 JEUX INDÉPENDANTS & DÉDIÉS :
 *  1. Snake Retro             26. Blackjack 21 Pro
 *  2. Flappy Drone            27. Memory Flip 16
 *  3. Space Invaders          28. Pierre-Feuille-Ciseaux
 *  4. Cyber Racer             29. Plus ou Moins (High-Low)
 *  5. Casse-Briques           30. Bataille Navale Mini
 *  6. 2048 Neon Puzzle        31. Tirs au But Penalty
 *  7. Neon Piano Tiles        32. Basket Dunk Shoot
 *  8. Tower Stacker           33. Sprint 100m Dash
 *  9. Knife Master            34. Ski Slalom
 * 10. Démineur Néon           35. Tir à l'Arc Bullseye
 * 11. Morpion Cyber IA        36. Air Hockey Flash
 * 12. Pong vs IA Pro          37. Beat Tap Reflex
 * 13. Asteroids Blaster       38. Simon Musical 4-Notes
 * 14. Missile Defense         39. Note Drop Rain
 * 15. Cyber Turret 360        40. Tempo Tapper 60 BPM
 * 16. Galaxian Dive           41. Guitar Hero Mini
 * 17. Tank 2D Battle          42. Fil Électrique Buzz
 * 18. Alien Swarm Strike      43. Perfect Slice
 * 19. Lights Out Matrix       44. Coin in the Jar
 * 20. Slide 15 Puzzle         45. Grappin Harpoon
 * 21. Color Flood             46. Balance Marble
 * 22. Match-3 Gem Blitz       47. Cyber Tetris
 * 23. Code Breaker            48. Cyber Pac-Maze
 * 24. Mini Sudoku 4x4         49. Lunar Lander
 * 25. Puissance 4 Cyber       50. Cannon Castle Blaster
 * ==================================================================================
 */

#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <Preferences.h>

#include "login_page.h"
#include "hub_page.h"
#include "settings_page.h"

// 12 Original Game Headers
#include "game_snake.h"
#include "game_flappy.h"
#include "game_invaders.h"
#include "game_racer.h"
#include "game_breakout.h"
#include "game_puzzle2048.h"
#include "game_pianotiles.h"
#include "game_stacker.h"
#include "game_knife.h"
#include "game_mines.h"
#include "game_tictactoe.h"
#include "game_pong.h"

// 7 Themed Game Pack Headers (Games 13 to 50)
#include "game_pack_action.h"
#include "game_pack_puzzle.h"
#include "game_pack_board.h"
#include "game_pack_sports.h"
#include "game_pack_rhythm.h"
#include "game_pack_precision.h"
#include "game_pack_retro.h"

// Mode Multijoueur 1v1 (Serveur WebSocket & Pack 7 Jeux)
#include "mp_server.h"
#include "game_pack_multiplayer.h"

Preferences preferences;

String access_pass = "arcade123";
String ap_ssid     = "ESP32-Arcade";
String ap_pass     = "";
String sta_ssid    = "";
String sta_pass    = "";

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);

DNSServer dnsServer;
WebServer server(80);

#define MAX_SESSIONS 8
struct Session {
  IPAddress ip;
  bool authenticated;
  unsigned long lastSeen;
};
Session sessions[MAX_SESSIONS];

int getSessionIndex(IPAddress ip) {
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].ip == ip) return i;
  }
  for (int i = 0; i < MAX_SESSIONS; i++) {
    if (sessions[i].ip == IPAddress(0,0,0,0)) {
      sessions[i].ip = ip;
      sessions[i].authenticated = false;
      sessions[i].lastSeen = millis();
      return i;
    }
  }
  int oldest = 0;
  for (int i = 1; i < MAX_SESSIONS; i++) {
    if (sessions[i].lastSeen < sessions[oldest].lastSeen) oldest = i;
  }
  sessions[oldest].ip = ip;
  sessions[oldest].authenticated = false;
  sessions[oldest].lastSeen = millis();
  return oldest;
}

bool isClientAuthenticated() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("arcade_auth=1") >= 0) return true;
  }
  int idx = getSessionIndex(server.client().remoteIP());
  return sessions[idx].authenticated;
}

void setClientAuthenticated(bool auth) {
  int idx = getSessionIndex(server.client().remoteIP());
  sessions[idx].authenticated = auth;
  sessions[idx].lastSeen = millis();
}

void handleLoginPage(bool showError = false) {
  String html = LOGIN_HTML;
  html.replace("%ERROR_DISPLAY%", showError ? "block" : "none");
  server.send(200, "text/html; charset=utf-8", html);
}

void handleLogin() {
  if (server.hasArg("password")) {
    String inputPass = server.arg("password");
    if (inputPass == access_pass) {
      setClientAuthenticated(true);
      server.sendHeader("Set-Cookie", "arcade_auth=1; Path=/; Max-Age=86400");
      server.sendHeader("Location", "/hub", true);
      server.send(302, "text/plain", "");
      Serial.println("[AUTH] Connexion réussie !");
      return;
    }
  }
  Serial.println("[AUTH] Mot de passe incorrect.");
  handleLoginPage(true);
}

void handleLogout() {
  setClientAuthenticated(false);
  server.sendHeader("Set-Cookie", "arcade_auth=0; Path=/; Max-Age=0");
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void handleRoot() {
  if (!isClientAuthenticated()) {
    handleLoginPage(false);
    return;
  }
  server.send(200, "text/html; charset=utf-8", HUB_HTML);
}

// Helper macro for game route
#define ROUTE_GAME(path, html_var) \
  server.on(path, HTTP_GET, []() { \
    if (!isClientAuthenticated()) { handleLoginPage(false); return; } \
    server.send(200, "text/html; charset=utf-8", html_var); \
  });

// Settings Handlers
void handleSettings() {
  if (!isClientAuthenticated()) { handleLoginPage(false); return; }
  
  uint32_t flashTotal = ESP.getFlashChipSize() / 1024;
  uint32_t flashUsed = ESP.getSketchSize() / 1024;
  uint32_t flashFree = ESP.getFreeSketchSpace() / 1024;
  uint32_t flashPct = (flashUsed + flashFree > 0) ? ((flashUsed * 100) / (flashUsed + flashFree)) : 0;
  uint32_t ramFree = ESP.getFreeHeap() / 1024;

  String html = SETTINGS_HTML;
  html.replace("%FLASH_TOTAL%", String(flashTotal));
  html.replace("%FLASH_USED%", String(flashUsed));
  html.replace("%FLASH_FREE%", String(flashFree));
  html.replace("%FLASH_PCT%", String(flashPct));
  html.replace("%RAM_FREE%", String(ramFree));

  html.replace("%ACCESS_PASS%", access_pass);
  html.replace("%STA_SSID%", sta_ssid);
  html.replace("%STA_PASS%", sta_pass);
  html.replace("%AP_SSID%", ap_ssid);
  html.replace("%AP_PASS%", ap_pass);
  server.send(200, "text/html; charset=utf-8", html);
}

void handleSaveSettings() {
  if (!isClientAuthenticated()) { handleLoginPage(false); return; }
  if (server.hasArg("access_pass") && server.arg("access_pass").length() >= 4) access_pass = server.arg("access_pass");
  if (server.hasArg("sta_ssid")) sta_ssid = server.arg("sta_ssid");
  if (server.hasArg("sta_pass")) sta_pass = server.arg("sta_pass");
  if (server.hasArg("ap_ssid") && server.arg("ap_ssid").length() > 0) ap_ssid = server.arg("ap_ssid");
  if (server.hasArg("ap_pass")) ap_pass = server.arg("ap_pass");

  preferences.begin("arcade", false);
  preferences.putString("access_pass", access_pass);
  preferences.putString("sta_ssid", sta_ssid);
  preferences.putString("sta_pass", sta_pass);
  preferences.putString("ap_ssid", ap_ssid);
  preferences.putString("ap_pass", ap_pass);
  preferences.end();

  String newPseudo = "";
  if (server.hasArg("player_pseudo")) {
    newPseudo = server.arg("player_pseudo");
    newPseudo.trim();
  }

  String msg = "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='3;url=/hub'></head>";
  msg += "<body style='background:#0d0f18;color:#00f3ff;font-family:sans-serif;text-align:center;padding:40px;'>";
  msg += "<h2>✅ Paramètres enregistrés !</h2>";
  if (newPseudo.length() > 0) {
    msg += "<p style='color:#00ff66;font-size:1.1rem;margin:12px 0;'>Pseudo joueur : <b>" + newPseudo + "</b></p>";
    msg += "<script>localStorage.setItem('arcade_pseudo', '" + newPseudo + "');</script>";
  }
  msg += "<p style='color:#fff;'>Mot de passe d'accès : <b>" + access_pass + "</b></p>";
  msg += "<p style='color:#8b9bb4;font-size:0.85rem;margin-top:16px;'>Redirection automatique vers le salon...</p>";
  msg += "</body></html>";
  server.send(200, "text/html; charset=utf-8", msg);
}


void handleNotFound() {
  String uri = server.uri();
  if (uri.indexOf("generate_204") >= 0 || uri.indexOf("hotspot-detect.html") >= 0 || uri.indexOf("canonical.html") >= 0 || uri.indexOf("connecttest.txt") >= 0 || uri.indexOf("ncsi.txt") >= 0) {
    server.sendHeader("Location", String("http://") + apIP.toString() + "/", true);
    server.send(302, "text/plain", "");
    return;
  }
  if (!isClientAuthenticated()) {
    handleLoginPage(false);
  } else {
    server.send(200, "text/html; charset=utf-8", HUB_HTML);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\n========================================");
  Serial.println("     ESP32 50-IN-1 WEB ARCADE SYSTEM    ");
  Serial.println("========================================");

  preferences.begin("arcade", true);
  access_pass = preferences.getString("access_pass", "arcade123");
  ap_ssid     = preferences.getString("ap_ssid", "ESP32-Arcade");
  ap_pass     = preferences.getString("ap_pass", "");
  sta_ssid    = preferences.getString("sta_ssid", "");
  sta_pass    = preferences.getString("sta_pass", "");
  preferences.end();

  for (int i = 0; i < MAX_SESSIONS; i++) {
    sessions[i].ip = IPAddress(0,0,0,0);
    sessions[i].authenticated = false;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  if (ap_pass.length() >= 8) {
    WiFi.softAP(ap_ssid.c_str(), ap_pass.c_str());
  } else {
    WiFi.softAP(ap_ssid.c_str());
  }

  Serial.printf("[OK] Point d'Accès Wi-Fi : %s (IP: http://%s)\n", ap_ssid.c_str(), apIP.toString().c_str());
  Serial.printf("[OK] Mot de passe d'accès aux 50 jeux : %s\n", access_pass.c_str());

  if (sta_ssid.length() > 0) {
    WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
  }

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(DNS_PORT, "*", apIP);

  const char * headerkeys[] = {"Cookie"};
  server.collectHeaders(headerkeys, 1);

  // Main Routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/index.html", HTTP_GET, handleRoot);
  server.on("/hub", HTTP_GET, handleRoot);
  server.on("/login", HTTP_POST, handleLogin);
  server.on("/logout", HTTP_GET, handleLogout);

  // --- 50 STANDALONE GAME ROUTES ---
  // 1-12
  ROUTE_GAME("/game_snake", GAME_SNAKE_HTML);
  ROUTE_GAME("/game_flappy", GAME_FLAPPY_HTML);
  ROUTE_GAME("/game_invaders", GAME_INVADERS_HTML);
  ROUTE_GAME("/game_racer", GAME_RACER_HTML);
  ROUTE_GAME("/game_breakout", GAME_BREAKOUT_HTML);
  ROUTE_GAME("/game_2048", GAME_PUZZLE2048_HTML);
  ROUTE_GAME("/game_pianotiles", GAME_PIANOTILES_HTML);
  ROUTE_GAME("/game_stacker", GAME_STACKER_HTML);
  ROUTE_GAME("/game_knife", GAME_KNIFE_HTML);
  ROUTE_GAME("/game_mines", GAME_MINES_HTML);
  ROUTE_GAME("/game_tictactoe", GAME_TICTACTOE_HTML);
  ROUTE_GAME("/game_pong", GAME_PONG_HTML);

  // 13-18 Action Pack
  ROUTE_GAME("/game_asteroids", GAME_ASTEROIDS_HTML);
  ROUTE_GAME("/game_missile", GAME_MISSILE_HTML);
  ROUTE_GAME("/game_turret", GAME_TURRET_HTML);
  ROUTE_GAME("/game_galaxian", GAME_GALAXIAN_HTML);
  ROUTE_GAME("/game_tank", GAME_TANK_HTML);
  ROUTE_GAME("/game_swarm", GAME_SWARM_HTML);

  // 19-24 Puzzle Pack
  ROUTE_GAME("/game_lightsout", GAME_LIGHTSOUT_HTML);
  ROUTE_GAME("/game_slide15", GAME_SLIDE15_HTML);
  ROUTE_GAME("/game_colorflood", GAME_COLORFLOOD_HTML);
  ROUTE_GAME("/game_match3", GAME_MATCH3_HTML);
  ROUTE_GAME("/game_codebreaker", GAME_CODEBREAKER_HTML);
  ROUTE_GAME("/game_sudoku", GAME_SUDOKU_HTML);

  // 25-30 Board Pack
  ROUTE_GAME("/game_connect4", GAME_CONNECT4_HTML);
  ROUTE_GAME("/game_blackjack", GAME_BLACKJACK_HTML);
  ROUTE_GAME("/game_memcards", GAME_MEMCARDS_HTML);
  ROUTE_GAME("/game_pfc", GAME_PFC_HTML);
  ROUTE_GAME("/game_highlow", GAME_HIGHLOW_HTML);
  ROUTE_GAME("/game_battleship", GAME_BATTLESHIP_HTML);

  // 31-36 Sports Pack
  ROUTE_GAME("/game_penalty", GAME_PENALTY_HTML);
  ROUTE_GAME("/game_basket", GAME_BASKET_HTML);
  ROUTE_GAME("/game_dash", GAME_DASH_HTML);
  ROUTE_GAME("/game_ski", GAME_SKI_HTML);
  ROUTE_GAME("/game_archery", GAME_ARCHERY_HTML);
  ROUTE_GAME("/game_airhockey", GAME_AIRHOCKEY_HTML);

  // 37-41 Rhythm Pack
  ROUTE_GAME("/game_beattap", GAME_BEATTAP_HTML);
  ROUTE_GAME("/game_simon", GAME_SIMON_HTML);
  ROUTE_GAME("/game_notedrop", GAME_NOTEDROP_HTML);
  ROUTE_GAME("/game_tempo", GAME_TEMPO_HTML);
  ROUTE_GAME("/game_guitar", GAME_GUITAR_HTML);

  // 42-46 Precision Pack
  ROUTE_GAME("/game_wire", GAME_WIRE_HTML);
  ROUTE_GAME("/game_slice", GAME_SLICE_HTML);
  ROUTE_GAME("/game_coinjar", GAME_COINJAR_HTML);
  ROUTE_GAME("/game_hook", GAME_HOOK_HTML);
  ROUTE_GAME("/game_balance", GAME_BALANCE_HTML);

  // 47-50 Retro Pack
  ROUTE_GAME("/game_tetris", GAME_TETRIS_HTML);
  ROUTE_GAME("/game_pacmaze", GAME_PACMAZE_HTML);
  ROUTE_GAME("/game_lunar", GAME_LUNAR_HTML);
  ROUTE_GAME("/game_cannon", GAME_CANNON_HTML);

  // --- 15 JEUX MULTIJOUEUR 1V1 TEMPS RÉEL ---
  // Pack 1 : 5 Jeux de Réflexion & Stratégie
  ROUTE_GAME("/mp_connect4", MP_CONNECT4_HTML);
  ROUTE_GAME("/mp_naval", MP_NAVAL_HTML);
  ROUTE_GAME("/mp_tictactoe", MP_TICTACTOE_HTML);
  ROUTE_GAME("/mp_memory", MP_MEMORY_HTML);
  ROUTE_GAME("/mp_mastermind", MP_MASTERMIND_HTML);

  // Pack 2 : 5 Jeux de Réflexe & Action
  ROUTE_GAME("/mp_pong", MP_PONG_HTML);
  ROUTE_GAME("/mp_airhockey", MP_HOCKEY_HTML);
  ROUTE_GAME("/mp_tank", MP_TANK_HTML);
  ROUTE_GAME("/mp_reflex", MP_REFLEX_HTML);
  ROUTE_GAME("/mp_tapduel", MP_TAPDUEL_HTML);

  // Pack 3 : 5 Jeux de Baccalauréat & Mots
  ROUTE_GAME("/mp_petitbac", MP_PETITBAC_HTML);
  ROUTE_GAME("/mp_anagram", MP_ANAGRAM_HTML);
  ROUTE_GAME("/mp_wordbomb", MP_WORDBOMB_HTML);
  ROUTE_GAME("/mp_wordchain", MP_WORDCHAIN_HTML);
  ROUTE_GAME("/mp_hangman", MP_HANGMAN_HTML);

  // Settings
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/save_settings", HTTP_POST, handleSaveSettings);

  server.onNotFound(handleNotFound);
  server.begin();

  // Démarrer le serveur multijoueur temps réel (port 81)
  initMultiplayerServer();

  Serial.println("[OK] Serveur Web HTTP opérationnel (50 Jeux Solo + 15 Jeux Multijoueur 1v1).");
  Serial.println("========================================\n");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  handleMultiplayer();
}
