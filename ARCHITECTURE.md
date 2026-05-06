# Architecture

This document explains the principles and the layers that make up the **Qt Quick
Starter Application** template. The opinions here are deliberate — they are
what makes the template actually solve the problem.

---

## Five things this architecture gives you on day one

Before describing the layers, here are the five concrete things this structure
delivers. The rest of the document is just how each of them is implemented.

1. **A clear UI / logic separation.** QML layouts and C++ logic live in
   different folders that connect through one well-defined layer
   (the ViewModel). You do not have to read a paper on architectural
   patterns to understand it — the folder names tell you where things go.
2. **A UI preview that keeps working.** The QML in `ui-kit/forms/` loads
   standalone in the `qml` runtime and in Qt Design Studio, with no build,
   no backend, no target board. This is true on day one and stays true on
   day 700, because the structural rules below make it impossible to
   accidentally couple a form file to a C++ module.
3. **A working Figma → Qt Design Studio → CMake bridge.** The same QML files
   are valid input to Qt Design Studio (via `ui-kit.qmlproject`) and to the
   main CMake build (via `ui-kit/CMakeLists.txt` and `qt_add_qml_module`).
   No copy step. No conversion step. No second source of truth.
4. **A CMake reference for QML modules.** Four small `CMakeLists.txt` files
   show how to use `qt_add_qml_module` for a pure-QML module, for a C++
   QML module, for a plain C++ library, and for an executable that ties
   them together. The dependency direction is enforced by where the
   `target_link_libraries` calls live.
5. **A way in for teams that find CMake hard.** The four CMake files are
   short and follow the same shape, so a developer who has never written
   `qt_add_qml_module` can copy and adapt them rather than starting from
   the documentation.

The rest of this document explains how each of these is implemented.

---

## A note on the name

The template is called Qt Quick **Starter** Application, not Qt Quick MVVM
Application. The structure described here is, in practice, close to MVVM —
the `ui-kit/` layer is the View, `viewmodels/` is the ViewModel, `domain/`
is the Model. We deliberately do not use the pattern name as the product name.

The primary audience is developers new to Qt — typically arriving through
commercial channels rather than the open-source community. For that
audience, a pattern name like MVVM adds vocabulary without helping them
get started. "Starter" tells them what this is: a place to start.

A developer who already knows MVVM will recognise the structure immediately.
A developer who does not will still benefit from it just by using it.

---

## Naming convention

This template follows the **Qt convention of avoiding C++ namespaces**.
Type names are unique on their own — `AppDataViewModel`,
`AppDataSource`, etc. Cross-folder name collisions are prevented by
giving types specific, descriptive names rather than relying on
namespace machinery.

QML module URIs (`App.UiKit`, `App.ViewModels`, `App.Integration`) are a
different mechanism — those are *QML namespaces* used by the QML engine
for type lookup, and they stay. The convention here is only about C++
`namespace` blocks.

The smaller the project, the heavier extra vocabulary feels. This main
branch is the smallest version of the starter, and namespace-free C++
is part of keeping it light.

---

## A note on scope — when this branch is enough

This branch ships a single `AppDataViewModel` `QML_SINGLETON` shared by
all views. That is the simplest thing that works, and it scales well
to roughly five or six pages.

For projects that grow past that point, see the sibling branch
`view-navigator`. It applies the same structural rules
(UI / logic split, host-testable UI, Figma bridge, modular CMake) but
replaces the single ViewModel with one ViewModel per page, a small
`ViewNavigator` that constructs them on push, lifecycle hooks, and an
`AppServices` struct for domain injection. The growth criteria are in
`README.md` → "Growing from here". Stay on this branch unless your page
count or shared-state pressure actually justifies the extra structure.

---

## The four layers

### Layer 1 — `ui-kit/` — pure QML

Everything here is valid input to the standard `qml` runtime that ships
with Qt. It uses only standard Qt modules (`QtQuick`, `QtQuick.Controls`,
`QtQuick.Layouts`) — no C++ code, no project-owned plugins, no
dependencies that only exist in the full build.

Every file under `ui-kit/` loads in any developer's laptop with Qt
installed — without a target board, without a backend, without an
application build. This is the single property the entire template is
designed to protect.

Structure:

