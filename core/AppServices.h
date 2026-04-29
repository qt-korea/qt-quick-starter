// =============================================================================
//  core/AppServices.h
//
//  Bundle of long-lived application services that ViewModels may need.
//
//  This is *not* a service locator and *not* a singleton. It is a plain
//  POD-ish struct of non-owning pointers. The actual service objects are
//  constructed and owned by main(); this struct just carries pointers to
//  them so every ViewModel constructor can take the same single argument.
//
//  ─── Why a struct, not a class ─────────────────────────────────────────
//
//  Adding a new service is one line — no getters, no setters, no DI
//  ceremony. All members default to nullptr, so a partially-wired
//  instance is detectable at the use site (the typical pattern is
//  `if (auto *s = m_services.foo) { ... }` which both null-checks and
//  documents which services this VM actually consumes).
//
//  ─── Why injected, not global ──────────────────────────────────────────
//
//  An earlier iteration of this template used a shared ViewModel
//  singleton. Shared mutable singletons hurt testability, hide
//  dependencies, and become merge-conflict hotspots. Domain services
//  would suffer the same problems if made global. AppServices keeps
//  domain instances injected while letting every ViewModel share a
//  uniform construction signature.
//
//  ─── How to add a new service ──────────────────────────────────────────
//
//    1. Forward-declare the class below.
//    2. Add one `Foo *foo = nullptr;` line to the struct.
//    3. In main.cpp, allocate the instance and assign it to the bundle.
//    4. In any VM that needs it, read m_services.foo.
//
//  No ViewNavigator change. No registerRoute<T>() change. No change to
//  other VMs. See ARCHITECTURE.md → "Wiring domain services" for the
//  full worked example.
//
//  ─── What goes here ────────────────────────────────────────────────────
//
//  Long-lived domain objects whose lifetime equals the application's:
//  data sources, repositories, hardware facades, auth state, settings
//  stores. Page-scoped state stays inside the ViewModel that owns the
//  page; transient values stay as function arguments.
//
//  ─── Why this lives in core/ ───────────────────────────────────────────
//
//  Every ViewModel in the project has the AppServices struct as part of
//  its constructor signature, so the *struct definition* is part of the
//  reusable infrastructure. The *members* are user-defined (one line per
//  domain service) — that part grows with the application. Definition is
//  infrastructure (core/), members are the user's wiring.
// =============================================================================

#pragma once

// Forward-declare every domain class the bundle exposes.
// The demo ships AppDataSource (defined in domain/); add new ones here
// when you grow the application.
class AppDataSource;
// class AuthState;        // future
// class SettingsRepo;     // future

struct AppServices
{
    AppDataSource *dataSource = nullptr;

    // Future services land here, one line each:
    //
    // AuthState     *auth     = nullptr;
    // SettingsRepo  *settings = nullptr;
};
