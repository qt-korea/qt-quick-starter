# Architecture

This document explains the principles and the layers that make up the **Qt Quick
Starter Application** template. The opinions here are deliberate — they are
what makes the template actually solve the problem.

---

## Six things this architecture gives you on day one

Before describing the layers, here are the six concrete things this structure
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
4. **A CMake reference for QML modules.** Five small `CMakeLists.txt` files
   show how to use `qt_add_qml_module` for a pure-QML module, for a C++
   QML module, for a plain C++ library, and for an executable that ties
   them together. The dependency direction is enforced by where the
   `target_link_libraries` calls live.
5. **An explicit infrastructure / demo split.** The reusable infrastructure
   (`ViewNavigator`, `ViewModelBase`, `AppServices`) lives in `core/`.
   Everything else is demonstration code, expected to be replaced or
   extended. The folder boundary draws the line.
6. **A way in for teams that find CMake hard.** The five CMake files are
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
Type names are unique on their own — `ViewNavigator`, `ViewModelBase`,
`AppServices`, `AppDataSource`, `View1ViewModel`, etc. Cross-folder name
collisions are prevented by giving types specific, descriptive names
rather than relying on namespaces.

The **infrastructure / demo line is drawn by the folder, not by a
namespace**:

- `core/` is reusable infrastructure. Treat it as a small in-tree library:
  depend on it, do not edit it.
- `domain/`, `viewmodels/`, `ui-kit/`, `app-main/` are demonstration code
  that you are expected to replace or extend with your own.

---

## A note on scope — when this branch may be too much

This branch ships per-page ViewModels constructed by a small
`ViewNavigator`, six lifecycle hooks, an `AppServices` injection
struct, and an explicit `core/` folder. That structure earns its
weight on projects with many pages, page-scoped state, or growing
domain wiring — places where a single shared ViewModel would
eventually become the project's largest merge-conflict surface.

For smaller projects (roughly five or six pages, mostly shared state),
the sibling branch `main` may be a better starting point. It applies
the same structural rules — UI / logic split, host-testable UI, Figma
bridge, modular CMake — but with a single shared `AppDataViewModel`
`QML_SINGLETON` instead of per-page ViewModels. Roughly 30 files
instead of 50, four `CMakeLists.txt` instead of five, and less
vocabulary to learn upfront.

The two branches are siblings, not generations. Pick the one whose
shape matches the project you are starting today; switch later if the
project outgrows it.

---

## The five layers

### Layer 1 — `core/` — reusable infrastructure (C++ QML module)

The plumbing that every page-based Qt Quick app needs:

- **`ViewModelBase`** — abstract base class for page-scoped ViewModels.
  Six virtual lifecycle hooks wired to the StackView page lifecycle.
- **`AppServices`** — plain struct of non-owning pointers to long-lived
  domain services. The single argument every ViewModel constructor takes.
- **`ViewNavigator`** — `QML_SINGLETON` that owns the route table and
  constructs the right ViewModel for each pushed page.

Registered as the QML module `App.Core`. The other layers depend on it;
nothing inside `core/` depends on the other layers. The starter is
designed so a real project rarely needs to edit anything in this folder.

### Layer 2 — `ui-kit/` — pure QML

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
- **`views/`** — thin wrappers around the form files. Inherit from
  `ViewBase.qml`, which receives a ViewModel via
  `required property var viewModel` and wires the lifecycle hooks. Place
  the page's `Form` as a filling child. Trigger navigation by calling
  `StackView.view.pushRoute("ViewN")` or `StackView.view.pop()`.
- **`components/`** — reusable custom UI parts (e.g. `AppButton.qml`).
  Built on `QtQuick.Templates` to bypass whichever Qt Quick Controls style
  is active. This keeps appearance consistent across target platforms.
- **`AppShell.qml`** — the navigation host. Owns the `StackView`, exposes
  a `pushRoute(name)` helper, and receives the navigator as a duck-typed
  `required property var navigator`. It does not import any project-owned
  C++ module.
- **`previews/AppPreview.qml`** — entry point used for developer preview.
  Renders the forms directly — no ViewModel, no navigator, no navigation
  logic.

