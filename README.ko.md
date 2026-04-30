# Qt Quick Starter Application

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

4. **QML 모듈을 위한 CMake 레퍼런스** — 작은 `CMakeLists.txt` 파일 5개로
   `qt_add_qml_module` 사용법, 의존성을 한 방향으로 유지하는 방법,
   QML-only 모듈에서 C++ 모듈, 실행파일까지 연결하는 방법을 보여줍니다.

5. **인프라 / 데모 분리가 명확함** — 재사용 가능한 부품(`ViewNavigator`,
   `ViewModelBase`, `AppServices`)은 `core/` 폴더에 모여 있고 *수정하지 않고
   의존만 하면 됩니다*. 다른 폴더(`domain/`, `viewmodels/`, `ui-kit/`,
   `app-main/`)는 `core/`를 *어떻게 쓰는지* 보여주는 데모 코드이며, 자기 코드로
   교체하거나 확장할 수 있습니다. 폴더 경계가 어느 쪽이 어느 쪽인지 알려줍니다.

6. **페이지마다 ViewModel 하나, 자동 생성** — 각 `View*.qml`은 짝이 되는
   `View*ViewModel` C++ 클래스를 가집니다. `ViewNavigator`가 페이지마다 알맞은
   ViewModel을 만들어 View에 전달합니다. 페이지 추가 = View 하나 + ViewModel
   하나 + route 테이블에 한 줄. ViewModel은 6개의 lifecycle hook(`onInitialize`,
   `onActivating`, `onActivated`, `onDeactivating`, `onDeactivated`,
   `onFinalize`)을 노출하며, StackView 페이지 라이프사이클에 묶여 있어 C++
   리소스의 준비와 해제가 *사용자가 실제로 보고 있는 화면*과 동기화됩니다.

이건 완성된 애플리케이션이 아닙니다. 의도적으로 최소한으로 줄인 *작은 골격*이며,
실제 프로젝트의 첫 커밋이 되도록 설계되었습니다.

> **더 작은 프로젝트인가요?** 자매 브랜치 `main`은 같은 구조 규칙을 따르되,
> 페이지별 ViewModel 대신 단일 공유 `AppDataViewModel` `QML_SINGLETON`을
> 사용합니다. 페이지가 5~6개 정도로 충분하고, `ViewNavigator` / `AppServices`
> / lifecycle hook 같은 추가 어휘가 프로젝트에 비해 무겁게 느껴진다면 `main`
> 브랜치가 더 어울립니다.

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
- **"모든 화면의 상태를 거대한 단일 ViewModel 싱글톤이 떠안는 걸 어떻게 피할까?"**
- **"starter의 어느 부분이 *유지해야 할 인프라*이고 어느 부분이 *교체할 데모
  코드*인지 어떻게 구분하지?"**

이 템플릿은 위 모든 질문에 일관된 구조 하나로 답합니다.

---

## 무엇이 들어 있나

```text
┌─ ui-kit/ ─────────────────────────────────────────────────────┐
│                                                               │
│   forms/    Pure QML 레이아웃 — 단독으로 미리보기 가능        │
│      ↑  property alias 바인딩                                 │
│   views/    ViewModel 바인딩 + StackView.pushRoute()          │
│                                                               │
│   ui-kit.qmlproject  ← Qt Design Studio / Figma2Qt 진입점     │
└───────────────────────────┬───────────────────────────────────┘
                            │  required property var viewModel
                            │  (push마다 새로운 ViewModel)
                ┌───────────▼────────────────┐
                │      viewmodels/            │  C++, 페이지마다 클래스 1개
                │      View1/2/3 ViewModel    │  (ViewModelBase 상속)
                └───────────┬────────────────┘
                            │  ViewNavigator가 생성
                ┌───────────▼────────────────┐
                │   core/ (인프라)             │  ViewNavigator + ViewModelBase
                │                             │  + AppServices struct
                └───────────┬────────────────┘
                            │  공유 인스턴스
                ┌───────────▼────────────────┐
                │       domain/               │  Pure C++ (예: AppDataSource)
                └────────────────────────────┘

        app-main/  →  진입점, route 선언, core/의 ViewNavigator로
                      서비스 주입

        각 박스 = 자체 CMakeLists.txt (총 5개 파일)
```

