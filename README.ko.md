# Qt Quick Starter Kit

*English version → [README.md](README.md)*

새 Qt Quick 프로젝트를 시작하는 개발자를 위한 Qt 6 starter 템플릿입니다.
특히 Qt를 처음 접하는 개발자가 빈 `main.qml`로 시작하는 대신 *실제로 동작하는,
믿을 만한 시작점*이 필요한 상황을 위해 만들어졌습니다.

이 템플릿이 첫날부터 제공하는 것:

1. **명확한 UI / 로직 분리** — QML 레이아웃은 한 폴더에, C++ 코드는 다른 폴더에
   두고 잘 정의된 단일 레이어로 연결합니다. 새로운 패턴을 익힐 필요 없이 — 폴더와
   규칙만으로 임베디드 Qt 프로젝트가 실제로 어떻게 흘러가는지 보여줍니다.

2. **계속 동작하는 UI 미리보기** — `ui-kit/forms/`의 모든 레이아웃 파일은 `qml`
   런타임이나 Qt Design Studio에서 *단독으로* 로드됩니다. 빌드 없이, 백엔드 없이,
   타겟 보드 없이. 이 속성은 첫날에도 유효하고 700일째에도 유효합니다.

3. **Figma → Qt Design Studio → CMake 브릿지** — `ui-kit/` 폴더는 자체적인
   `.qmlproject` 파일을 포함해 Qt Design Studio (그리고 Figma2Qt)에서 직접 열
   수 있고, *동시에* 같은 파일들이 메인 CMake 프로젝트에 정식 QML 모듈로
   컴파일됩니다. 단일 진실 소스(single source of truth), 두 개의 유효한 진입점.

4. **QML 모듈을 위한 CMake 레퍼런스** — 작은 `CMakeLists.txt` 파일 4개로
   `qt_add_qml_module` 사용법, 의존성을 한 방향으로 유지하는 방법,
   QML-only 모듈에서 C++ 모듈, 실행파일까지 연결하는 방법을 보여줍니다.
   `qt_add_qml_module` 문서를 읽으면서 *"근데 이걸 내 프로젝트 어디에 두지?"*
   하고 막막했다면, 이 템플릿이 그 답입니다.

5. **CMake가 어렵게 느껴지는 팀을 위한 진입로** — 4개의 CMake 파일은 짧고,
   주석이 달려 있고, 모두 같은 패턴을 따릅니다. 복사해서 이름만 바꾸면 자기
   프로젝트로 키울 수 있고, CMake 전문가가 될 필요는 없습니다.

이건 완성된 애플리케이션이 아닙니다. 약 30개 파일로 의도적으로 최소한으로 줄인
*작은 골격*이며, 실제 프로젝트의 첫 커밋이 되도록 설계되었습니다.

> **더 큰 프로젝트인가요?** 자매 브랜치 `view-navigator`는 같은 구조 규칙을
> 따르되, 페이지마다 ViewModel을 두고 작은 `ViewNavigator`와 services-injection
> 패턴을 적용합니다. 페이지 수가 늘어 단일 공유 ViewModel이 비좁게 느껴지기
> 시작할 때 그쪽으로 옮기면 됩니다. 자세한 내용은 이 문서 마지막의 "여기서
> 키워가기" 섹션을 참고하세요.

---

## 왜 "Starter"인가

이 이름은 신중하게 선택했습니다. 주된 대상은 Qt를 처음 접하는 개발자 — 보통
오픈소스 커뮤니티가 아니라 상용 라이선스 도입 과정을 통해 들어오는 — 이며, 실제
Qt Quick 프로젝트를 시작할 *믿을 만한 출발점*이 필요한 사람들입니다. 어떤
패턴도 아니고, 어떤 철학도 아닙니다. 그냥 시작점일 뿐입니다.

MVVM 같은 아키텍처 패턴에 이미 익숙한 팀은 이 구조를 보자마자 알아볼 것입니다.
이름이 그 어휘를 강제하지는 않지만요.

---

## 이 템플릿이 해결하는 것

전형적인 임베디드 Qt Quick 프로젝트는 단일 `main.qml`로 시작합니다. 처음 몇 주
동안 개발자는 매우 생산적입니다. QML을 편집하고, Ctrl-R을 누르고, 변화를 봅니다.
모든 것이 개발자의 노트북에서 돌아갑니다.