This folder also includes a `ui-kit.qmlproject` file that lets the team
open it in Qt Design Studio if they want to. But Qt Design Studio is *not
required*. Any `qml` runtime is enough.

### Layer 3 — `viewmodels/` — C++ ViewModels (page-scoped instances)

C++ classes registered via `QML_ELEMENT` and `QML_UNCREATABLE`, grouped
into the `App.ViewModels` module. There is exactly one ViewModel class per
view (`View1ViewModel`, `View2ViewModel`, `View3ViewModel`), each derived
from the `ViewModelBase` defined in `core/`.

ViewModels expose UI-shaped state to QML through `Q_PROPERTY` and
`Q_INVOKABLE`. The property names and method signatures in the C++ header
*are* the contract — there is no separate QML interface file to keep in
sync. Views consume this contract via the duck-typed
`required property var viewModel`.

ViewModels hold UI-shaped state. They do not contain business logic or I/O —
that is delegated to `domain/` objects. Every ViewModel constructor takes
the same single argument — a `const AppServices &` bundle — and picks the
services it actually needs. Adding a new domain class does not change any
existing ViewModel's signature; it only changes the bundle definition.
See "Wiring domain services" below.

`QML_UNCREATABLE` is deliberate: QML knows the type metadata (so property
bindings work) but cannot construct instances. **Every instance flows
through the `ViewNavigator`**, which keeps construction in one place and
prevents QML files from accidentally instantiating one with the wrong
dependencies.

### Layer 4 — `domain/` — pure C++

Domain state and (in a real application) the repositories. No QtQuick, no
QML. Compiles and runs without a display. Fully unit-testable with plain
`QtTest`. The starter ships with one demonstrative `AppDataSource`. New
domain classes go here. See "Wiring domain services" below for how they
are exposed to ViewModels.

### Layer 5 — `app-main/` — integration QML + executable

Contains `Main.qml` (the only QML file that imports project-owned C++
modules) and `main.cpp`. This is the single place where:

- the **`AppServices` bundle is constructed** (each domain service is
  allocated and assigned to the bundle);
- the **route table is declared** (one `registerRoute<TViewModel>(name, url)`
  call per page);
- the `ViewNavigator` (the `QML_SINGLETON` defined in `core/`) is given
  the bundle to inject into every ViewModel it later constructs.

`Main.qml` simply hands the navigator to `AppShell` via a property.

The dependency direction is strictly one-way:

```text
core ◄── domain
core ◄── viewmodels ◄── domain
core ◄── app-main ◄── viewmodels ◄── domain
ui-kit (consumed by app-main as a QML module — no project-owned C++ link)
```

`core/` knows nothing about ViewModels. ViewModels know nothing about QML
layouts. Forms know nothing about ViewModels.

---

## Page-scoped ViewModels and the ViewNavigator

The starter uses **one ViewModel class per view**, with a `ViewNavigator`
that handles instantiation:

```text
View1.qml     ↔     View1ViewModel       (page-scoped state)
View2.qml     ↔     View2ViewModel
View3.qml     ↔     View3ViewModel
                          │
                          ▼  const AppServices &
                  +---------------------+
                  |   AppServices       |     (shared, page-shared state)
                  |---------------------|
                  |  AppDataSource *    |
                  |  (future) AuthState |
                  |  (future) Settings  |
                  +---------------------+
```

The alternative — a single shared ViewModel singleton — works for three
pages but does not scale: a real application with ten or twenty screens
ends up with one ViewModel class holding the state of every screen, and
that class becomes the single largest source of merge conflicts in the
project. The per-page model keeps page state isolated while shared
domain state stays shared via the `AppServices` bundle.

### How a navigation actually flows

When a user taps the "Go to View 2" button on View 1:

1. The button's `onClicked` calls `StackView.view.pushRoute("View2")`.
2. `AppShell`'s `pushRoute()` helper calls
   `navigator.resolveRoute("View2")`.
3. The `ViewNavigator` looks up `"View2"` in its route table and:
   - constructs a fresh `View2ViewModel`, passing the `AppServices`
     bundle by const reference to its constructor (the VM stores its
     own copy of the small struct of pointers);
   - sets the new ViewModel's QML ownership to `JavaScriptOwnership`;
   - returns `{ url, viewModel }` to QML.
