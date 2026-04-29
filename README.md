# Qt Quick Starter Application

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
4. **A CMake reference for QML modules** — four small `CMakeLists.txt` files
   showing how to use `qt_add_qml_module`, how to keep dependencies
   one-directional, and how to wire a QML-only module to a C++ module to an
   executable. If you have ever stared at `qt_add_qml_module` documentation
   and wondered "but where do I put it in my project?" — this template
   answers that.
5. **A way in for teams that find CMake hard** — the four CMake files are
   short, commented, and follow the same pattern. You can copy them, rename
   them, and grow your project from there without becoming a CMake expert
   first.

This is not a finished application. It is roughly 30 files of scaffolding,
deliberately minimal, designed to be the first commit of a real project.

> **Larger project?** A sibling branch — `feature/routed-viewmodels` —
> applies the same structural rules but with one ViewModel per page,
> a small `ViewNavigator`, and a services-injection pattern. Reach for
> it when your page count starts to make a single shared ViewModel feel
> crowded. See "Growing from here" at the bottom of this document.

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

This template answers all of these in one consistent structure. The same folder
layout that gives you UI preview also gives you the Figma bridge, the CMake
module example, and the layered structure that scales.

---

## What you get

```text
┌─ ui-kit/ ─────────────────────────────────────────────────────┐
│                                                               │
│   forms/    Pure QML layouts — preview-able standalone        │
│      ↑  property alias bindings                               │
│   views/    ViewModel binding + StackView navigation          │
│                                                               │
│   ui-kit.qmlproject  ← Qt Design Studio / Figma2Qt opens this │
└───────────────────────────┬───────────────────────────────────┘
                            │  required property var viewModel
                ┌───────────▼────────────┐
                │      viewmodels/        │  C++ QML_SINGLETON
                └───────────┬────────────┘
                            │  bindDataSource()
                ┌───────────▼────────────┐
                │       domain/           │  Pure C++
                └────────────────────────┘

        app-main/  →  entry point, wires viewmodels ↔ domain

        Each box = its own CMakeLists.txt (4 files total)
```

The sample is intentionally small — three pages in a `StackView`
(`View1` → `View2` → `View3`), one value property, one status string, a few
log messages. The structure is the product; the sample exists only to make
the structure easy to see and to copy.

The ViewModel exposes three properties (`value`, `status`, `lastVisitedPage`)
and one invokable method (`notifyPageEntered`). They are logged to the
console on every page transition so you can see the property flow end-to-end.

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

## The four `CMakeLists.txt` files — a working reference

The whole project has exactly four CMake files, one per layer, each
following the same pattern. Reading them in order is a short tutorial on
modern Qt 6 CMake:

| File | What it shows |
|---|---|
| `domain/CMakeLists.txt` | A pure C++ static library — the simplest possible Qt CMake target. |
| `viewmodels/CMakeLists.txt` | A C++ QML module via `qt_add_qml_module` with `QML_SINGLETON` registration. |
| `ui-kit/CMakeLists.txt` | A pure-QML module via `qt_add_qml_module` — no C++ link dependencies, only QML and resource files. |
| `app-main/CMakeLists.txt` | The `qt_add_executable` target that pulls all three modules together. |

The dependency arrow goes one way only: `domain → viewmodels → app-main`,
with `ui-kit` consumed by `app-main` as a QML module. No layer depends on
the layer above it. Each `CMakeLists.txt` is short enough to read in one
sitting, commented to explain *why* each line is there, and named so you can
copy any of them as a starting point for a new module of your own.

If your team has been writing Qt projects with one giant `CMakeLists.txt`
at the top and `find_package(Qt6 ...)` scattered around, this is what the
modular alternative looks like.

---
## Quick start

### Preview the UI on your laptop — no build

```bash
cd ui-kit
qml forms/View1Form.ui.qml
```

Or open `ui-kit/ui-kit.qmlproject` in Qt Design Studio, Qt Creator for a richer
preview environment with live edit.

---

## Repository layout

