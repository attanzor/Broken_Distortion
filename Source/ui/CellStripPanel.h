#pragma once

#include <JuceHeader.h>

#include <array>
#include <functional>

class CellStripPanel final : public juce::Component
{
public:
    static constexpr int kNumCells = 9;
    static constexpr int kWaveTableSize = 513;

    CellStripPanel();

    void paint (juce::Graphics&) override;
    void resized() override;
    void mouseDown (const juce::MouseEvent&) override;

    void setCellShapeData (int cellIndex, const std::array<float, kWaveTableSize>& data);
    void setSelectedCell (int cellIndex);
    int getSelectedCell() const { return selectedCell; }

    juce::Slider& getMorphSlider (int cellIndex);

    std::function<void (int cellIndex)> onCellClicked;

private:
    struct MiniCell
    {
        std::array<float, kWaveTableSize> shapeData {};
        std::unique_ptr<juce::Slider> morphSlider;
        juce::Rectangle<int> bounds;
        juce::Rectangle<int> shapeBounds;
        juce::Rectangle<int> sliderBounds;
    };

    std::array<MiniCell, kNumCells> miniCells;
    int selectedCell = -1;

    void drawShape (juce::Graphics& g, juce::Rectangle<int> bounds,
                    const std::array<float, kWaveTableSize>& data, float alpha);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CellStripPanel)
};