4. `AppShell` calls `StackView.push(url, { viewModel })` — the View
   receives the freshly built ViewModel via its `required property`.
5. The View runs through the lifecycle hooks (see next section).
6. When the user later pops the page, the View object is destroyed; the
   QML JavaScript GC reclaims the ViewModel shortly afterwards.

A second push of `"View2"` later in the session creates a *new*
`View2ViewModel` — its `enterCount` is 1 again, but
`AppDataSource::value` continues from wherever it was. Page-scoped state
is per-instance; shared domain state is shared.

### ViewModel lifecycle

`ViewModelBase` exposes six virtual hooks that mirror the StackView page
lifecycle. The View wires them once; concrete ViewModels override only
the ones they care about.

| Hook | Fires when | Typical use |
|---|---|---|
| `onInitialize()` | Once, after construction; View QML object is alive but page is not yet visible | Heavy data load, async fetches, allocating large buffers, subscribing to long-lived signals |
| `onActivating()` | Push or reveal transition begins | Animation prep, prefetch |
| `onActivated()` | Page is fully visible and interactive | Start timers, sensor polling, focus controls, increment per-visit counters |
| `onDeactivating()` | Page is starting to leave (transition begins) | Save draft state, pause animations before they are hidden |
| `onDeactivated()` | Page is no longer visible; ViewModel is still alive | Stop timers, release camera/sensor handles, pause expensive subscriptions |
| `onFinalize()` | Once, from `Component.onDestruction`; View is about to be destroyed | Persist state, free large buffers, unregister from external systems |

#### The full sequence — a worked example

User opens the app (View1 pushed), navigates to View2, then back to View1,
then closes the app:

```
View1 pushed:
  Navigator: new View1ViewModel(services)   ◄ constructor (lightweight)
  Component.onCompleted   → onInitialize()  ◄ heavy setup
  StackView.onActivating  → onActivating()
  StackView.onActivated   → onActivated()   ◄ View1 is interactive

User taps "Go to View 2":
  StackView.onDeactivating → onDeactivating()  on View1
  StackView.onDeactivated  → onDeactivated()   on View1
  Navigator: new View2ViewModel(services)
  Component.onCompleted   → onInitialize()  on View2
  StackView.onActivating  → onActivating()  on View2
  StackView.onActivated   → onActivated()   on View2

Now on the stack: [View1 (inactive, alive), View2 (active)]

User taps "Back":
  StackView.onDeactivating → onDeactivating()  on View2
  StackView.onDeactivated  → onDeactivated()   on View2
  Component.onDestruction  → onFinalize()      on View2 ◄ last cleanup
  View2 QML destroyed; View2ViewModel GC'd shortly after
  StackView.onActivating  → onActivating()  on View1  ◄ re-reveal
  StackView.onActivated   → onActivated()   on View1
                                                       ◄ note: NOT
                                                         onInitialize
                                                         again — that
                                                         only fires once
                                                         per instance

User closes the app (View1 pop on shutdown):
  StackView.onDeactivating → onDeactivating()
  StackView.onDeactivated  → onDeactivated()
  Component.onDestruction  → onFinalize()    ◄ last chance to persist
```

#### Why six hooks, not two

The minimum useful set is `onInitialize` + `onFinalize` (matched lifetime
pair). The next most useful is `onActivated` + `onDeactivated`
(matched visibility pair) for resources that should only run while the
user is looking at the page. `onActivating` + `onDeactivating` are the
transition-aware variants — useful for animation coordination and for
*"about to leave, save now"* patterns where you want to act before the
page is fully gone.

Each hook ships with a default log line that uses `objectName()` as the
label. Derived ViewModels set their object name in the constructor
(`setObjectName("View1VM")`) and inherit the entire diagnostic log set
for free — the demo VMs only override the one hook (`onActivated`) that
has page-local state to update.

#### Override pattern

When a derived class overrides a hook, it can choose to keep the
base's diagnostic log or replace it entirely:

