#include <windows.h>
//
#include <gl/GLU.h>

#include <filesystem>
#include <fstream>
#include <map>
#define MSAA_0X 9
#define MSAA_2X 25
#define MSAA_4X 41
#define MSAA_8X 57
#define MSAA_16X 73
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"
extern "C" {
_declspec(dllexport) int NvOptimusEnablement = 1;
_declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
std::string P(std::string p = "") {
  char _[MAX_PATH];
  GetModuleFileNameA(0, _, MAX_PATH);
  auto s = std::string(_);
  return s.substr(0, s.find_last_of('\\') + 1) + p;
}
std::map<std::string, int> s;
void S() {
  std::string p = P(std::string(getenv("USERNAME")) + ".ini");
  if (s.empty()) {
    std::ifstream f(p, std::ios::binary);
    for (std::string i; getline(f, i);) {
      auto _ = i.find(',');
      s.emplace(i.substr(0, _), stoi(i.substr(_ + 1)));
    }
    return;
  }
  std::ofstream f(p, std::ios::binary);
  for (auto &[k, v] : s) f << k << ',' << v << '\n';
}
int &S(std::string k, int v = 0) {
  if (s.find(k) == s.end()) s.emplace(k, v);
  return s[k];
}
void RP() {
  if (P().substr(P().size() - 8) != "Release\\") return;
  std::ostringstream ss;
  for (auto &i : std::filesystem::directory_iterator("D\\T")) {
    stbi_set_flip_vertically_on_load(1);
    int x, y, n;
    auto *_ = stbi_load(i.path().string().c_str(), &x, &y, &n, 0);
    auto l = x * y * n;
    ss << "T_" << i.path().filename().replace_extension().string() << '\n'
       << x << '\n'
       << y << '\n'
       << n << '\n';
    ss.write((char *)_, l);
    ss << '\n';
    stbi_image_free(_);
  }
  auto MPos = ss.tellp();
  for (auto &i : std::filesystem::directory_iterator("D\\M")) {
  }
  auto APos = ss.tellp();
  for (auto &i : std::filesystem::directory_iterator("D\\A")) {
  }
  std::ofstream(P("D.exe"), std::ios::binary) << MPos << '\n'
                                              << APos << '\n'
                                              << ss.str();
}
struct Txtr {
  int x, y, n;
  std::unique_ptr<unsigned char[]> d;
};
Txtr *RU(std::string r) {
  std::ifstream f(P("D.exe"), std::ios::binary);
  int MPos, APos;
  f >> MPos >> APos;
  switch (r.front()) {
    case 'T':
      for (std::string i; getline(f, i);) {
        if (i.find(r) != std::string::npos) {
          auto t = new Txtr;
          f >> t->x >> t->y >> t->n;
          auto l = t->x * t->y * t->n;
          t->d = std::make_unique<unsigned char[]>(l);
          f.read((char *)t->d.get(), l);
          return t;
        }
      }
      break;
    case 'M':
      f.seekg(MPos);
      break;
    case 'A':
      f.seekg(APos);
      break;
  }
}
struct Entt;
struct Op;
struct Tr;
struct Li;
struct Ui;
std::vector<Entt *> Entts, Kills;
std::vector<Op *> Ops;
std::vector<Tr *> Trs;
std::vector<Li *> Lis;
std::vector<Ui *> Uis;
struct Entt {
  bool IsOn = 1;
  void Enbl(bool b) {
    IsOn = b;
    OEnbl(b);
  }
  virtual void OEnbl(bool b) {}
  virtual void Tick() {}
  virtual void OSpwn() { Entts.push_back(this); }
  virtual void OKill() {
    Entts.erase(std::find(Entts.begin(), Entts.end(), this));
  }
  virtual ~Entt() {}
};
struct Op : Entt {
  void OSpwn() { Ops.push_back(this); }
  void OKill() { Ops.erase(std::find(Ops.begin(), Ops.end(), this)); }
};
struct Tr : Entt {};
struct Li : Entt {};
struct Ui : Entt {};
template <typename T>
T *Spwn() {
  auto _ = new T;
  _->OSpwn();
  return _;
}
void Kill(Entt *e) { Kills.push_back(e); }
void Exit() {
  for (auto &i : Entts) delete i;
  S();
}
struct L : Entt {
  L();
  ~L();
  void Tick();
};
float CC[] = {0, 0, 0, 0};
float TDelt, TScal = 1;
MSG M;
int WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PSTR, _In_ INT) {
  if (SetForegroundWindow(FindWindowA(0, " "))) return 0;
  RP();
  S();
  auto W = CreateWindowExA(SetProcessDPIAware(), "Static", " ",
                           WS_POPUP | WS_SIZEBOX | SS_NOTIFY, 0, 0, 0, 0, 0, 0,
                           0, 0);
  auto H = GetDC(W);
  ShowWindow(W, 3);
  SetPixelFormat(H, S("V_MSAA", MSAA_4X), new PIXELFORMATDESCRIPTOR{});
  wglMakeCurrent(H, wglCreateContext(H));
  wglUseFontBitmapsA(H, 0, 255, 0);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ((GLUtessBeginProc)wglGetProcAddress("wglSwapIntervalEXT"))(S("V_VSYNC", 1));
  int _[] = {0, GL_DEPTH_TEST, GL_COLOR_MATERIAL, GL_TEXTURE_2D};
  for (auto &i : _) glEnable(i);
  Spwn<L>();
  while (SwapBuffers(H)) {
    Sleep(!SetForegroundWindow(W));
    if (PeekMessageA(&M, 0, 0, 0, 1)) DispatchMessageA(&M);
    TDelt = (clock() - *_) * TScal / 1000;
    *_ = clock();
    for (auto &i : Entts)
      if (i->IsOn) i->Tick();
    glClearColor(CC[0], CC[1], CC[2], CC[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(S("V_FOV", 60),
                   float(GetSystemMetrics(0)) / GetSystemMetrics(1), .01, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (auto &i : Ops)
      if (i->IsOn) i->Tick();
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    for (auto &i : Trs)
      if (i->IsOn) i->Tick();
    glEnable(GL_DEPTH_TEST);
    for (auto &i : Lis)
      if (i->IsOn) i->Tick();
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(GetSystemMetrics(0) / -2, GetSystemMetrics(0) / 2,
               GetSystemMetrics(1) / -2, GetSystemMetrics(1) / 2);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    for (auto &i : Uis)
      if (i->IsOn) i->Tick();
    glDisable(GL_BLEND);
    for (auto &i : Kills) {
      i->OKill();
      delete i;
    }
    Kills.clear();
  }
  Exit();
  return 0;
}
L::L() { auto t = RU("RGB"); }
L::~L() {}
void L::Tick() {}