인프라(`core/`)는 그대로 두는 것이 원칙입니다. 다른 폴더(`domain/`,
`viewmodels/`, `ui-kit/`, `app-main/`)는 작은 동작 샘플 — `StackView` 안에서
`View1` → `View2` → `View3` 세 페이지 — 를 담고 있고, 각 페이지는 자기
ViewModel을 가집니다. 모든 페이지는 도메인에서 오는 동일한 `value`/`status`와
페이지별 `enterCount`(페이지 스코프 상태)를 함께 보여줍니다. 이 샘플은 *구조를
보여주고 복사하기 쉽게 하기 위한 것일 뿐* — 실제 애플리케이션을 키워가면서
자유롭게 지우거나 교체하면 됩니다.

---

## 페이지 하나가 만들어지는 과정 — 한 페이지 투어

사용자가 "Go to View 2" 버튼을 눌렀을 때:

1. 버튼이 `StackView.view.pushRoute("View2")`를 호출.
2. `AppShell`이 `ViewNavigator`에게 `"View2"`를 resolve해 달라고 요청.
3. `ViewNavigator`가 새 `View2ViewModel`을 생성 — 생성자에 `AppServices`
   번들(공유 도메인 인스턴스 포인터들의 struct)을 const reference로 전달.
4. `ViewNavigator`가 QML로 `{ url, viewModel }`을 반환.
5. `AppShell`이 `StackView.push(url, { viewModel })` 호출.
6. `View2.qml`의 `required property var viewModel`이 갓 만들어진 객체로
   채워지고, property 바인딩이 즉시 동작 시작.

사용자가 다시 View 1로 pop하면 View 2 객체는 파괴되고 그 직후 JavaScript GC가
`View2ViewModel`을 회수합니다. View 2를 다시 push하면 `enterCount`가 다시 1인
새 인스턴스가 생성됩니다.

`AppServices` 안의 공유 도메인 인스턴스(페이지 간 공유 상태)는 어떤 view에도
의존하지 않고 독립적으로 계속 동작합니다.

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

## 5개의 `CMakeLists.txt` — 동작하는 레퍼런스

전체 프로젝트는 정확히 5개의 CMake 파일을 가지며, 각 레이어마다 하나씩, 모두
같은 패턴을 따릅니다:

| 파일 | 무엇을 보여주나 |
|---|---|
| `core/CMakeLists.txt` | 재사용 인프라를 담은 C++ QML 모듈 (`ViewNavigator` `QML_SINGLETON`, `ViewModelBase` 추상 타입, `AppServices` struct) |
| `domain/CMakeLists.txt` | 순수 C++ static library — 가장 단순한 형태의 Qt CMake 타겟 |
| `viewmodels/CMakeLists.txt` | `qt_add_qml_module` 기반 C++ QML 모듈로, `QML_UNCREATABLE` 페이지 스코프 ViewModel을 등록 |
| `ui-kit/CMakeLists.txt` | `qt_add_qml_module` 기반 pure-QML 모듈 — C++ link 의존성 없이 QML과 리소스 파일만 |
| `app-main/CMakeLists.txt` | 4개 모듈을 묶어 실행파일을 만들고 `App.Integration` QML 진입 모듈을 제공하는 `qt_add_executable` 타겟 |

의존성 화살표는 한 방향뿐입니다:

```text
core ◄── domain
core ◄── viewmodels ◄── domain
core ◄── app-main ◄── viewmodels ◄── domain
ui-kit (app-main이 QML 모듈로 소비 — 프로젝트 소유 C++ link 없음)
```

어떤 레이어도 자기 위쪽 레이어에 의존하지 않습니다.

---

## Naming convention

이 템플릿은 **C++ namespace를 피하는 Qt 컨벤션**을 따릅니다. 타입 이름
자체로 충분히 고유합니다 — `ViewNavigator`, `ViewModelBase`, `AppServices`,
`AppDataSource`, `View1ViewModel` 등. 폴더 간 이름 충돌은 namespace에
의존하는 대신 *구체적이고 묘사적인 이름*으로 방지합니다.

