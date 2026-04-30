# Qt Quick Starter Application

*한국어 버전 → [README.ko.md](README.ko.md)*

A Qt 6 starter template for developers beginning a new Qt Quick project — especially
those new to Qt who need a credible, working starting point rather than an empty
`main.qml`.

This template gives you, on day one:

1. **A clear UI / logic separation** — your QML layouts live in one folder,
   your C++ code lives in another, and they connect through a single, well-defined
   layer. No new patterns to learn — just folders and rules that match how
   embedded Qt projects actually work.
2. **A UI preview that keeps working** — every layout file in `ui-kit/forms/`
   loads standalone in the `qml` runtime or in Qt Design Studio, with no build,
   no backend, no target board. This stays true on day one and on day 700.
3. **A working Figma → Qt Design Studio → CMake bridge** — the `ui-kit/`
   folder is a self-contained `.qmlproject` that Qt Design Studio (and Figma2Qt)
   can open directly, *and* the same files compile into your main CMake project
   as a proper QML module. One source of truth, two valid entry points.
4. **A CMake reference for QML modules** — five small `CMakeLists.txt` files
   showing how to use `qt_add_qml_module`, how to keep dependencies
   one-directional, and how to wire a QML-only module to a C++ module to an
   executable.
5. **A clear infrastructure / demo split** — the reusable plumbing
   (`ViewNavigator`, `ViewModelBase`, `AppServices`) lives in its own
   `core/` folder and is meant to be depended on, not edited. The other
   folders (`domain/`, `viewmodels/`, `ui-kit/`, `app-main/`) demonstrate
   how to *use* `core/` and are meant to be replaced or extended with your
   own code. The folder boundary tells you which is which.
6. **One ViewModel per page, instantiated automatically** — each `View*.qml`
   has a matching `View*ViewModel` C++ class. The `ViewNavigator` constructs
   the right ViewModel for each page and hands it to the View. Add a page =
   add one View, one ViewModel, and one line in the route table. The
   ViewModel exposes six lifecycle hooks (`onInitialize`, `onActivating`,
   `onActivated`, `onDeactivating`, `onDeactivated`, `onFinalize`) wired to
   the StackView page lifecycle, so C++ resource preparation and cleanup is
   synchronised with what the user is actually seeing.

This is not a finished application. It is a small amount of scaffolding,
deliberately minimal, designed to be the first commit of a real project.

> **Smaller project?** A sibling branch — `main` — applies the same
> structural rules but with a single shared `AppDataViewModel`
> `QML_SINGLETON` instead of per-page ViewModels. Reach for it when ~5–6
> pages is enough and the extra vocabulary (`ViewNavigator`,
> `AppServices`, lifecycle hooks) feels heavier than your project needs.

---

## Why "Starter"

The name was chosen deliberately. The primary audience is developers new to
Qt — typically arriving through commercial onboarding rather than the open-source
community — who need a credible starting point for a real Qt Quick project.
Not a pattern. Not a philosophy. Just a starting point.

Teams that already know architectural patterns like MVVM will recognise the
structure immediately. The name does not force that vocabulary on anyone else.

---

## What this template solves

A typical embedded Qt Quick project starts with a single `main.qml`. The
developer is highly productive for the first few weeks. Edit QML, press Ctrl-R,
see the change. Everything runs on the developer's laptop.

Then C++ types appear. Singletons get registered. Custom plugins are added
for hardware-backed models. Vendor SDKs link in. One by one, `main.qml` acquires
dependencies that only exist in the full build.

By the time a team notices, it is already hard to undo. A simple UI tweak —
a colour change, a spacing adjustment — now requires a full target build, a
flash to the board, and a boot cycle. Feedback loops stretch from **seconds
to minutes, sometimes tens of minutes**. Across a team of several people and
a multi-year project — and across every OTA update cycle after launch — that
is a large cumulative cost.

On top of this, several other questions show up early in real projects and
have no obvious starting answer:

- **"How do I keep the UI previewable as the project grows?"**
- **"How do I connect Figma / Qt Design Studio output to my actual CMake build?"**
- **"How do I structure `qt_add_qml_module` for more than one module?"**
- **"How do I split QML and C++ into folders without breaking imports?"**
- **"How do I avoid one giant ViewModel singleton owning the state of every screen?"**
- **"Which parts of the starter is *infrastructure I should keep* and which are
  *demo code I should replace*?"**

This template answers all of these in one consistent structure.

---

## What you get