- **`forms/`** — pure layout files (`.ui.qml`). No knowledge of any
  ViewModel. They expose child element properties via `property alias` for
  external binding. They contain hardcoded defaults that act as the visible
  state when the form is loaded standalone.
- **`views/`** — thin wrappers around the form files. Take a ViewModel via
  `required property var viewModel` and bind it to the form aliases. Handle
  StackView navigation directly via `StackView.view.push/pop`.
- **`components/`** — reusable custom UI parts (e.g. `AppButton.qml`).
  Built on `QtQuick.Templates` to bypass whichever Qt Quick Controls style
  is active. This keeps appearance consistent across target platforms.
- **`AppShell.qml`** — the navigation host. Owns the `StackView`,
  instantiates the initial page, and passes the ViewModel down.
- **`previews/AppPreview.qml`** — entry point used for developer preview.
  Renders the forms directly — no ViewModel, no navigation logic.

This folder also includes a `ui-kit.qmlproject` file that lets the team
open it in Qt Design Studio if they want to. But Qt Design Studio is *not
required*. Any `qml` runtime is enough.

### Layer 2 — `app-main/` — integration QML + executable

Contains `Main.qml` (the only QML file that may import project-owned C++
modules) and `main.cpp`, and the CMake target. `main.cpp` retrieves the
engine-managed `AppDataViewModel` singleton via
`engine.singletonInstance<>()` and calls `bindDataSource()` before QML
loads.

### Layer 3 — `viewmodels/` — C++ ViewModels

C++ classes registered via `QML_ELEMENT` and `QML_SINGLETON`, grouped into
the `App.ViewModels` module. ViewModels expose UI-shaped state to QML
through `Q_PROPERTY` and `Q_INVOKABLE`.

The ViewModel is the single source of truth for the QML contract. The
property names and method signatures in the C++ header *are* the contract —
there is no separate QML interface file to keep in sync. Views consume
this contract via the duck-typed `required property var viewModel`.

ViewModels hold UI-shaped state. They do not contain business logic or I/O —
that is delegated to `domain/` objects.

### Layer 4 — `domain/` — pure C++

Domain state and (in a real application) the repositories. No QtQuick, no
QML. Compiles and runs without a display. Fully unit-testable with plain
`QtTest`. The template provides one `AppDataSource` that you can extend or
replace with your own domain objects.

The dependency direction is strictly one-way:

```text
domain  ◄── viewmodels  ◄── app-main  ◄── ui-kit (as a QML module)
```

`domain` knows nothing about ViewModels. ViewModels know nothing about QML
layouts. Forms know nothing about ViewModels.

---

## How the Figma / Qt Design Studio bridge works

This is one of the things this template makes concrete, so it deserves its
own section.

### The setup

`ui-kit/ui-kit.qmlproject` is a small declarative file (a few lines) that
tells Qt Design Studio:

- where the QML files live (the `ui-kit/` folder itself)
- which import paths to use (only standard Qt modules)
- which file to use as the preview entry point (`previews/AppPreview.qml`)

Because `ui-kit/` only uses standard Qt modules, Qt Design Studio can open
this file *without* the rest of the project being built.

### What this enables

A typical embedded HMI workflow looks like this:

1. **Designer in Figma** — works on visual design and exports to Qt Design
   Studio using the Figma2Qt plugin.
2. **Qt Design Studio** — receives the exported design as a `.ui.qml`
   file, lets the designer further refine layout and timing, and saves
   directly into `ui-kit/forms/`.
3. **CMake build** — the same `.ui.qml` files are picked up by
   `qt_add_qml_module` in `ui-kit/CMakeLists.txt`, compiled into the
   `App.UiKit` QML module, and consumed by `Main.qml` in `app-main/`.

The same files are used in all three steps. There is no copy step, no
conversion step, no parallel "code version" of the design.

### What this prevents

Without this bridge, projects typically fall into one of two failure modes:

- **The "designer's QML never runs"** — the designer produces QML in Qt
  Design Studio that uses non-standard imports, custom C++ types, or paths
  that only resolve in the design tool. The developer reimplements
  everything by hand. The two diverge.
- **The "developer's QML breaks Design Studio"** — the developer adds C++
  imports and singleton references throughout the QML. Qt Design Studio
  can no longer open the files. The designer can no longer edit them.

