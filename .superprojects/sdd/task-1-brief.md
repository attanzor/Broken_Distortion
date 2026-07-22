### Task 1: ModulationState — new atomic fields

**Files:**
- Modify: `Source/gravel/ModulationState.h`

**Interfaces:**
- Consumes: existing `ModulationState` struct pattern
- Produces: 8 new `std::atomic<float>` fields: `slopeSpeed`, `slopeSpikyness`, `slopeAmount`, `slopeApplied`, `jitterSpeed`, `jitterSpikyness`, `jitterAmount`, `jitterApplied`; updated `toValueTree()`/`fromValueTree()`; enable toggles `slopeEnabled`, `jitterEnabled`

- [ ] **Step 1: Add the 8 new atomic fields + 2 enable toggles**

After line 18 (`explModEnabled`), add:
```cpp
    std::atomic<float> slopeSpeed    { 10.0f };
    std::atomic<float> slopeSpikyness { 1.0f };
    std::atomic<float> slopeAmount   { 0.0f };
    std::atomic<float> slopeApplied  { 0.0f };
    std::atomic<bool>  slopeEnabled  { true };
    std::atomic<float> jitterSpeed   { 1.0f };
    std::atomic<float> jitterSpikyness { 1.0f };
    std::atomic<float> jitterAmount  { 0.0f };
    std::atomic<float> jitterApplied { 0.0f };
    std::atomic<bool>  jitterEnabled { true };
```

- [ ] **Step 2: Update `toValueTree()`**

Add after `tree.setProperty("explModEnabled", ...)`:
```cpp
        tree.setProperty ("slopeSpeed",    slopeSpeed.load(),    nullptr);
        tree.setProperty ("slopeSpikyness", slopeSpikyness.load(), nullptr);
        tree.setProperty ("slopeAmount",   slopeAmount.load(),   nullptr);
        tree.setProperty ("slopeEnabled",  slopeEnabled.load(),  nullptr);
        tree.setProperty ("jitterSpeed",   jitterSpeed.load(),   nullptr);
        tree.setProperty ("jitterSpikyness", jitterSpikyness.load(), nullptr);
        tree.setProperty ("jitterAmount",  jitterAmount.load(),  nullptr);
        tree.setProperty ("jitterEnabled", jitterEnabled.load(), nullptr);
```

- [ ] **Step 3: Update `fromValueTree()`**

Add after `explModEnabled.store(...)`:
```cpp
        slopeSpeed.store   (tree.getProperty ("slopeSpeed", 10.0f));
        slopeSpikyness.store (tree.getProperty ("slopeSpikyness", 1.0f));
        slopeAmount.store  (tree.getProperty ("slopeAmount", 0.0f));
        slopeEnabled.store (static_cast<bool> (tree.getProperty ("slopeEnabled", true)));
        jitterSpeed.store  (tree.getProperty ("jitterSpeed", 1.0f));
        jitterSpikyness.store (tree.getProperty ("jitterSpikyness", 1.0f));
        jitterAmount.store (tree.getProperty ("jitterAmount", 0.0f));
        jitterEnabled.store (static_cast<bool> (tree.getProperty ("jitterEnabled", true)));
```

---