```text
┌─ ui-kit/ ─────────────────────────────────────────────────────┐
│                                                               │
│   forms/    Pure QML layouts — preview-able standalone        │
│      ↑  property alias bindings                               │
│   views/    ViewModel binding + StackView.pushRoute()         │
│                                                               │
│   ui-kit.qmlproject  ← Qt Design Studio / Figma2Qt opens this │
└───────────────────────────┬───────────────────────────────────┘
                            │  required property var viewModel
                            │  (one fresh ViewModel per push)
                ┌───────────▼────────────────┐
                │      viewmodels/            │  C++, one class per view
                │      View1/2/3 ViewModel    │  (derived from ViewModelBase)
                └───────────┬────────────────┘
                            │  built by ViewNavigator
                ┌───────────▼────────────────┐
                │   core/ (infrastructure)    │  ViewNavigator + ViewModelBase
                │                             │  + AppServices struct
                └───────────┬────────────────┘
                            │  shared instances
                ┌───────────▼────────────────┐
                │       domain/               │  Pure C++ (e.g. AppDataSource)
                └────────────────────────────┘

        app-main/  →  entry point, declares routes, wires services into the
                      ViewNavigator from core/

        Each box = its own CMakeLists.txt (5 files total)
```

The infrastructure (`core/`) is meant to be left alone. The other folders
(`domain/`, `viewmodels/`, `ui-kit/`, `app-main/`) contain a small working
sample: three pages in a `StackView` (`View1` → `View2` → `View3`), each
with its own ViewModel. Every page shows the same shared `value`/`status`
(from the domain) plus its own `enterCount` (page-scoped state). The
sample exists only to make the structure easy to see and to copy —
delete or replace it as you grow your real application.

---

## How a page is built — the one-page tour

When the user taps "Go to View 2":

1. The button calls `StackView.view.pushRoute("View2")`.
2. `AppShell` asks the `ViewNavigator` to resolve `"View2"`.
3. `ViewNavigator` constructs a new `View2ViewModel`, passing the
   `AppServices` bundle (a struct of pointers to shared domain instances)
   by const reference to its constructor.
4. `ViewNavigator` returns `{ url, viewModel }` to QML.
5. `AppShell` calls `StackView.push(url, { viewModel })`.
6. `View2.qml`'s `required property var viewModel` is satisfied by the
   freshly-built object. Property bindings start working immediately.

When the user pops back to View 1, the View 2 object is destroyed and
the JavaScript GC reclaims its `View2ViewModel` shortly after. Pushing
View 2 again creates a new instance with `enterCount` back to 1.

The shared domain instances inside `AppServices` (the page-shared state)
keep running independently of any view.

---

## The two entry points to `ui-kit/`

This is one of the things this template makes concrete. The same set of QML
files is valid input for two completely different tools:

### Entry point 1 — `qml` runtime (any developer's laptop)

```bash
cd ui-kit
qml forms/View1Form.ui.qml
```

Page 1 renders with its hardcoded defaults. No build. No backend. No board.
Edit the form, reload, see the result. This works on any laptop with Qt
installed and keeps working throughout the project's life as long as the
structural rules are followed.

### Entry point 2 — Qt Design Studio / Figma2Qt

```bash
# Open ui-kit/ui-kit.qmlproject in Qt Design Studio
```

The `ui-kit.qmlproject` file declares its own QML import paths and project
structure, so Qt Design Studio (and the Figma2Qt export plugin) can open
this folder directly — without needing the C++ build, the CMake project,
or any of the back-end layers.

This means:

- A designer using Figma can export to Qt Design Studio and have their work
  land in `ui-kit/forms/` as `.ui.qml` files.
- Those exact same `.ui.qml` files are then consumed by the main CMake build
  via `qt_add_qml_module` in `ui-kit/CMakeLists.txt` — no copying, no
  conversion, no dual maintenance.
- A C++ developer can change the back-end. A QML developer can change the
  layouts. A designer can change the visuals in Qt Design Studio. None of
  them block the others.

This is the bridge that, in real projects, is usually built ad-hoc, badly,
and one project at a time. Here it is the default.

---

## The five `CMakeLists.txt` files — a working reference

The whole project has exactly five CMake files, one per layer, each
following the same pattern:

| File | What it shows |
|---|---|
| `core/CMakeLists.txt` | A C++ QML module containing the reusable infrastructure (`ViewNavigator` `QML_SINGLETON`, `ViewModelBase` abstract type, `AppServices` struct). |
| `domain/CMakeLists.txt` | A pure C++ static library — the simplest possible Qt CMake target. |
| `viewmodels/CMakeLists.txt` | A C++ QML module via `qt_add_qml_module` with `QML_UNCREATABLE` page-scoped ViewModels. |
| `ui-kit/CMakeLists.txt` | A pure-QML module via `qt_add_qml_module` — no C++ link dependencies, only QML and resource files. |
| `app-main/CMakeLists.txt` | The `qt_add_executable` target that pulls all four modules together and provides the `App.Integration` QML entry module. |

The dependency arrow goes one way only:

```text
core ◄── domain
core ◄── viewmodels ◄── domain
core ◄── app-main ◄── viewmodels ◄── domain
ui-kit (consumed by app-main as a QML module — no project-owned C++ link)
```

No layer depends on the layer above it.

---

## Naming convention

This template follows the **Qt convention of avoiding C++ namespaces**.
Type names are unique on their own — `ViewNavigator`, `ViewModelBase`,
`AppServices`, `AppDataSource`, `View1ViewModel`, etc. Cross-folder
collisions are prevented by giving types specific, descriptive names
rather than relying on namespaces.