This template's structural rules — `ui-kit/` may not import project-owned
C++ modules — eliminate both failure modes by construction. The designer's
files always open in Qt Design Studio. The developer's files always
compile in CMake. They are the same files.

---

## How the four `CMakeLists.txt` files work

This is the second concrete thing this template makes a working reference
for. Each layer has its own `CMakeLists.txt`, and reading them in order is
a short tutorial on modern Qt 6 CMake.

### `domain/CMakeLists.txt` — plain C++

The simplest layer. A static library, no Qt-specific QML magic. Just:

- `qt_add_library(app_domain STATIC ...)`
- A list of `.cpp` and `.h` files
- `target_link_libraries(app_domain PUBLIC Qt6::Core)`

Anyone who has used CMake at all will recognise this. It is here to
establish the pattern: each layer is its own CMake target, with explicit
sources and explicit dependencies.

### `viewmodels/CMakeLists.txt` — C++ QML module

This is where `qt_add_qml_module` shows up for the first time. It
demonstrates:

- How to declare a QML module URI (`App.ViewModels`)
- How to register C++ classes with `QML_ELEMENT` / `QML_SINGLETON`
- How to link to `domain/` (`PRIVATE`, not `PUBLIC` — the layers above
  do not need to see domain types)

If you have ever wondered where `qt_add_qml_module(... TYPES ...)` should
live in a multi-module project, this is the answer.

### `ui-kit/CMakeLists.txt` — pure-QML module

This is the one that surprises some teams. A `qt_add_qml_module` call
*without* any C++ source files. Just QML and resources. Demonstrates:

- That a QML module does not require C++ to be a module
- How to list `.ui.qml`, `.qml`, and resource files
- That the module has no `target_link_libraries` to project C++ targets —
  this is the structural rule that keeps `ui-kit/` previewable

A common mistake in real projects is to add a small "helper" C++ class to
the UI module and link it. Once that link exists, the UI module is no
longer openable in Qt Design Studio without the full build. This file is
deliberately structured so that mistake is visible the moment it happens.

### `app-main/CMakeLists.txt` — the executable

`qt_add_executable` plus links to the three module libraries. This is
where everything comes together. Demonstrates:

- How to register the application as a QML module of its own
  (`App.Integration`) — the only module allowed to bridge C++ and QML
- How to link all three other modules
- Where the `main.cpp` entry point lives

### Why this matters for teams that find CMake hard

Most Qt projects in the field have one giant top-level `CMakeLists.txt`
with everything in it. That works for very small projects but becomes
unreadable fast. The modular alternative — one `CMakeLists.txt` per
folder — is documented in Qt's own docs but is rarely shown as a worked
example for a complete project.

This template is that worked example. Four files. Same shape. Each one
does one thing. A developer who copies them and renames them will end up
with a correctly-structured Qt 6 CMake project, even if they have not
internalised every option of `qt_add_qml_module` yet.

---

## Deployment

The template ships with explicit `install(...)` rules so that
`cmake --install` produces a deployment tree ready to copy onto an
embedded target (Boot2Qt-style devices, custom Yocto images, or any
Linux sysroot that already has Qt installed).

### What the install layout looks like

After `cmake --install build --prefix /opt/qt-quick-starter`:

```text
/opt/qt-quick-starter/
├── bin/
│   └── qt-quick-starter             # executable, with App.Integration
│                                   # QML embedded as a resource
├── lib/                            # (or lib64/, see GNUInstallDirs)
│   └── libapp_viewmodels.so        # C++ backing for App.ViewModels
└── qml/
    └── App/
        ├── Integration/
        │   └── qmldir                  # module-discovery metadata
        ├── UiKit/
        │   ├── qmldir
        │   ├── ui_kit_module.qmltypes
        │   └── libui_kit_moduleplugin.so
        └── ViewModels/
            ├── qmldir
            ├── app_viewmodels.qmltypes
            └── libapp_viewmodelsplugin.so
```

Qt itself (the QtQuick / QtQuick.Controls / QtQuick.Layouts modules) is
not part of this tree. The deployment assumes Qt is already installed on
the device image — which is the case for any Boot2Qt build, and for
custom Yocto images that include the Qt layer. If you need to ship Qt
too, that is a different deployment tool's job (`qt_generate_deploy_app_script`,
`windeployqt`, etc.); this template stays out of that.