```text
qt-quick-starter/
├── README.md
├── ARCHITECTURE.md
├── CMakeLists.txt                  # top-level
│
├── ui-kit/                         # Pure QML — designer-friendly
│   ├── ui-kit.qmlproject           # Qt Design Studio / Figma2Qt entry
│   ├── CMakeLists.txt              # qt_add_qml_module (QML-only)
│   ├── AppShell.qml                # StackView host
│   ├── forms/                      # Pure QML layouts (preview-able)
│   │   ├── View1Form.ui.qml
│   │   ├── View2Form.ui.qml
│   │   └── View3Form.ui.qml
│   ├── views/                      # ViewModel binding + navigation
│   │   ├── View1.qml
│   │   ├── View2.qml
│   │   └── View3.qml
│   ├── components/                 # Reusable custom UI parts
│   │   └── AppButton.qml
│   └── previews/AppPreview.qml
│
├── app-main/                       # Entry point — wires everything
│   ├── Main.qml
│   ├── main.cpp
│   └── CMakeLists.txt              # qt_add_executable
│
├── viewmodels/                     # C++ ViewModels (QML_SINGLETON)
│   ├── AppDataViewModel.h
│   ├── AppDataViewModel.cpp
│   └── CMakeLists.txt              # qt_add_qml_module (with C++)
│
├── domain/                         # Pure C++ domain state
│   ├── AppDataSource.h
│   ├── AppDataSource.cpp
│   └── CMakeLists.txt              # plain C++ library
│
└── tests/                          # Domain + UI tests
```

---

## Growing from here

This template is intentionally **minimal scaffolding**. The common ways to
extend it:

- **Add a new property.** Add it to `AppDataViewModel.h/.cpp` and bind it
  in the relevant `ui-kit/views/` file. No interface file to keep in sync,
  no mock to maintain.
- **Add a new page.** Create `View4Form.ui.qml` in `ui-kit/forms/`, then
  `View4.qml` in `ui-kit/views/`. Register both in
  `ui-kit/CMakeLists.txt`. Push to it from a view via
  `StackView.view.push(Qt.resolvedUrl(...))`.
- **Add a new C++ module.** Copy `viewmodels/CMakeLists.txt`, rename the
  module URI, register your new types. The pattern is the same for every
  C++ QML module you will ever add.
- **Connect a real data source.** Write a class under `domain/` (a database,
  a network client, a vendor API), and wire it in
  `AppDataViewModel::bindDataSource` exactly the way the current
  `AppDataSource` is wired.

Through all of these, `ui-kit/forms/` keeps loading in `qml` and in Qt
Design Studio. The designer's workflow is not blocked by the C++ work, and
the C++ work is not blocked by the design iteration.

### When `main` is not enough — the `feature/routed-viewmodels` branch

The single-singleton ViewModel pattern on this branch is the smallest
thing that works. It scales well to roughly five or six pages. Past
that, two failure modes typically appear:

- **Merge conflict gravity.** Every page's properties live in one
  `AppDataViewModel` class. As the page count grows, the ViewModel file
  becomes the most-edited file in the project, and unrelated work
  collides there.
- **Page-scoped state has nowhere to live.** A counter, a draft, a
  per-page selection — there is no clean home for it. It either pollutes
  the shared ViewModel or leaks into QML state.

The sibling branch `feature/routed-viewmodels` applies the same
structural rules (UI / logic split, host-testable UI, Figma bridge,
modular CMake) but replaces the single ViewModel with:

- One `View*ViewModel` C++ class per page, derived from `ViewModelBase`,
  instantiated by a small `ViewNavigator` on each `pushRoute()` call.
- Six lifecycle hooks (`onInitialize`, `onActivating`, `onActivated`,
  `onDeactivating`, `onDeactivated`, `onFinalize`) so C++ resource
  setup and teardown synchronise with what the user is actually seeing.
- An `AppServices` struct that bundles long-lived domain pointers, so
  adding a new domain class does not change any ViewModel constructor
  signature.
- A `core/` folder that holds the reusable infrastructure separately
  from demo code.

It is more code (~50 files instead of ~30) and more vocabulary to learn.
It earns its weight only when the project actually has many pages.
**Stay on this branch if your project has a small number of pages and
a shared state model. Switch to `feature/routed-viewmodels` when you
start feeling the symptoms above.**

---