그러다 C++ 타입이 등장합니다. 싱글톤이 등록됩니다. 하드웨어 기반 모델을 위한
커스텀 플러그인이 추가됩니다. 벤더 SDK가 링크됩니다. 하나씩, `main.qml`은 *전체
빌드에서만 존재하는* 의존성을 갖게 됩니다.

팀이 알아챘을 즈음엔 이미 되돌리기 어려운 상태입니다. 단순한 UI 변경 — 색상 변경,
간격 조정 — 도 이제 풀 타겟 빌드, 보드 플래시, 부팅 사이클이 필요합니다. 피드백
루프가 **수 초에서 수 분, 때로는 수십 분**으로 늘어납니다. 여러 명으로 구성된
팀과 다년간 진행되는 프로젝트, 그리고 출시 후 모든 OTA 업데이트 사이클을
누적하면 — 그 비용은 결코 작지 않습니다.

이외에도, 실제 프로젝트에서 일찍 등장하지만 명확한 출발점이 없는 질문들이
있습니다:

- **"프로젝트가 커지는 동안 UI를 계속 미리보기 가능하게 유지하려면?"**
- **"Figma / Qt Design Studio 결과물을 실제 CMake 빌드에 어떻게 연결하지?"**
- **"`qt_add_qml_module`을 모듈 두 개 이상으로 쓰려면 어떻게 구성해야 하나?"**
- **"QML과 C++을 폴더로 분리하면서 import를 깨뜨리지 않으려면?"**

이 템플릿은 위 모든 질문에 일관된 구조 하나로 답합니다. UI 미리보기를 가능하게
하는 같은 폴더 구조가 동시에 Figma 브릿지, CMake 모듈 예시, 그리고 확장 가능한
레이어 구조까지 함께 제공합니다.

---

## 무엇이 들어 있나

```text
┌─ ui-kit/ ─────────────────────────────────────────────────────┐
│                                                               │
│   forms/    Pure QML 레이아웃 — 단독으로 미리보기 가능        │
│      ↑  property alias 바인딩                                 │
│   views/    ViewModel 바인딩 + StackView 네비게이션           │
│                                                               │
│   ui-kit.qmlproject  ← Qt Design Studio / Figma2Qt 진입점     │
└───────────────────────────┬───────────────────────────────────┘
                            │  required property var viewModel
                ┌───────────▼────────────┐
                │      viewmodels/        │  C++ QML_SINGLETON
                └───────────┬────────────┘
                            │  bindDataSource()
                ┌───────────▼────────────┐
                │       domain/           │  Pure C++
                └────────────────────────┘

        app-main/  →  진입점, viewmodels ↔ domain 연결

        각 박스 = 자체 CMakeLists.txt (총 4개 파일)
```

샘플은 의도적으로 작게 만들어졌습니다 — `StackView` 안의 페이지 3개
(`View1` → `View2` → `View3`), value property 하나, status 문자열 하나, 그리고
몇 개의 로그 메시지. *구조가 곧 제품이고, 샘플은 구조를 쉽게 보고 복사하기
위해서만 존재*합니다.

ViewModel은 3개의 property(`value`, `status`, `lastVisitedPage`)와 1개의
invokable 메서드(`notifyPageEntered`)를 노출합니다. 페이지 전환마다 콘솔에
로그를 남기므로 property가 처음부터 끝까지 어떻게 흐르는지 확인할 수 있습니다.

---

## `ui-kit/`의 두 진입점

이 템플릿이 구체적으로 만들어낸 것 중 하나입니다. 같은 QML 파일 묶음이 *완전히
다른 두 도구*에 대해 유효한 입력이 됩니다:

### 진입점 1 — `qml` 런타임 (모든 개발자의 노트북)

```bash
cd ui-kit
qml forms/View1Form.ui.qml
```

페이지 1이 하드코딩된 기본값으로 렌더링됩니다. 빌드 없이. 백엔드 없이. 보드 없이.
form을 편집하고, 다시 로드하고, 결과를 봅니다. Qt가 설치된 모든 노트북에서
동작하며, 구조 규칙만 지키면 프로젝트가 진행되는 내내 계속 동작합니다.

### 진입점 2 — Qt Design Studio / Figma2Qt

```bash
# Qt Design Studio에서 ui-kit/ui-kit.qmlproject 파일 열기
```

`ui-kit.qmlproject` 파일은 자체 QML import 경로와 프로젝트 구조를 선언하므로,
Qt Design Studio (그리고 Figma2Qt 익스포트 플러그인)는 이 폴더를 *직접 열 수
있습니다* — C++ 빌드, CMake 프로젝트, 백엔드 레이어 없이도.