### Why per-module install rules instead of a single top-level block

Each layer's `CMakeLists.txt` declares its own install rules, next to the
target definition. The same way each layer is responsible for its own
build rules, each layer is responsible for its own deployment rules.
When you add a new module by copying `viewmodels/CMakeLists.txt`, the
install lines come along with it — there is no separate file to remember
to edit.

The top-level `CMakeLists.txt` only sets two things related to install:

- `include(GNUInstallDirs)` — portable directory variables
  (`CMAKE_INSTALL_BINDIR`, `CMAKE_INSTALL_LIBDIR`) that adapt to the
  target distribution. On systems where 64-bit libraries live in `lib64`
  (Fedora, RHEL), `CMAKE_INSTALL_LIBDIR` resolves to `lib64`; on Debian,
  Ubuntu, and most embedded sysroots it resolves to `lib`. Hard-coding
  `"lib"` works for embedded use but breaks Linux desktop packaging.
- `QQS_QML_INSTALL_DIR` — a single cache variable for where QML modules
  go. Defaults to `qml`; override with
  `cmake -DQQS_QML_INSTALL_DIR=share/qt6/qml ...` if your sysroot uses a
  different convention.

### Why .qml source files are not deployed

`qt_add_qml_module` compiles every `.qml` and `.ui.qml` in `QML_FILES`
into the plugin shared library and embeds them as Qt resources. The
generated `qmldir` contains:

```text
prefer :/qt/qml/App/UiKit/
```

which tells the QML engine to load the embedded copies in preference to
anything on disk. Source `.qml` files alongside the plugin would be
ignored at runtime — so the install rules deliberately exclude them and
ship only the artefacts the engine actually loads:

- `qmldir` — module-discovery metadata
- `*.qmltypes` — type information for tooling (small, included for
  on-device debugging convenience; not strictly required at runtime)
- the plugin shared library

This keeps the deployed tree minimal and makes "what runs on the device"
clearer.

If you ever need source `.qml` on the device for live-reload debugging or
for Qt Design Studio's connect-to-device preview, add `*.qml` and
`*.ui.qml` back to the `FILES_MATCHING` patterns for that session. For
production images, leave them out.

### Why the static library has no install rule

`app_domain` is a `STATIC` library: its object code is linked directly
into every target that depends on it (the executable and
`app_viewmodels`). There is no separate `.so` to ship — the domain code
already travels inside whatever links it. `domain/CMakeLists.txt`
therefore has no `install()` block. If you ever switch `app_domain` to
`SHARED`, add an `install(TARGETS app_domain ...)` block matching the
one in `viewmodels/CMakeLists.txt`.

### How the executable finds QML modules at runtime

`main.cpp` calls:

```cpp
engine.addImportPath(
    QDir(app.applicationDirPath()).absoluteFilePath("../qml"));
```

This resolves to `<install-prefix>/qml/` based on the executable's own
location, not on environment variables. The deployed binary works
wherever the install tree lands — no launcher script setting
`QML_IMPORT_PATH`, no systemd unit threading environment through, no
`/etc/profile.d/` hook. A self-contained tarball or a single rsync of
the install tree is enough to run.

This path expression assumes the install layout above (`bin/` next to
`qml/`). If you change the install layout, change this line to match —
it is the one place the executable is told how to find QML modules.
The expression also resolves correctly inside the build tree, so
launching from `build/app-main/qt-quick-starter` during development
works without an install step.

---

## The host-testable UI workflow

> **`ui-kit/forms/` is the developer's workspace for UI iteration. Every
> form file loads standalone on the developer's laptop, throughout the
> project's life.**

Open a single form with the standard `qml` runtime:

```bash
qml ui-kit/forms/View1Form.ui.qml
```

You see the layout with its hardcoded defaults — no ViewModel, no backend,
no full build. Adjust spacing, colour, alignment, reload, see the result
immediately.

When new UI is needed:

1. Edit or create a `Form.ui.qml` file under `ui-kit/forms/` — instantly
   verifiable with `qml`.
2. Expose new elements via `property alias` in the form.
3. Bind the alias to the ViewModel in the corresponding `views/` file.
4. Add the supporting property to `AppDataViewModel.h/.cpp`.

