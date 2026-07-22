### Task 2: RuneFader — green/purple arrows + modifier drag

**Files:**
- Modify: `Source/gravel/RuneFader.h`, `Source/gravel/RuneFader.cpp`

**Interfaces:**
- Consumes: `ModulationState` fields (no direct coupling — stores own float amounts like `targetModulation`)
- Produces: `setSlopeModulation(float)`, `setJitterModulation(float)`, `onSlopeDragged`, `onJitterDragged` callbacks; green/purple arrow colors; 3 drag modes based on modifier keys

- [ ] **Step 1: Add new members to RuneFader.h**

Add after existing `targetModulation` / `appliedModulation` / `arrow` members:

```cpp
    void setSlopeModulation (float amount);
    float getSlopeModulation() const { return slopeTarget; }
    void setJitterModulation (float amount);
    float getJitterModulation() const { return jitterTarget; }

    void setSlopeArrowColour (juce::Colour c) { slopeArrow = c; repaint(); }
    void setJitterArrowColour (juce::Colour c) { jitterArrow = c; repaint(); }

    std::function<void(float)> onSlopeDragged;
    std::function<void(float)> onJitterDragged;

    // in private section, alongside existing modulation members:
    float slopeTarget = 0.0f;
    float slopeSmoothed = 0.0f;
    float jitterTarget = 0.0f;
    float jitterSmoothed = 0.0f;
    bool slopeDragActive = false;
    bool jitterDragActive = false;
    juce::Colour slopeArrow;
    juce::Colour jitterArrow;

    void paintGreenArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintPurpleArrow (juce::Graphics& g, float x, float railTop, float railBottom);
    void paintHorizontalGreenArrow (juce::Graphics& g, float railLeft, float railRight, float y);
    void paintHorizontalPurpleArrow (juce::Graphics& g, float railLeft, float railRight, float y);
```

Initialize `slopeArrow` and `jitterArrow` in the constructor initializer list:
```cpp
      slopeArrow (juce::Colour (0xff4caf50).withAlpha (0.55f)),
      jitterArrow (juce::Colour (0xffab47bc).withAlpha (0.55f)),
```

- [ ] **Step 2: Implement setter methods in RuneFader.cpp**

```cpp
void RuneFader::setSlopeModulation (float amount)
{
    slopeTarget = juce::jlimit (-1.0f, 1.0f, amount);
}

void RuneFader::setJitterModulation (float amount)
{
    jitterTarget = juce::jlimit (-1.0f, 1.0f, amount);
}
```

- [ ] **Step 3: Modify mouseDown() to check modifier keys**

In both primary and secondary branches, before the existing edge-zone logic, add:
```cpp
    if (event.mods.isShiftDown())
    {
        slopeDragActive = true;
        dragDirectionNegative = (relativeX < 0.5f);  // or relativeY
        slopeTarget = 0.0f;
        repaint();
        return;
    }
    if (event.mods.isAltDown())
    {
        jitterDragActive = true;
        dragDirectionNegative = (relativeX < 0.5f);
        jitterTarget = 0.0f;
        repaint();
        return;
    }
```

- [ ] **Step 4: Modify mouseDrag() to handle 3 active flags**

```cpp
    if (slopeDragActive)
    {
        // compute slopeTarget from drag distance (same formula as orange arrow)
        if (onSlopeDragged) onSlopeDragged(slopeTarget);
        repaint();
        return;
    }
    if (jitterDragActive)
    {
        // compute jitterTarget from drag distance
        if (onJitterDragged) onJitterDragged(jitterTarget);
        repaint();
        return;
    }
    // existing arrowDragActive logic...
```

- [ ] **Step 5: Modify mouseUp()**

```cpp
    if (slopeDragActive)
    {
        slopeDragActive = false;
        if (std::abs(slopeTarget) <= 0.01f)
        {
            slopeTarget = 0.0f;
            if (onSlopeDragged) onSlopeDragged(0.0f);
            repaint();
        }
        return;
    }
    if (jitterDragActive)
    {
        jitterDragActive = false;
        if (std::abs(jitterTarget) <= 0.01f)
        {
            jitterTarget = 0.0f;
            if (onJitterDragged) onJitterDragged(0.0f);
            repaint();
        }
        return;
    }
    // existing arrowDragActive logic...
```

- [ ] **Step 6: Add painting calls in paint()**

After white arrow painting, for primary:
```cpp
    slopeSmoothed += (slopeTarget - slopeSmoothed) * 0.18f;
    paintHorizontalGreenArrow(g, railLeft, railRight, y);
    jitterSmoothed += (jitterTarget - jitterSmoothed) * 0.18f;
    paintHorizontalPurpleArrow(g, railLeft, railRight, y);
```

Same for secondary (vertical) with paintGreenArrow/paintPurpleArrow.

- [ ] **Step 7: Implement green/purple paint methods**

Copy `paintHorizontalOrangeArrow` and `paintOrangeArrow` verbatim, replacing `arrow` with `slopeArrow`/`jitterArrow` and renaming.

---