이게 의미하는 바:

- Figma를 사용하는 디자이너가 Qt Design Studio로 export하면, 작업물이
  `ui-kit/forms/`에 `.ui.qml` 파일로 그대로 들어갑니다.
- 같은 `.ui.qml` 파일이 `ui-kit/CMakeLists.txt`의 `qt_add_qml_module`을 통해
  메인 CMake 빌드에 소비됩니다 — 복사도, 변환도, 이중 관리도 없음.
- C++ 개발자는 백엔드를 바꿀 수 있고, QML 개발자는 레이아웃을 바꿀 수 있고,
  디자이너는 Qt Design Studio에서 비주얼을 바꿀 수 있습니다. 누구도 다른 사람을
  막지 않습니다.

이 브릿지는 실제 프로젝트에서 보통 임시방편으로, 부실하게, 프로젝트마다 새로
만들어지는 부분입니다. 여기서는 그게 기본값입니다.

---

## 4개의 `CMakeLists.txt` — 동작하는 레퍼런스

전체 프로젝트는 정확히 4개의 CMake 파일을 가지며, 각 레이어마다 하나씩, 모두
같은 패턴을 따릅니다. 순서대로 읽으면 모던 Qt 6 CMake에 대한 짧은 튜토리얼이
됩니다:

| 파일 | 무엇을 보여주나 |
|---|---|
| `domain/CMakeLists.txt` | 순수 C++ static library — 가장 단순한 형태의 Qt CMake 타겟 |
| `viewmodels/CMakeLists.txt` | `qt_add_qml_module` 기반 C++ QML 모듈로, `QML_SINGLETON` 등록 |
| `ui-kit/CMakeLists.txt` | `qt_add_qml_module` 기반 pure-QML 모듈 — C++ link 의존성 없이 QML과 리소스 파일만 |
| `app-main/CMakeLists.txt` | 3개 모듈을 묶어 실행파일을 만드는 `qt_add_executable` 타겟 |

의존성 화살표는 한 방향뿐입니다: `domain → viewmodels → app-main`, 그리고
`ui-kit`은 `app-main`이 QML 모듈로 소비합니다. 어떤 레이어도 자기 위쪽 레이어에
의존하지 않습니다. 각 `CMakeLists.txt`는 한 자리에 앉아서 다 읽을 수 있을 만큼
짧고, 각 줄이 *왜* 거기 있는지 주석으로 설명되어 있으며, 자기 모듈 추가의
출발점으로 그대로 복사할 수 있도록 이름이 명확합니다.

만약 팀이 지금까지 거대한 단일 `CMakeLists.txt` 하나에 `find_package(Qt6 ...)`
같은 호출을 여기저기 흩뿌려 온 형태라면, 이 템플릿이 *모듈식 대안*의 모습입니다.

---

## Quick start

### 빌드 없이 노트북에서 UI 미리보기

```bash
cd ui-kit
qml forms/View1Form.ui.qml
```

또는 `ui-kit/ui-kit.qmlproject`를 Qt Design Studio나 Qt Creator에서 열면 라이브
편집이 가능한 더 풍부한 미리보기 환경을 얻을 수 있습니다.

---

## 저장소 레이아웃

```text
qt-quick-starter/
├── README.md
├── README.ko.md
├── ARCHITECTURE.md
├── CMakeLists.txt                  # 최상위
│
├── ui-kit/                         # Pure QML — 디자이너 친화적
│   ├── ui-kit.qmlproject           # Qt Design Studio / Figma2Qt 진입점
│   ├── CMakeLists.txt              # qt_add_qml_module (QML 전용)
│   ├── AppShell.qml                # StackView host
│   ├── forms/                      # Pure QML 레이아웃 (미리보기 가능)
│   │   ├── View1Form.ui.qml
│   │   ├── View2Form.ui.qml
│   │   └── View3Form.ui.qml
│   ├── views/                      # ViewModel 바인딩 + 네비게이션
│   │   ├── View1.qml
│   │   ├── View2.qml
│   │   └── View3.qml
│   ├── components/                 # 재사용 가능한 커스텀 UI 부품
│   │   └── AppButton.qml
│   └── previews/AppPreview.qml
│
├── app-main/                       # 진입점 — 모든 것을 연결
│   ├── Main.qml
│   ├── main.cpp
│   └── CMakeLists.txt              # qt_add_executable
│
├── viewmodels/                     # C++ ViewModel (QML_SINGLETON)
│   ├── AppDataViewModel.h
│   ├── AppDataViewModel.cpp
│   └── CMakeLists.txt              # qt_add_qml_module (C++ 포함)
│
├── domain/                         # 순수 C++ 도메인 상태
│   ├── AppDataSource.h
│   ├── AppDataSource.cpp
│   └── CMakeLists.txt              # 평범한 C++ library
│
└── tests/                          # 도메인 + UI 테스트
```