```cpp
// Keep the log, then add page-local work:
void View1ViewModel::onActivated() {
    ViewModelBase::onActivated();   // ◄ inherited log line
    ++m_enterCount;                 // ◄ page-local state
    emit enterCountChanged();
}

// Replace the log entirely (when you have your own logging):
void View1ViewModel::onInitialize() {
    fetchProductCatalog();          // no base call — default log skipped
}
```

This is the standard "call the base, then add work" Qt idiom. A
ViewModel that needs no page-specific behaviour beyond the demo logs
does not have to override anything.

#### Why the View wires the hooks, not the navigator

Lifecycle fundamentally belongs to the QML view object — it is the View
that StackView pushes, transitions, and destroys. The navigator only
knows about construction. Wiring the hooks in the View keeps the
relationship between a QML signal and a C++ method visible at the call
site, which is easier to debug than a hidden C++ subscription set up by
the navigator.

#### Where the wiring physically lives

The six wirings are mechanical and identical for every page — there is
no per-page customisation. They are extracted into one file,
`ui-kit/views/ViewBase.qml`:

```qml
// ui-kit/views/ViewBase.qml
Item {
    id: root
    required property var viewModel

    Component.onCompleted:    viewModel.onInitialize()
    Component.onDestruction:  viewModel.onFinalize()
    StackView.onActivating:   viewModel.onActivating()
    StackView.onActivated:    viewModel.onActivated()
    StackView.onDeactivating: viewModel.onDeactivating()
    StackView.onDeactivated:  viewModel.onDeactivated()
}
```

Concrete views inherit from `ViewBase` and place the page's `Form` as a
filling child:

```qml
// ui-kit/views/View1.qml
ViewBase {
    id: root

    View1Form {
        anchors.fill: parent
        statusText: "value: " + root.viewModel.value + ...
        goView2Btn.onClicked: root.StackView.view.pushRoute("View2")
    }
}
```

`ViewBase.qml` is pure QML — standard Qt imports only, no navigator,
no ViewModel type. It satisfies the ui-kit purity rule and is loadable
in the `qml` runtime and in Qt Design Studio.

Why a base type and not just copy-paste? Because the wiring is genuinely
identical at every site. There is no per-page variation that an explicit
boilerplate would be defending. The form contents are the variable part
— the wiring is not.

### Where the route table lives

The route table is defined in exactly one place: `app-main/main.cpp`.
Adding a new page is three edits, in this order:

1. Create `viewmodels/ViewNViewModel.h/.cpp` deriving from `ViewModelBase`.
2. Create `ui-kit/forms/ViewNForm.ui.qml` and `ui-kit/views/ViewN.qml`.
3. Add one `navigator->registerRoute<ViewNViewModel>("ViewN", QUrl(...))`
   line in `main.cpp`.

No QML import changes. No ui-kit `CMakeLists.txt` re-shuffle beyond adding
the new files to the `QML_FILES` list.

### Why the navigator lives in `core/`, not in `ui-kit/`

`ui-kit/` is forbidden from importing project-owned C++ modules — that is
the rule that keeps it openable in the `qml` runtime and in Qt Design
Studio. So the navigator cannot live there.

`AppShell.qml` *does* live in `ui-kit/`, because it is pure QML — it
receives the navigator as a duck-typed `required property var navigator`,
not as an imported type, in exactly the same way Views receive the
ViewModel.

---

## Wiring domain services

The starter ships with one demo domain class (`AppDataSource`). Real
projects grow more — repositories, hardware facades, auth state, settings
stores, telemetry clients, etc. This section explains how new domain
classes are wired into the application.

### The mechanism — `AppServices`

`core/AppServices.h` declares a plain struct of non-owning pointers to
domain instances. `main.cpp` constructs the instances and assigns them
to the struct. The `ViewNavigator` carries the struct and hands it to
every ViewModel constructor by const reference. Each ViewModel stores
its own copy and reads only the services it actually needs.

Three small design choices make this work the way it does:

- **Why a struct, not a class with getters.** Adding a service is one
  line, with no DI ceremony. All members default to `nullptr`, so a
  partially-wired bundle is detectable at the use site — the typical
  pattern is `if (auto *s = m_services.foo) { ... }` which both
  null-checks and documents which services this VM actually consumes.