The **infrastructure / demo line is drawn by the folder, not by a
namespace**. `core/` is reusable infrastructure; everything else is
demonstration code you are expected to replace or extend.

---

## Quick start

### Preview the UI on your laptop — no build

```bash
cd ui-kit
qml forms/View1Form.ui.qml
```

Or open `ui-kit/ui-kit.qmlproject` in Qt Design Studio or Qt Creator for a
richer preview environment with live edit.

---

## Repository layout

```text
qt-quick-starter/
├── README.md
├── ARCHITECTURE.md
├── CMakeLists.txt                  # top-level
│
├── core/                           # ◄ Reusable infrastructure — do not edit
│   ├── ViewNavigator.h             #   route table + ViewModel factory
│   ├── ViewNavigator.cpp           #   (QML_SINGLETON in App.Core)
│   ├── ViewModelBase.h             #   abstract base for page ViewModels
│   ├── AppServices.h               #   struct of pointers — extend as you grow
│   └── CMakeLists.txt              #   qt_add_qml_module (URI: App.Core)
│
├── ui-kit/                         # Pure QML — designer-friendly
│   ├── ui-kit.qmlproject           # Qt Design Studio / Figma2Qt entry
│   ├── CMakeLists.txt              # qt_add_qml_module (QML-only)
│   ├── AppShell.qml                # StackView host + pushRoute helper
│   ├── forms/                      # Pure QML layouts (preview-able)
│   │   ├── View1Form.ui.qml
│   │   ├── View2Form.ui.qml
│   │   └── View3Form.ui.qml
│   ├── views/                      # ViewModel binding + navigation
│   │   ├── ViewBase.qml            # base type — wires lifecycle hooks
│   │   ├── View1.qml
│   │   ├── View2.qml
│   │   └── View3.qml
│   ├── components/                 # Reusable custom UI parts
│   │   └── AppButton.qml
│   └── previews/AppPreview.qml
│
├── app-main/                       # Entry point — wires everything
│   ├── Main.qml
│   ├── main.cpp                    # declares the route table
│   └── CMakeLists.txt              # qt_add_executable + App.Integration
│
├── viewmodels/                     # C++ ViewModels (one per view)
│   ├── View1ViewModel.h/.cpp
│   ├── View2ViewModel.h/.cpp
│   ├── View3ViewModel.h/.cpp
│   └── CMakeLists.txt              # qt_add_qml_module (with C++)
│
├── domain/                         # Pure C++ domain state
│   ├── AppDataSource.h
│   ├── AppDataSource.cpp
│   └── CMakeLists.txt              # plain C++ library
│
└── tests/                          # Host-runnable tests
    ├── domain/                     # plain QtTest — domain logic
    │   └── tst_app_datasource.cpp
    ├── viewmodels/                 # plain QtTest — VM behaviour + AppServices
    │   └── tst_view1_viewmodel.cpp
    ├── ui-visual/                  # QtQuickTest — form layout assertions
    │   ├── main.cpp
    │   └── tst_ui_kit.qml
    └── CMakeLists.txt
```

---

## Growing from here

This template is intentionally **minimal scaffolding**. The common ways to
extend it:

- **Add a new property to an existing page.** Add it to that page's
  `View*ViewModel.h/.cpp` and bind it in the matching `ui-kit/views/`
  file. No interface file to keep in sync, no mock to maintain.
- **Add a new page.** Three steps:
  1. Create `ViewNForm.ui.qml` in `ui-kit/forms/` and `ViewN.qml` in
     `ui-kit/views/`. Register both in `ui-kit/CMakeLists.txt`.
  2. Create `ViewNViewModel.h/.cpp` in `viewmodels/` deriving from
     `ViewModelBase`. Register them in `viewmodels/CMakeLists.txt`.
  3. Add one `navigator->registerRoute<ViewNViewModel>("ViewN", QUrl(...))`
     line in `app-main/main.cpp`.

  Then push to it from any view via
  `StackView.view.pushRoute("ViewN")`. No QML imports change.
- **Add a new C++ module.** Copy `viewmodels/CMakeLists.txt`, rename the
  module URI, register your new types. The pattern is the same for every
  C++ QML module you will ever add.
- **Connect a real data source.** Write a class under `domain/` (a database,
  a network client, a vendor API), forward-declare it in
  `core/AppServices.h` and add one `Foo *foo = nullptr;` line to the
  `AppServices` struct, then allocate it in `main.cpp` and assign it to
  `services.foo`. Every ViewModel that wants to consume it reads
  `m_services.foo`. No `core/` change beyond the one-line struct member,
  no `registerRoute<T>()` change, no signature change to other VMs. See
  `ARCHITECTURE.md` → "Wiring domain services" for the full worked
  example.

Through all of these, `ui-kit/forms/` keeps loading in `qml` and in Qt
Design Studio. The designer's workflow is not blocked by the C++ work, and
the C++ work is not blocked by the design iteration.

---