**인프라 / 데모 경계는 namespace가 아니라 폴더로 그어집니다.** `core/`는
재사용 인프라이고, 나머지는 모두 교체하거나 확장하기 위한 데모 코드입니다.

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
├── core/                           # ◄ 재사용 인프라 — 수정하지 않음
│   ├── ViewNavigator.h             #   route 테이블 + ViewModel 팩토리
│   ├── ViewNavigator.cpp           #   (App.Core의 QML_SINGLETON)
│   ├── ViewModelBase.h             #   페이지 ViewModel을 위한 추상 base
│   ├── AppServices.h               #   포인터 struct — 키워가며 확장
│   └── CMakeLists.txt              #   qt_add_qml_module (URI: App.Core)
│
├── ui-kit/                         # Pure QML — 디자이너 친화적
│   ├── ui-kit.qmlproject           # Qt Design Studio / Figma2Qt 진입점
│   ├── CMakeLists.txt              # qt_add_qml_module (QML 전용)
│   ├── AppShell.qml                # StackView host + pushRoute helper
│   ├── forms/                      # Pure QML 레이아웃 (미리보기 가능)
│   │   ├── View1Form.ui.qml
│   │   ├── View2Form.ui.qml
│   │   └── View3Form.ui.qml
│   ├── views/                      # ViewModel 바인딩 + 네비게이션
│   │   ├── ViewBase.qml            # base 타입 — lifecycle hook 연결
│   │   ├── View1.qml
│   │   ├── View2.qml
│   │   └── View3.qml
│   ├── components/                 # 재사용 가능한 커스텀 UI 부품
│   │   └── AppButton.qml
│   └── previews/AppPreview.qml
│
├── app-main/                       # 진입점 — 모든 것을 연결
│   ├── Main.qml
│   ├── main.cpp                    # route 테이블 선언
│   └── CMakeLists.txt              # qt_add_executable + App.Integration
│
├── viewmodels/                     # C++ ViewModel (view마다 1개)
│   ├── View1ViewModel.h/.cpp
│   ├── View2ViewModel.h/.cpp
│   ├── View3ViewModel.h/.cpp
│   └── CMakeLists.txt              # qt_add_qml_module (C++ 포함)
│
├── domain/                         # 순수 C++ 도메인 상태
│   ├── AppDataSource.h
│   ├── AppDataSource.cpp
│   └── CMakeLists.txt              # 평범한 C++ library
│
└── tests/                          # 호스트에서 실행 가능한 테스트
    ├── domain/                     # plain QtTest — 도메인 로직
    │   └── tst_app_datasource.cpp
    ├── viewmodels/                 # plain QtTest — VM 동작 + AppServices
    │   └── tst_view1_viewmodel.cpp
    ├── ui-visual/                  # QtQuickTest — form 레이아웃 검증
    │   ├── main.cpp
    │   └── tst_ui_kit.qml
    └── CMakeLists.txt
```

---

## 여기서 키워가기

이 템플릿은 의도적으로 **최소한의 골격**입니다. 일반적인 확장 방법:

- **기존 페이지에 새 property 추가.** 해당 페이지의 `View*ViewModel.h/.cpp`에
  추가하고 짝이 되는 `ui-kit/views/` 파일에서 바인딩합니다. 따로 동기화할
  인터페이스 파일도, 유지할 mock도 없습니다.

- **새 페이지 추가.** 세 단계:
  1. `ui-kit/forms/`에 `ViewNForm.ui.qml`, `ui-kit/views/`에 `ViewN.qml`
     생성. 둘 다 `ui-kit/CMakeLists.txt`에 등록.
  2. `viewmodels/`에 `ViewModelBase`를 상속한 `ViewNViewModel.h/.cpp` 생성.
     `viewmodels/CMakeLists.txt`에 등록.
  3. `app-main/main.cpp`에
     `navigator->registerRoute<ViewNViewModel>("ViewN", QUrl(...))` 한 줄 추가.

  그 후 어느 view에서든 `StackView.view.pushRoute("ViewN")`으로 push 가능.
  QML import는 변경되지 않습니다.

- **새 C++ 모듈 추가.** `viewmodels/CMakeLists.txt`를 복사해 모듈 URI를 바꾸고
  새 타입을 등록하면 됩니다. C++ QML 모듈을 추가할 때마다 항상 같은 패턴입니다.

- **실제 데이터 소스 연결.** `domain/` 아래에 클래스(데이터베이스, 네트워크
  클라이언트, 벤더 API 등)를 작성하고, `core/AppServices.h`에서
  forward-declare 한 뒤 `AppServices` struct에 `Foo *foo = nullptr;` 한 줄
  추가합니다. 그 후 `main.cpp`에서 인스턴스를 할당해 `services.foo`에
  대입하면 됩니다. 이 서비스를 사용하고 싶은 ViewModel은 `m_services.foo`를
  읽으면 됩니다. `core/`의 한 줄 struct 멤버 외에는 아무 변경도 없고,
  `registerRoute<T>()` 변경도 없으며, 다른 VM의 시그니처 변경도 없습니다.
  전체 예시는 `ARCHITECTURE.md`의 "Wiring domain services"를 참고하세요.

이 모든 과정 동안 `ui-kit/forms/`는 `qml`과 Qt Design Studio에서 계속 로드됩니다.
디자이너의 워크플로는 C++ 작업에 막히지 않고, C++ 작업도 디자인 반복에 막히지
않습니다.

---