- **Why non-owning pointers.** Lifetime is owned by `main()` via
  `QObject` parenting to the `QGuiApplication`. This is the simplest
  ownership rule in Qt and it works for all app-lifetime services. The
  struct just carries the pointers; it does not own anything.
- **Why pass by const reference and store by value.** The struct is
  small (a handful of pointers) so copies are cheap, and storing by
  value gives every ViewModel a stable snapshot for its own lifetime.
  The bundle is immutable per-VM, but the pointed-to service objects
  remain mutable for normal runtime updates.

### Why this lives in `core/`

Every ViewModel in the project has the `AppServices` struct in its
constructor signature, so the *struct definition* is part of the
reusable infrastructure. The *members* are user-defined (one line per
domain service) — that part grows with the application.

### Adding a new service — a worked example

Suppose we add `AuthState` (the user's signed-in/out state). The total
set of changes is **three files, four lines**:

```diff
  // core/AppServices.h
  class AppDataSource;
+ class AuthState;

  struct AppServices {
      AppDataSource *dataSource = nullptr;
+     AuthState     *auth       = nullptr;
  };
```

```diff
  // domain/CMakeLists.txt
  qt_add_library(app_domain STATIC
      AppDataSource.h AppDataSource.cpp
+     AuthState.h     AuthState.cpp
  )
```

```diff
  // app-main/main.cpp
  AppServices services;
  services.dataSource = new AppDataSource(&app);
+ services.auth       = new AuthState(&app);
  navigator->setServices(services);
```

What does **not** change:

- `core/ViewNavigator.h/.cpp` — the `registerRoute<T>()` template is unchanged.
- Existing `ViewN.qml`, `ViewNForm.ui.qml` — unchanged.
- `View1ViewModel`, `View2ViewModel`, `View3ViewModel` constructors —
  signatures unchanged. They simply ignore `m_services.auth` until they
  decide to consume it.
- `ui-kit/` — unchanged.
- `tests/` — unchanged.

A ViewModel that *does* want to consume the new service adds a few lines
inside its own constructor and accessors:

```cpp
// View1ViewModel.cpp
View1ViewModel::View1ViewModel(const AppServices &services, QObject *parent)
    : ViewModelBase(parent), m_services(services)
{
    if (auto *src = m_services.dataSource) {
        connect(src, &AppDataSource::valueChanged,
                this, &View1ViewModel::valueChanged);
    }
    if (auto *auth = m_services.auth) {            // ◄ added
        connect(auth, &AuthState::signedInChanged,
                this, &View1ViewModel::userNameChanged);
    }
}

QString View1ViewModel::userName() const            // ◄ added
{
    return m_services.auth ? m_services.auth->userName() : QString();
}
```

The `if (auto *p = m_services.foo)` pattern is doing two jobs at once:
it makes the VM tolerant of partially-wired bundles (which tests rely on
— see below) and it documents which services this VM actually consumes.
A reader skimming a 200-line VM file can scan the constructor's null
checks to see the dependency list.

### What this buys for testing

Each ViewModel takes exactly one argument — a struct of pointers — so
tests construct only what they need:

```cpp
// tests/viewmodels/tst_view1_viewmodel.cpp
void TestView1ViewModel::reflects_data_source_value()
{
    AppDataSource src;
    AppServices   services;
    services.dataSource = &src;     // auth left nullptr on purpose

    View1ViewModel vm(services);

    src.onTick();
    QCOMPARE(vm.value(), 1);
}

void TestView1ViewModel::shows_user_when_signed_in()
{
    AuthState   auth;
    AppServices services;
    services.auth = &auth;          // dataSource left nullptr on purpose

    View1ViewModel vm(services);

    auth.signIn(QStringLiteral("Benjamin"));
    QCOMPARE(vm.userName(), QStringLiteral("Benjamin"));
}
```

No singletons to reset between tests. No "the previous test left the
world in state X" debugging. Each test owns its services for its own
duration.

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

## How the five `CMakeLists.txt` files work

Each layer has its own `CMakeLists.txt`, and reading them in order is a
short tutorial on modern Qt 6 CMake.

### `core/CMakeLists.txt` — C++ QML module (infrastructure)

A `qt_add_qml_module` registering the URI `App.Core` and three C++ types:
`ViewModelBase` (`QML_UNCREATABLE`), `AppServices` (no QML registration —
plain struct), and `ViewNavigator` (`QML_SINGLETON`). Demonstrates how to
ship reusable QML-aware C++ types as a self-contained module.

### `domain/CMakeLists.txt` — plain C++

A static library, no Qt-specific QML magic:

- `qt_add_library(app_domain STATIC ...)`
- A list of `.cpp` and `.h` files
- `target_link_libraries(app_domain PUBLIC Qt6::Core)`

Anyone who has used CMake at all will recognise this. It is here to
establish the pattern: each layer is its own CMake target, with explicit
sources and explicit dependencies.

### `viewmodels/CMakeLists.txt` — C++ QML module (demo)

The same shape as `core/CMakeLists.txt`, but for demo ViewModels. It
links to `app_domain` (because each VM forwards domain state) and
`app_core` (because each VM derives from `ViewModelBase` and stores an
`AppServices`). Each new ViewModel adds two files (`.h` + `.cpp`) to
the `SOURCES` list — that is the full cost.

### `ui-kit/CMakeLists.txt` — pure-QML module

A `qt_add_qml_module` call *without* any C++ source files. Just QML and
resources. Demonstrates:

- That a QML module does not require C++ to be a module
- How to list `.ui.qml`, `.qml`, and resource files
- That the module has no `target_link_libraries` to project C++ targets —
  this is the structural rule that keeps `ui-kit/` previewable

A common mistake in real projects is to add a small "helper" C++ class to
the UI module and link it. Once that link exists, the UI module is no
longer openable in Qt Design Studio without the full build. This file is
deliberately structured so that mistake is visible the moment it happens.

### `app-main/CMakeLists.txt` — the executable

`qt_add_executable` plus links to all four module libraries. This is
where everything comes together. Demonstrates:

- How to register the application as a QML module of its own
  (`App.Integration`) — the only QML module allowed to bridge C++ and the
  ui-kit
- How to link all four other modules
- Where the `main.cpp` entry point lives

### Why this matters for teams that find CMake hard

Most Qt projects in the field have one giant top-level `CMakeLists.txt`
with everything in it. That works for very small projects but becomes
unreadable fast. The modular alternative — one `CMakeLists.txt` per
folder — is documented in Qt's own docs but is rarely shown as a worked
example for a complete project.

This template is that worked example. Five files. Same shape. Each one
does one thing. A developer who copies them and renames them will end up
with a correctly-structured Qt 6 CMake project, even if they have not
internalised every option of `qt_add_qml_module` yet.

---

## The host-testable UI workflow

> **`ui-kit/forms/` is the developer's workspace for UI iteration. Every
> form file loads standalone on the developer's laptop, throughout the
> project's life.**

Open a single form with the standard `qml` runtime:

```bash
qml ui-kit/forms/View1Form.ui.qml
```

You see the layout with its hardcoded defaults — no ViewModel, no
navigator, no full build. Adjust spacing, colour, alignment, reload, see
the result immediately.

When new UI is needed:

1. Edit or create a `Form.ui.qml` file under `ui-kit/forms/` — instantly
   verifiable with `qml`.
2. Expose new elements via `property alias` in the form.
3. Bind the alias to the ViewModel in the corresponding `views/` file.
4. Add the supporting property to the matching `ViewNViewModel.h/.cpp`.

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
2. No QML file under `ui-kit/` imports `App.Core`, `App.ViewModels`, or
   `App.Integration` (the project-owned C++ modules).
3. `ui-kit/CMakeLists.txt` does not link to any of the `app_core`,
   `app_domain`, `app_viewmodels`, or `qt-quick-starter` (executable)
   targets.

A team that wants to automate the check can add a small CI script. The
template intentionally does not ship one, to keep the scaffolding small.

---

## The required-property injection pattern

Views never import the ViewModel directly. They never import the
navigator either. The lifecycle wiring lives in `ViewBase.qml`; concrete
views inherit from it and define the page's content as a child:

```qml
// ui-kit/views/View1.qml
ViewBase {
    id: root
    // viewModel is inherited from ViewBase as a required property,
    // so pushing this view without one fails loudly at load time.

    View1Form {
        anchors.fill: parent

        statusText: "value: " + root.viewModel.value
                  + "  |  " + root.viewModel.status
                  + "  |  enters: " + root.viewModel.enterCount

        goView2Btn.onClicked: root.StackView.view.pushRoute("View2")
    }
}
```

The parent decides what to inject:

```qml
// ui-kit/previews/AppPreview.qml — developer preview
View1Form { anchors.fill: parent }     // no ViewModel needed, form is self-contained
```

```qml
// app-main/Main.qml — real application
AppShell { navigator: ViewNavigator; initialRoute: "View1" }
```

`AppShell` then receives a freshly-built ViewModel from the navigator
for each `pushRoute()` call and injects it as a property on the View.

Because the View sees only duck-typed properties, **the same view file works
with any object** that exposes the right properties.

---

## Navigation

Views trigger navigation by calling `pushRoute(name)` on the StackView,
not by constructing URLs themselves:

```qml
// ui-kit/views/View2.qml
backBtn.onClicked:    StackView.view.pop()
goView3Btn.onClicked: StackView.view.pushRoute("View3")
```

`AppShell.qml` owns the `StackView` and the `pushRoute()` helper. Views
do not know about URLs, ViewModel constructors, or the navigator type.
They only know route names — strings. That is the one piece of coupling
between the View layer and the route table, and it is intentionally tiny.

---

## Test strategy

### Host-side UI layout tests — `tests/ui-visual/`

`QtQuickTest` tests that instantiate form files directly. No ViewModel
needed, no navigator needed, no C++ needed. Verify alias properties,
default values, presence of elements:

```qml
View1Form { id: form }
TestCase {
    function test_default_status() {
        compare(form.statusText, "value: 0  |  idle")
    }
}
```

Because the form files have no ViewModel dependency, **these tests do not
need to be updated when the ViewModel structure changes**.

### Domain unit tests — `tests/domain/`

Plain `QtTest` for `domain/` classes. No display, no QML. Run on any
runner.

### ViewModel unit tests — `tests/viewmodels/`

Plain `QtTest` for ViewModels. No display, no QML, no navigator. Each
test builds the `AppServices` bundle it needs and constructs the VM
directly. This exercises the same construction path the navigator uses
at runtime, minus the QML ownership transfer.

The template ships `tst_view1_viewmodel.cpp` as the running proof of the
`AppServices` pattern — if domain injection ever stops being testable
that way, this file fails. View2 / View3 ViewModels share the same shape
by design, so they are not separately tested unless their behaviour
diverges.

### Deliberately out of scope

- Target-side integration tests (board-in-the-loop).
- Screenshot-diff visual regression tests — easy to add on top of
  `tests/ui-visual/` with `QQuickItemGrabResult`.

---

## What the rules take away from you

An honest list of what you give up:

- **You cannot use a C++ singleton directly in a `ui-kit` view.** Route
  it through the ViewModel that the view receives via `required property`,
  or expose it through the navigator.
- **You cannot register a custom QML element in C++ and use it in
  `ui-kit`.** Keep it in `app-main/Main.qml` and inject it as a property,
  or write a pure-QML stand-in for preview.
- **You cannot use `QtLocation` / `QtMultimedia` in `ui-kit` if the local
  environment does not have those plugins.** Route through the ViewModel.
- **You cannot construct a ViewModel from QML.** The navigator is the one
  place that builds them, with the right domain dependencies. This is
  enforced by `QML_UNCREATABLE`.

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
| **View** | A `.qml` wrapper that binds a ViewModel to a Form and triggers navigation |
| **ViewModel** | A C++ class derived from `ViewModelBase`, page-scoped, instantiated by the `ViewNavigator` |
| **Route** | A name (string) mapped by the `ViewNavigator` to a view URL plus a ViewModel factory |
| **ViewNavigator** | The `App.Core` `QML_SINGLETON` that owns the route table and constructs ViewModels |
| **AppServices** | A plain struct of non-owning pointers to domain services. The single argument every ViewModel constructor takes. Lives in `core/AppServices.h`. |
| **`core/`** | The reusable infrastructure folder. `ViewModelBase`, `AppServices`, `ViewNavigator`. Treated as a small in-tree library — depended on, not edited. |