---

## 여기서 키워가기

이 템플릿은 의도적으로 **최소한의 골격**입니다. 일반적인 확장 방법:

- **새 property 추가.** `AppDataViewModel.h/.cpp`에 추가하고 관련된
  `ui-kit/views/` 파일에서 바인딩합니다. 따로 동기화할 인터페이스 파일도, 유지할
  mock도 없습니다.

- **새 페이지 추가.** `ui-kit/forms/`에 `View4Form.ui.qml`을, `ui-kit/views/`에
  `View4.qml`을 만듭니다. 둘 다 `ui-kit/CMakeLists.txt`에 등록합니다. 어느
  view에서든 `StackView.view.push(Qt.resolvedUrl(...))`로 push 가능합니다.

- **새 C++ 모듈 추가.** `viewmodels/CMakeLists.txt`를 복사해 모듈 URI를 바꾸고
  새 타입을 등록하면 됩니다. C++ QML 모듈을 추가할 때마다 항상 같은 패턴입니다.

- **실제 데이터 소스 연결.** `domain/` 아래에 클래스(데이터베이스, 네트워크
  클라이언트, 벤더 API 등)를 작성하고, 현재 `AppDataSource`가 연결된 방식과
  *동일하게* `AppDataViewModel::bindDataSource`에서 연결합니다.

이 모든 과정 동안 `ui-kit/forms/`는 `qml`과 Qt Design Studio에서 계속 로드됩니다.
디자이너의 워크플로는 C++ 작업에 막히지 않고, C++ 작업도 디자인 반복에 막히지
않습니다.

### `main`으로 충분하지 않을 때 — `view-navigator` 브랜치

이 브랜치의 단일 공유 ViewModel 패턴은 가장 단순한 형태입니다. 페이지가 5~6개
정도 까지는 잘 확장됩니다. 그 이상이 되면 보통 두 가지 실패 양상이 나타납니다:

- **머지 충돌의 중력장.** 모든 페이지의 property가 단일 `AppDataViewModel`
  클래스에 있다 보니, 페이지 수가 늘면서 이 ViewModel 파일은 프로젝트에서 가장
  자주 편집되는 파일이 됩니다. 서로 무관한 작업이 이곳에서 충돌합니다.

- **페이지 스코프 상태가 들어갈 곳이 없음.** 카운터, 임시 입력값, 페이지별
  선택 상태 — 이런 것들이 깔끔히 들어갈 자리가 없습니다. 결국 공유 ViewModel을
  오염시키거나 QML 상태로 흘러나갑니다.

자매 브랜치 `view-navigator`는 같은 구조 규칙(UI / 로직 분리, 호스트에서
테스트 가능한 UI, Figma 브릿지, 모듈식 CMake)을 그대로 적용하면서, 단일
ViewModel을 다음으로 대체합니다:

- 페이지마다 하나씩의 `View*ViewModel` C++ 클래스 — `ViewModelBase`를 상속하며
  `pushRoute()` 호출마다 작은 `ViewNavigator`가 인스턴스화합니다.

- 6개의 lifecycle hook (`onInitialize`, `onActivating`, `onActivated`,
  `onDeactivating`, `onDeactivated`, `onFinalize`) — C++ 리소스 준비/해제가
  사용자가 실제로 보고 있는 화면과 동기화됩니다.

- 장기 보존 도메인 포인터를 묶는 `AppServices` struct — 새 도메인 클래스를
  추가해도 어떤 ViewModel 생성자 시그니처도 변하지 않습니다.

- 데모 코드와 분리된 재사용 인프라를 담는 `core/` 폴더.

코드가 더 많고(~30개 대신 ~50개 파일) 익혀야 할 어휘도 늘어납니다. 페이지가
실제로 많을 때만 그 무게를 정당화할 수 있습니다. **프로젝트의 페이지 수가
적고 상태가 대부분 공유되는 형태라면 이 브랜치에 머무세요. 위의 증상이
느껴지기 시작하면 `view-navigator`로 옮기세요.**

---