Steps 1–2 happen entirely on the developer's laptop, in seconds per
iteration, without ever touching the target board. Step 4 happens when the
C++ side is ready, on its own schedule. Neither blocks the other.

---

## The host-testable UI rule

> **`ui-kit/` must be loadable on the developer's own machine, with
> nothing more than the `qml` runtime, throughout the project's life.**

This is upheld by convention and code review, not by tooling. The
expectations are simple:

1. No QML file under `ui-kit/` imports anything beyond standard Qt modules
   and relative-path imports.
2. No QML file under `ui-kit/` imports `App.ViewModels` or
   `App.Integration` (the project-owned C++ modules).
3. `ui-kit/CMakeLists.txt` does not link to any of the `app_domain`,
   `app_viewmodels`, or `app-main` targets.

A team that wants to automate the check can add a small CI script. The
template intentionally does not ship one, to keep the scaffolding small.

---

## The required-property injection pattern

Views never import the ViewModel directly. Instead:

```qml
// ui-kit/views/View1.qml
View1Form {
    id: root
    required property var viewModel    // a contract, not a type

    statusText: viewModel.value + " | " + viewModel.status
    goView2Btn.onClicked: StackView.view.push(
        Qt.resolvedUrl("View2.qml"), { viewModel: root.viewModel })
}
```

The parent decides what to inject:

```qml
// ui-kit/previews/AppPreview.qml — developer preview
View1Form { anchors.fill: parent }     // no ViewModel needed, form is self-contained
```

```qml
// app-main/Main.qml — real application
AppShell { viewModel: AppDataViewModel }    // C++ singleton injected here
```

Because the view sees only duck-typed properties, **the same view file works
with any object** that exposes the right properties.

---

## Navigation

Views handle navigation directly via the `StackView` attached property. No
signals, no coordinators:

```qml
// ui-kit/views/View2.qml
backBtn.onClicked:    StackView.view.pop()
goView3Btn.onClicked: StackView.view.push(Qt.resolvedUrl("View3.qml"),
                                          { viewModel: root.viewModel })
```

`AppShell.qml` only provides the `StackView` and sets up the initial page.
It does not know about the rest of the navigation graph.

---

## Test strategy

### Host-side UI layout tests — `tests/ui-visual/`

`QtQuickTest` tests that instantiate form files directly. No ViewModel
needed, no C++ needed. Verify alias properties, default values, presence
of elements:

```qml
View1Form { id: form }
TestCase {
    function test_default_status() {
        compare(form.statusText, "value: 0  |  idle")
    }
}
```

Because the form files have no ViewModel dependency, **these tests do not
need to be updated when the ViewModel grows**. ViewModel behaviour is
covered by C++ unit tests instead.

### Domain unit tests — `tests/domain/`

Plain `QtTest` for `domain/` classes. No display, no QML. Run on any
runner.

### Deliberately out of scope

- Target-side integration tests (board-in-the-loop).
- Screenshot-diff visual regression tests — easy to add on top of
  `tests/ui-visual/` with `QQuickItemGrabResult`.

---

## What the rules take away from you

An honest list of what you give up:

- **You cannot use a C++ singleton directly in a `ui-kit` view.** Route
  it through the ViewModel that the view receives via `required property`.
- **You cannot register a custom QML element in C++ and use it in
  `ui-kit`.** Keep it in `app-main/Main.qml` and inject it as a property,
  or write a pure-QML stand-in for preview.
- **You cannot use `QtLocation` / `QtMultimedia` in `ui-kit` if the local
  environment does not have those plugins.** Route through the ViewModel.

Each restriction buys you back the ability to test the UI on the
developer's laptop — which is what makes the first two weeks productive
*and* the next two years bearable.

---

## Glossary

| Term | Meaning |
| --- | --- |
| **Host** | The developer's laptop — not the target board |
| **Target** | The device or board where the final app runs |
| **Pure QML** | QML loadable by the standard `qml` runtime — no custom C++ |
| **Bound QML** | QML that imports project-owned C++ modules |
| **Form** | A `.ui.qml` file containing pure layout with `property alias` exposure |
| **View** | A `.qml` wrapper that binds a ViewModel to a Form and handles navigation |
| **ViewModel** | A C++ `QML_SINGLETON` class exposing domain state to QML |